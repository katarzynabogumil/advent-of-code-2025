#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

string generateKey(string node, bool visited1, bool visited2)
{
  return node + to_string(visited1) + to_string(visited2);
}

long long inspectPath(unordered_map<string, vector<string>> &input, unordered_map<string, long long> &visitedPaths, string node, bool visitedDac, bool visitedFft)
{
  if (node == "out")
  {
    if (visitedDac && visitedFft)
    {
      return 1;
    }
    return 0;
  }

  auto key = generateKey(node, visitedDac, visitedFft);
  if (visitedPaths.find(key) != visitedPaths.end())
  {
    return visitedPaths[key];
  }

  if (node == "dac")
  {
    visitedDac = true;
  }

  if (node == "fft")
  {
    visitedFft = true;
  }

  long long paths = 0;
  for (auto &val : input[node])
  {
    auto newPaths = inspectPath(input, visitedPaths, val, visitedDac, visitedFft);

    auto valKey = generateKey(val, visitedDac, visitedFft);
    if (visitedPaths.find(valKey) == visitedPaths.end())
    {
      visitedPaths[valKey] = newPaths;
    }

    paths += newPaths;
  }

  return paths;
}

long long part2(unordered_map<string, vector<string>> &input)
{
  unordered_map<string, long long> visitedPaths;
  string start = "svr";

  return inspectPath(input, visitedPaths, start, false, false);
}

int part1(unordered_map<string, vector<string>> &input)
{
  string start = "you";
  string end = "out";

  list<string> q;
  auto firstConnections = input[start];
  for (auto &val : firstConnections)
  {
    q.push_back(val);
  }

  int paths = 0;
  while (q.size() > 0)
  {
    auto node = q.front();
    q.pop_front();

    if (node == end)
    {
      ++paths;
    }
    else
    {
      auto connections = input[node];
      for (auto &val : connections)
      {
        q.push_back(val);
      }
    }
  }

  return paths;
}

void parseLine(string &line, string &name, vector<string> &connections)
{
  name = line.substr(0, 3);

  for (size_t i = 5; i < line.size(); i += 4)
  {
    connections.push_back(line.substr(i, 3));
  }

  return;
}

unordered_map<string, vector<string>> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  unordered_map<string, vector<string>> result;
  string line;

  while (getline(file, line))
  {
    if (!line.empty())
    {
      string name;
      vector<string> connections;
      parseLine(line, name, connections);
      result[name] = connections;
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
