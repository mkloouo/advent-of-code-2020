#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main()
{
  std::ifstream ifile("./day-1-input.txt");

  if (!ifile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  size_t numbers_count = 0; 
  std::vector<unsigned short int> numbers{};

  {
    std::string line;
    while (!ifile.eof())
    {
      std::getline(ifile, line, '\n');

      if (line.empty())
      {
        continue;
      }

      numbers.push_back(static_cast<unsigned short int>(std::stoul(line, 0, 10)));
      ++numbers_count;
    }
  }

  for (size_t i = 0; i < numbers_count; ++i)
  {
    for (size_t j = 0; j < numbers_count; ++j)
    {
      for (size_t k = 0; k < numbers_count; ++k)
      {
        if (i == j || i == k || j == k)
        {
          continue;
        }

        if (numbers[i] + numbers[j] + numbers[k] == 2020)
        {
          std::cout << numbers[i] * numbers[j] * numbers[k] << std::endl;
          return 0;
        }
      }
    }
  }

  std::cout << "No 2020 sum available" << std::endl;
  return 2;
}