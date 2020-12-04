#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>
#include <numeric>

struct Player
{
  size_t x;
  size_t y;

  size_t right;
  size_t down;

  bool finished;

  size_t trees;
};

int main()
{

  std::string line1 = "asubdjabskd", line2 = "asubdjaaskd";

  // std::getline(std::cin, line1, '\n');
  // std::getline(std::cin, line2, '\n');

  bool are_they_the_same = line1.size() == line2.size();

  if (!are_they_the_same)
  {
    std::cout << "line1 is less or greater in length than line2" << std::endl;
    return 0;
  }

  size_t index = 0;
  while (index < line1.size()) // 12
  {
    if (line1[index] != line2[index])
    {
      std::cout << (index + 1) << " symbol of line1 does not equal to the same symbol of line2" << std::endl;
      return 0;
    }

    index = index + 1;
  }

  std::cout << "line1 is equal to line2" << std::endl;

  return 0;

  std::ifstream ifile("./input.txt");

  if (!ifile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  std::vector<std::vector<bool>> toboggan_map{};
  size_t rows = 0;

  {

    std::string line;
    while (!ifile.eof())
    {
      std::getline(ifile, line, '\n');

      if (line.empty())
      {
        continue;
      }

      toboggan_map.push_back(std::vector<bool>{});
      // for (auto it = line.begin(); it != line.end(); ++it)
      for (size_t index = 0; index < line.size(); ++index)
      {
        toboggan_map[rows].push_back(line[index] == '#');
      }
      ++rows;
    }
  }

  std::vector<Player> players{5};
  players[0] = {.right = 1, .down = 1};
  players[1] = {.right = 3, .down = 1};
  players[2] = {.right = 5, .down = 1};
  players[3] = {.right = 7, .down = 1};
  players[4] = {.right = 1, .down = 2};

  size_t columns = toboggan_map[0].size();

  bool players_finished = false;

  while (!players_finished)
  {
    for (auto &player : players)
    {
      if (player.finished)
      {
        continue;
      }

      if (player.y > rows - 1)
      {
        player.finished = true;
        continue;
      }

      player.trees += toboggan_map[player.y % rows][player.x % columns] ? 1 : 0;

      player.x += player.right;
      player.y += player.down;
    }

    players_finished = std::all_of(players.begin(), players.end(), [](const Player &player) {
      return player.finished;
    });
  }

  std::cout << std::accumulate(players.begin(), players.end(), static_cast<unsigned long long>(0), [](unsigned long long total_trees, const Player &player) {
    std::cout << "p trees: " << player.trees << std::endl;

    if (total_trees == 0)
    {
      return static_cast<unsigned long long>(player.trees);
    }

    return total_trees *= static_cast<unsigned long long>(player.trees);
  }) << std::endl;

  return 2;
}