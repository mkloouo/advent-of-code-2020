#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <regex>
#include <utility>
#include <numeric>
#include <set>

int main()
{
  std::ifstream ifile("./input.txt");

  if (!ifile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  size_t unique_answers = 0;

  {
    std::string curr_line;
    std::map<char, size_t> answers{};
    size_t people = 0;

    while (!ifile.eof())
    {
      std::getline(ifile, curr_line, '\n');

      if (curr_line.empty())
      {
        size_t init = 0;
        for (const auto &p : answers)
        {
          init += (p.second == people ? 1 : 0);
        }

        unique_answers += init;

        people = 0;
        answers.clear();
        continue;
      }

      ++people;
      for (size_t i = 0; i < curr_line.size(); ++i)
      {
        answers[curr_line[i]]++;
      }
    }
  }

  std::cout << unique_answers << std::endl;

  return 0;
}