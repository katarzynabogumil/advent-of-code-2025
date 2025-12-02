#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

int moveDialP1(int pos, int dir, int steps)
{
  pos += dir * steps;
  pos %= 100;

  if (pos < 0)
  {
    pos += 100;
  }

  return pos;
}

int part1(const vector<vector<int>> &input)
{
  int pos = 50;
  int counter = 0;

  for (const auto &row : input)
  {
    pos = moveDialP1(pos, row[0], row[1]);
    if (pos == 0)
    {
      ++counter;
    }
  }

  return counter;
}

int moveDialP2(int &pos, int &counter, int dir, int steps)
{
  int startPost = pos;
  counter += steps / 100;
  steps %= 100;
  pos += dir * steps;

  if ((pos >= 100 || (pos >= 0 && startPost < 0)) ||
      (pos <= -100 || (pos <= 0 && startPost > 0)))
  {
    ++counter;
  }

  pos += 200;
  pos %= 100;
  return pos;
}

int part2(const vector<vector<int>> &input)
{
  int pos = 50;
  int counter = 0;

  for (const auto &row : input)
  {
    pos = moveDialP2(pos, counter, row[0], row[1]);
  }

  return counter;
}

vector<int> parseLine(const string &line)
{
  if (line.length() < 2)
  {
    throw runtime_error("Line is too short to parse: " + line);
  }
  vector<int> result;
  char direction = line[0];

  if (direction == 'L')
  {
    result.push_back(-1);
  }
  else if (direction == 'R')
  {
    result.push_back(1);
  }
  else
  {
    throw runtime_error("Invalid direction character in line: " + line);
  }

  result.push_back(stoi(line.substr(1)));
  return result;
}

vector<vector<int>> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<vector<int>> result;
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

    int resPart1 = part1(parsedInput);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    int resPart2 = part2(parsedInput);
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
