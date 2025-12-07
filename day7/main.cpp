#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

using namespace std;

// First try for part 2 - too long
// long long trackBeam(const vector<vector<char>> &input, size_t startRow, size_t startCol)
// {
//   for (size_t row = startRow; row < input.size(); ++row)
//   {
//     if (input[row][startCol] == '^')
//     {
//       return trackBeam(input, row + 1, startCol - 1) + trackBeam(input, row + 1, startCol + 1);
//     }
//   }

//   return 1;
// }

long long part2(const vector<vector<char>> &input)
{
  vector<long long> solutions;
  for (size_t col = 0; col < input[0].size(); ++col)
  {
    solutions.push_back(0);
  }

  for (size_t row = 0; row < input.size(); ++row)
  {
    for (size_t col = 0; col < input[0].size(); ++col)
    {
      auto point = input[row][col];
      if (point == 'S')
      {
        solutions[col] += 1;
      }
      else if (point == '^' && solutions[col] > 0)
      {
        solutions[col - 1] += solutions[col];
        solutions[col + 1] += solutions[col];
        solutions[col] = 0;
      }
    }
  }

  long long sum = 0;
  for (auto &val : solutions)
  {
    sum += val;
  }

  return sum;
}

long long part1(const vector<vector<char>> &input)
{
  set<int> beams;
  long long counter = 0;

  for (size_t row = 0; row < input.size(); ++row)
  {
    for (size_t col = 0; col < input[0].size(); ++col)
    {
      auto point = input[row][col];
      if (point == 'S')
      {
        beams.insert(col);
      }
      else if (point == '^' && beams.contains(col))
      {
        ++counter;
        beams.erase(col);
        beams.insert(col - 1);
        beams.insert(col + 1);
      }
    }
  }

  return counter;
}

vector<char> parseLine(const string &line)
{
  istringstream iss(line);
  vector<char> result;
  char num;

  while (iss >> num)
  {
    result.push_back(num);
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
