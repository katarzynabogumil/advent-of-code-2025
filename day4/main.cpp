#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

bool checkLocation(const vector<vector<char>> &input, int xMax, int yMax, int x, int y)
{
  int rolls = 0;

  for (int i = max(y - 1, 0); i <= min(yMax - 1, y + 1); ++i)
  {
    for (int j = max(x - 1, 0); j <= min(xMax - 1, x + 1); ++j)
    {
      if ((i != y || j != x) && input[i][j] == '@')
      {
        ++rolls;
      }
    }
  }

  return rolls < 4;
}

long long part1(const vector<vector<char>> &input)
{
  long long sum = 0;
  int xMax = static_cast<int>(input[0].size());
  int yMax = static_cast<int>(input.size());

  for (size_t i = 0; i < input.size(); ++i)
  {
    for (size_t j = 0; j < input[i].size(); ++j)
    {
      if (input[i][j] == '@' && checkLocation(input, xMax, yMax, j, i))
      {
        ++sum;
      }
    }
  }

  return sum;
}

long long part2(vector<vector<char>> &input)
{
  long long sum = 0;
  int xMax = static_cast<int>(input[0].size());
  int yMax = static_cast<int>(input.size());
  int removed = 1;

  while (removed > 0)
  {
    removed = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
      for (size_t j = 0; j < input[i].size(); ++j)
      {
        if (input[i][j] == '@' && checkLocation(input, xMax, yMax, j, i))
        {
          input[i][j] = '.';
          ++removed;
        }
      }
    }
    sum += removed;
  }

  return sum;
}

vector<char> parseLine(const string &line)
{
  istringstream iss(line);
  vector<char> result;

  for (char val : line)
  {
    result.push_back(val);
  }

  return result;
}

vector<vector<char>> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<vector<char>> result;
  string line;

  while (getline(file, line))
  {
    if (!line.empty())
    {
      result.push_back(parseLine(line));
    }
  }

  return result;
}

int main()
{
  try
  {
    auto start = chrono::high_resolution_clock::now();

    auto parsedInput = parseFile("input.txt");

    auto resPart1 = part1(parsedInput);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    auto resPart2 = part2(parsedInput);
    auto timePart2 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 2: {} in {}μs\n", resPart2, timePart2.count());

    return 0;
  }
  catch (const exception &e)
  {
    cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
