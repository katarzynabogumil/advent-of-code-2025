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

bool validateIDPart2(const long long &id)
{
  int size = 1;

  while (true)
  {
    if (pow(10, size) > id)
    {
      for (int i = 1; i <= size / 2; ++i)
      {
        for (int j = 0; j + 2 * i <= size; j += i)
        {
          long long lowerLimit = pow(10, j);
          long long upperLimit = pow(10, (i * 2 + j));
          long long divider = pow(10, i + j);
          long long first = id % upperLimit / divider;
          long long second = id % divider / lowerLimit;

          if (first != second)
          {
            break;
          }

          if (j + 2 * i == size)
          {
            return false;
          }
        }
      }

      return true;
    }

    size += 1;
  }
}

long long part2(const vector<vector<long long>> &input)
{
  long long sum = 0;

  for (const auto &range : input)
  {
    for (long long i = range[0]; i <= range[1]; ++i)
    {
      if (!validateIDPart2(i))
      {
        sum += i;
      }
    }
  }

  return sum;
}

bool validateIDPart1(const long long &id)
{
  int i = 2;
  while (true)
  {
    if (pow(10, i) > id)
    {
      if (pow(10, i - 1) > id)
      {
        return true;
      }

      long long divider = pow(10, i / 2);
      auto first = id / divider;
      auto second = id % divider;

      if (first == second)
      {
        return false;
      }
      return true;
    }

    i += 2;
  }
}

long long part1(const vector<vector<long long>> &input)
{
  long long sum = 0;

  for (const auto &range : input)
  {
    for (auto i = range[0]; i <= range[1]; ++i)
    {
      if (!validateIDPart1(i))
      {
        sum += i;
      }
    }
  }

  return sum;
}

vector<string> split(const string &s, char seperator)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);

  while (getline(tokenStream, token, seperator))
  {
    if (!token.empty())
    {
      tokens.push_back(token);
    }
  }
  return tokens;
}

vector<vector<long long>> parseLine(const string &line)
{
  vector<string> ranges = split(line, ',');
  vector<vector<long long>> result;

  for (const string &range : ranges)
  {
    vector<string> dash_tokens = split(range, '-');

    if (dash_tokens.size() == 2)
    {
      long long start = stol(dash_tokens[0]);
      long long end = stol(dash_tokens[1]);

      result.push_back({start, end});
    }
  }

  return result;
}

vector<vector<long long>> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<vector<long long>> result;
  string line;

  if (getline(file, line) && !line.empty())
  {
    return parseLine(line);
  }

  return result;
}

int main()
{
  try
  {
    auto start = chrono::high_resolution_clock::now();

    auto parsedInput = parseFile("input.txt");

    long long resPart1 = part1(parsedInput);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    long long resPart2 = part2(parsedInput);
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
