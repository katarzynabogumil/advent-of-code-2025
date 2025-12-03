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

long long getMax(string str, int depth, const char digits[])
{
  if (depth == 0)
  {
    return 0;
  }

  vector<int> result;
  for (int i = 9; i >= 0; --i)
  {
    for (size_t j = 0; j <= str.size() - depth; ++j)
    {
      if (str[j] == digits[i])
      {
        return pow(10, depth - 1) * i + getMax(str.substr(j + 1), depth - 1, digits);
      }
    }
  }

  return 0;
}

long long solve(const vector<string> &input, int depth)
{
  const char digits[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  long long sum = 0;

  for (const auto &row : input)
  {
    sum += getMax(row, depth, digits);
  }

  return sum;
}

vector<string> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<string> result;
  string line;

  while (getline(file, line))
  {
    if (!line.empty())
    {
      result.push_back(line);
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

    auto resPart1 = solve(parsedInput, 2);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    auto resPart2 = solve(parsedInput, 12);
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
