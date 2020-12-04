#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

struct TobogganPassword
{
  unsigned int chars['a' - 'A'];
  unsigned min_count;
  unsigned max_count;
  char letter;
};

int main()
{
  std::ifstream ifile("./day-2-input.txt");

  if (!ifile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  size_t valid_pws = 0;

  {
    std::string line;
    while (!ifile.eof())
    {
      std::getline(ifile, line, '\n');

      if (line.empty())
      {
        continue;
      }

      auto dash_index = line.find("-");
      auto space_index = line.find(" ", dash_index);
      auto colon_space_index = line.find(": ", space_index);

      TobogganPassword pw{};

      pw.min_count = std::stoul(line.substr(0, dash_index), 0, 10);
      pw.max_count = std::stoul(line.substr(dash_index + 1, space_index), 0, 10);
      pw.letter = std::tolower((line.substr(space_index + 1, colon_space_index))[0]);

      auto actual_pw = trim_copy(line.substr(colon_space_index + 1));

      try
      {
        auto min_has_letter = std::tolower(actual_pw[pw.min_count - 1]) == pw.letter;
        auto max_has_letter = std::towlower(actual_pw[pw.max_count - 1]) == pw.letter;

        auto valid = (min_has_letter && !max_has_letter) || (!min_has_letter && max_has_letter); 

        if (valid)
        {
          std::cout << actual_pw << " : " << pw.min_count << "-" << pw.max_count << " : " << pw.letter << std::endl;
          valid_pws++;
        }
      }
      catch (std::exception e)
      {
        std::cout << e.what() << std::endl;
      }
    }
  }

  std::cout << valid_pws << std::endl;
  return 2;
}