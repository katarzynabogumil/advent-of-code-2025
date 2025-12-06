#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

long long solve(const vector<vector<long long>> &numbers, vector<string> &operators)
{
  long long sum = 0;
  for (size_t col = 0; col < numbers.size(); ++col)
  {
    long long columnSum = numbers[col][0];
    auto op = operators[col];

    for (size_t i = 1; i < numbers[col].size(); ++i)
    {
      auto num = numbers[col][i];
      if (op == "+")
      {
        columnSum += num;
      }
      else if (op == "-")
      {
        columnSum -= num;
      }
      else if (op == "*")
      {
        columnSum *= num;
      }
      else if (op == "/")
      {
        columnSum /= num;
      }
    }

    sum += columnSum;
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

void parseLinesPart2(vector<string> &lines, vector<vector<long long>> &numbers)
{
  auto rows = lines.size() - 1; // skip operators, already parsed
  size_t columnIndex = 0;
  for (size_t inputCol = 0; inputCol < lines[0].size(); ++inputCol)
  {
    bool isEmpty = true;
    for (size_t inputRow = 0; inputRow < rows; ++inputRow)
    {
      if (lines[inputRow][inputCol] != ' ')
      {
        isEmpty = false;
        break;
      }
    }

    if (isEmpty)
    {
      reverse(numbers[columnIndex].begin(), numbers[columnIndex].end());
      columnIndex++;
    }
    else
    {
      string newNumber;
      for (size_t inputRow = 0; inputRow < rows; ++inputRow)
      {
        newNumber += lines[inputRow][inputCol];
      }

      if (numbers.size() <= columnIndex)
      {
        vector<long long> columnNumbers;
        columnNumbers.push_back(stoll(newNumber));
        numbers.push_back(columnNumbers);
      }
      else
      {
        numbers[columnIndex].push_back(stoll(newNumber));
      }
    }
  }

  return;
}

void parseLinesPart1(vector<string> &lines, vector<vector<long long>> &numbers, vector<string> &operators)
{
  for (size_t i = 0; i < lines.size(); ++i)
  {
    if (i == lines.size() - 1)
    {
      operators = split(lines[i], ' ');
    }
    else
    {
      vector<string> line = split(lines[i], ' ');

      if (i == 0)
      {
        for (size_t col = 0; col < line.size(); ++col)
        {
          vector<long long> columnNumbers;
          numbers.push_back(columnNumbers);
        }
      }

      for (size_t col = 0; col < line.size(); ++col)
      {
        auto num = stoll(line[col]);
        numbers[col].push_back(num);
      }
    }
  }

  return;
}

vector<string> parseFile(string filename)
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

    vector<string> lines = parseFile("input.txt");
    vector<string> operators;

    vector<vector<long long>> numbersPart1;
    parseLinesPart1(lines, numbersPart1, operators);
    auto resPart1 = solve(numbersPart1, operators);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    vector<vector<long long>> numbersPart2;
    parseLinesPart2(lines, numbersPart2);
    auto resPart2 = solve(numbersPart2, operators);
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
