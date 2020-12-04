#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <regex>
#include <utility>

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

} // namespace local

namespace passport
{
  bool is_byr_valid(const std::string &value)
  {
    auto byr = std::stoul(value, 0, 10);
    return byr >= 1920 && byr <= 2002;
  }
  bool is_iyr_valid(const std::string &value)
  {
    auto iyr = std::stoul(value, 0, 10);
    return iyr >= 2010 && iyr <= 2020;
  }
  bool is_eyr_valid(const std::string &value)
  {
    auto eyr = std::stoul(value, 0, 10);
    return eyr >= 2020 && eyr <= 2030;
  }
  bool is_hgt_valid(const std::string &value)
  {
    const std::regex re{"([0-9]+)(cm|in)"};
    std::smatch pieces_match;

    if (std::regex_match(value, pieces_match, re))
    {
      if (pieces_match.size() == 3)
      {
        auto value = std::stoul(pieces_match[1]);
        auto type = pieces_match[2];

        return (type == "cm" && value >= 150 && value <= 193) || (type == "in" && value >= 59 && value <= 76);
      }
    }
    return false;
  }
  bool is_hcl_valid(const std::string &value)
  {
    const std::regex re{"#([0-9a-f]){6})"};
    std::smatch m;

    auto valid_hcl = std::regex_match(value, m, re);

    if (valid_hcl)
    {
      std::cout << "valid eye color: " << value << std::endl;
    }

    return valid_hcl;
  }
  bool is_ecl_valid(const std::string &value)
  {
    const std::vector<std::string> colors{
        "amb",
        "blu",
        "brn",
        "gry",
        "grn",
        "hzl",
        "oth",
    };

    return std::find(colors.begin(), colors.end(), value) != colors.end();
  }
  bool is_pid_valid(const std::string &value)
  {
    const std::regex re{"[0-9]{9}"};
    std::smatch m;

    return std::regex_match(value, m, re);
  }
} // namespace passport

int main()
{
  std::ifstream ifile("./input.txt");

  if (!ifile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  const std::map<std::string, std::function<bool(const std::string &)>> passport_validation_map{
      {"byr", passport::is_byr_valid},
      {"iyr", passport::is_iyr_valid},
      {"eyr", passport::is_eyr_valid},
      {"hgt", passport::is_hgt_valid},
      {"hcl", passport::is_hcl_valid},
      {"ecl", passport::is_ecl_valid},
      {"pid", passport::is_pid_valid},
  };

  size_t valid_passports = 0;

  {
    std::string all_passport_lines;

    while (!ifile.eof())
    {
      std::string passport_line;
      std::getline(ifile, passport_line, '\n');

      if (passport_line.empty())
      {
        local::trim(all_passport_lines);

        std::cout << "line: " << all_passport_lines << std::endl;
        auto fields_splitted_by_space = local::split_string(all_passport_lines, "[\\s]+");

        std::map<std::string, std::string> passport_fields{};
        for (const auto &field_by_space : fields_splitted_by_space)
        {
          auto splitted_field = local::split_string(field_by_space, "[\\:]+");
          passport_fields.insert(std::make_pair(splitted_field[0], splitted_field[1]));
        }

        if (std::all_of(passport_validation_map.begin(), passport_validation_map.end(), [&passport_fields](auto const &passport_validation_field) {
              if (passport_fields.find(passport_validation_field.first) == passport_fields.end())
              {
                return false;
              }

              return passport_validation_field.second(passport_fields[passport_validation_field.first]);
            }))
        {
          ++valid_passports;
        }

        all_passport_lines.clear();
      }
      else
      {
        all_passport_lines.append(" ");
        all_passport_lines.append(passport_line);
      }
    }
  }

  std::cout << valid_passports << std::endl;

  return 0;
}