#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <regex>
#include <utility>
#include <set>

namespace local
{
  std::vector<std::string> split_string(const std::string &subject, const std::string &delim)
  {
    const std::regex re{delim};
    std::vector<std::string> container{
        std::sregex_token_iterator(subject.begin(),
                                   subject.end(),
                                   re,
                                   -1),
        std::sregex_token_iterator()};
    return container;
  }
  static inline void ltrim(std::string &s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
              return !std::isspace(ch);
            }));
  }

  // trim from end (in place)
  static inline void rtrim(std::string &s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
              return !std::isspace(ch);
            }).base(),
            s.end());
  }

  // trim from both ends (in place)
  static inline void trim(std::string &s)
  {
    ltrim(s);
    rtrim(s);
  }

  // trim from start (copying)
  static inline std::string ltrim_copy(std::string s)
  {
    ltrim(s);
    return s;
  }

  // trim from end (copying)
  static inline std::string rtrim_copy(std::string s)
  {
    rtrim(s);
    return s;
  }

  // trim from both ends (copying)
  static inline std::string trim_copy(std::string s)
  {
    trim(s);
    return s;
  }

  size_t find_at_zero_mid(
      const size_t initial_high,
      const std::string &str,
      const size_t str_length,
      const std::function<bool(const char)> &is_high_or_low,
      const std::function<bool(const char)> &is_high)
  {
    size_t index = 0, low = 0, mid = initial_high / 2, high = initial_high - 1;

    while (index < str_length && is_high_or_low(str[index]))
    {
      if (is_high(str[index]))
      {
        high -= mid;
      }
      else
      {
        low += mid;
      }

      mid /= 2;
      ++index;
    }

    return is_high(str[index]) ? high : low;
  }

} // namespace local

namespace weird_plane_system
{
  using position = struct
  {
    size_t row;
    size_t column;
  };

  const size_t plane_rows = 128;
  const size_t plane_columns = 8;

  const size_t get_seat_id(const size_t row, const size_t column)
  {
    return (row * 8) + column;
  }

  const size_t get_seat_id(const position &pos)
  {
    return get_seat_id(pos.row, pos.column);
  }

  const bool is_front_row_specification(const char spec)
  {
    return spec == 'F';
  }

  const bool is_back_row_specification(const char spec)
  {
    return spec == 'B';
  }

  const bool is_row_specification(const char spec)
  {
    return is_front_row_specification(spec) || is_back_row_specification(spec);
  }

  const bool is_right_column_specification(const char spec)
  {
    return spec == 'R';
  }

  const bool is_left_column_specification(const char spec)
  {
    return spec == 'L';
  }

  const bool is_column_specification(const char spec)
  {
    return is_right_column_specification(spec) || is_left_column_specification(spec);
  }

  position get_position(const std::string &pass_string)
  {
    const auto pass_string_length = pass_string.size();
    position pos;

    pos.row = local::find_at_zero_mid(
        plane_rows,
        pass_string,
        7,
        is_row_specification,
        is_front_row_specification);
    pos.column = local::find_at_zero_mid(
        plane_columns,
        &pass_string[7],
        3,
        is_column_specification,
        is_left_column_specification);

    return pos;
  }

} // namespace weird_plane_system

int main()
{
  std::ifstream ifile("./input.txt");

  if (!ifile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  size_t max_seat_id = 0;
  std::set<size_t> seat_ids{};

  {
    std::string plane_pass;

    while (!ifile.eof())
    {
      std::getline(ifile, plane_pass, '\n');

      if (plane_pass.empty())
      {
        continue;
      }

      auto current_pos = weird_plane_system::get_position(plane_pass);
      size_t current_seat_id = weird_plane_system::get_seat_id(current_pos);
      seat_ids.insert(current_seat_id);

      if (current_seat_id > max_seat_id)
      {
        max_seat_id = current_seat_id;
      }
    }
  }

  std::cout << "max seat id: " << max_seat_id << std::endl;

  auto it = seat_ids.begin();
  size_t prev_seat_id = *it;

  ++it;
  size_t current_seat_id = *it;

  for (; it != seat_ids.end(); ++it, prev_seat_id = current_seat_id, current_seat_id = *it)
  {
    if (current_seat_id - 1 != prev_seat_id)
    {
      std::cout << "missing seat id: " << (current_seat_id - 1) << std::endl;
      break;
    }
  }

  return 0;
}