#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

using namespace std;

vector<vector<long long>> mergeRanges(const vector<vector<long long>> &ranges)
{
  vector<vector<long long>> result;
  set<long long> includedRangesIndice;

  for (size_t i = 0; i < ranges.size(); ++i)
  {
    if (includedRangesIndice.contains(i))
    {
      continue;
    }

    includedRangesIndice.insert(i);
    auto start = ranges[i][0];
    auto end = ranges[i][1];
    vector<long long> range;

    for (size_t j = 0; j < ranges.size(); ++j)
    {
      if (includedRangesIndice.contains(j))
      {
        continue;
      }

      auto otherStart = ranges[j][0];
      auto otherEnd = ranges[j][1];

      if (start <= otherStart && end >= otherEnd)
      {
        range.push_back(start);
        range.push_back(end);
        includedRangesIndice.insert(j);
        break;
      }

      if (start > otherStart && otherEnd >= start - 1 && end >= otherEnd)
      {
        range.push_back(otherStart);
        range.push_back(end);
        includedRangesIndice.insert(j);
        break;
      }

      if (start <= otherStart && end >= otherStart - 1 && end < otherEnd)
      {
        range.push_back(start);
        range.push_back(otherEnd);
        includedRangesIndice.insert(j);
        break;
      }

      if (start > otherStart && end < otherEnd)
      {
        range.push_back(otherStart);
        range.push_back(otherEnd);
        includedRangesIndice.insert(j);
        break;
      }
    }

    if (range.empty())
    {
      range.push_back(start);
      range.push_back(end);
    }
    result.push_back(range);
  }

  return result;
}

long long part2(const vector<vector<long long>> &ranges)
{
  long long result = 0;
  vector<vector<long long>> mergedRanges = ranges;
  size_t previousLength = 0;
  auto length = mergedRanges.size();

  while (previousLength != length)
  {
    mergedRanges = mergeRanges(mergedRanges);
    previousLength = length;
    length = mergedRanges.size();
  }

  for (const auto &range : mergedRanges)
  {
    auto start = range[0];
    auto end = range[1];
    result += end - start + 1;
  }
  return result;
}

long long part1(const vector<vector<long long>> &ranges, vector<long long> &ingredients)
{
  long long counter = 0;
  for (const auto &val : ingredients)
  {
    bool isFresh = false;
    for (const auto &range : ranges)
    {
      auto start = range[0];
      auto end = range[1];
      if (val >= start && val <= end)
      {
        isFresh = true;
        break;
      }
    }

    if (isFresh)
    {
      ++counter;
    }
  }

  return counter;
}

vector<long long> parseRange(const string &line)
{
  istringstream iss(line);
  vector<long long> result;
  string token;

  while (getline(iss, token, '-'))
  {
    if (!token.empty())
    {
      result.push_back(stoll(token));
    }
  }

  return result;
}

void parseFile(string &filename, vector<vector<long long>> &ranges, vector<long long> &ingredients)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  bool isParsingIngredients = false;
  string line;

  while (getline(file, line))
  {
    if (line.empty())
    {
      isParsingIngredients = true;
    }
    else if (isParsingIngredients)
    {
      ingredients.push_back(stoll(line));
    }
    else
    {
      ranges.push_back(parseRange(line));
    }
  }

  return;
}

int main()
{
  try
  {
    auto start = chrono::high_resolution_clock::now();

    vector<vector<long long>> ranges;
    vector<long long> ingredients;
    string filename = "input.txt";

    parseFile(filename, ranges, ingredients);

    auto resPart1 = part1(ranges, ingredients);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    auto resPart2 = part2(ranges);
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
