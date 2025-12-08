#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <cmath>

struct Box
{
  int x;
  int y;
  int z;
};

using namespace std;

long long calculateDistance(Box box1, Box box2)
{
  long long xDiff = box1.x - box2.x;
  long long yDiff = box1.y - box2.y;
  long long zDiff = box1.z - box2.z;

  return sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
}

void createDistanceMap(vector<Box> &input, vector<vector<long long>> &distanceMap)
{
  for (size_t i = 0; i < input.size(); ++i)
  {
    vector<long long> row;
    for (size_t j = 0; j < input.size(); ++j)
    {
      if (i == j)
      {
        row.push_back(0);
      }
      else
      {
        row.push_back(calculateDistance(input[i], input[j]));
      }
    }
    distanceMap.push_back(row);
  }
}

void connectBoxesP2(vector<set<size_t>> &circuits, vector<vector<long long>> &distanceMap, size_t index1, size_t index2)
{
  bool wasAdded = false;
  bool foundToErase = false;
  size_t circuitWhereAdded;
  size_t circuitToErase;

  for (size_t i = 0; i < circuits.size(); ++i)
  {
    if (circuits[i].contains(index1) || circuits[i].contains(index2))
    {
      if (wasAdded)
      {
        for (auto &indexToMove : circuits[i])
        {
          circuits[circuitWhereAdded].insert(indexToMove);
          for (auto &val : circuits[circuitWhereAdded])
          {
            distanceMap[indexToMove][val] = 0;
            distanceMap[val][indexToMove] = 0;
          }
        }
        foundToErase = true;
        circuitToErase = i;
        break;
      }
      else
      {
        wasAdded = true;
        circuitWhereAdded = i;

        if (circuits[i].contains(index1))
        {
          circuits[i].insert(index2);
          for (auto &val : circuits[i])
          {
            distanceMap[index2][val] = 0;
            distanceMap[val][index2] = 0;
          }
        }
        else if (circuits[i].contains(index2))
        {
          circuits[i].insert(index1);
          for (auto &val : circuits[i])
          {
            distanceMap[index1][val] = 0;
            distanceMap[val][index1] = 0;
          }
        }
      }
    }
  }

  if (foundToErase)
  {
    circuits.erase(circuits.begin() + circuitToErase);
  }

  if (!wasAdded)
  {
    set<size_t> newCircuit;
    newCircuit.insert(index1);
    newCircuit.insert(index2);
    circuits.push_back(newCircuit);
    distanceMap[index1][index2] = 0;
    distanceMap[index2][index1] = 0;
  }
}

void findClosestAndConnectP2(vector<Box> &input, vector<vector<long long>> &distanceMap, vector<set<size_t>> &circuits,
                             Box &lastBox1, Box &lastBox2)
{
  long long minDistance = LLONG_MAX;
  size_t boxIndex1 = 0;
  size_t boxIndex2 = 0;

  for (size_t i = 0; i < input.size(); ++i)
  {
    for (size_t j = i + 1; j < input.size(); ++j)
    {
      auto distance = distanceMap[i][j];
      if (distance <= minDistance && distance != 0)
      {
        minDistance = distance;
        boxIndex1 = i;
        boxIndex2 = j;
      }
    }
  }

  connectBoxesP2(circuits, distanceMap, boxIndex1, boxIndex2);

  lastBox1 = input[boxIndex1];
  lastBox2 = input[boxIndex2];
}

size_t part2(vector<Box> &input)
{
  vector<vector<long long>> distanceMap;
  createDistanceMap(input, distanceMap);

  vector<set<size_t>> circuits;
  Box lastBox1;
  Box lastBox2;

  while (circuits.size() == 0 || circuits[0].size() < input.size())
  {
    findClosestAndConnectP2(input, distanceMap, circuits, lastBox1, lastBox2);
  }

  return lastBox1.x * lastBox2.x;
}

void connectBoxesP1(vector<set<size_t>> &circuits, size_t index1, size_t index2)
{
  bool wasAdded = false;
  bool foundToErase = false;
  size_t circuitWhereAdded;
  size_t circuitToErase;

  for (size_t i = 0; i < circuits.size(); ++i)
  {
    if (circuits[i].contains(index1) || circuits[i].contains(index2))
    {
      if (wasAdded)
      {
        for (auto &indexToMove : circuits[i])
        {
          circuits[circuitWhereAdded].insert(indexToMove);
        }
        foundToErase = true;
        circuitToErase = i;
        break;
      }
      else
      {
        wasAdded = true;
        circuitWhereAdded = i;

        if (circuits[i].contains(index1))
        {
          circuits[i].insert(index2);
        }
        else if (circuits[i].contains(index2))
        {
          circuits[i].insert(index1);
        }
      }
    }
  }

  if (foundToErase)
  {
    circuits.erase(circuits.begin() + circuitToErase);
  }

  if (!wasAdded)
  {
    set<size_t> newCircuit;
    newCircuit.insert(index1);
    newCircuit.insert(index2);
    circuits.push_back(newCircuit);
  }
}

void findClosestAndConnectP1(vector<Box> &input, vector<vector<long long>> &distanceMap, vector<set<size_t>> &circuits)
{
  long long minDistance = LLONG_MAX;
  size_t boxIndex1 = 0;
  size_t boxIndex2 = 0;

  for (size_t i = 0; i < input.size(); ++i)
  {
    for (size_t j = i + 1; j < input.size(); ++j)
    {
      auto distance = distanceMap[i][j];
      if (distance <= minDistance && distance != 0)
      {
        minDistance = distance;
        boxIndex1 = i;
        boxIndex2 = j;
      }
    }
  }

  distanceMap[boxIndex1][boxIndex2] = 0;
  distanceMap[boxIndex2][boxIndex1] = 0;

  for (const auto &circuit : circuits)
  {
    if (circuit.contains(boxIndex1) && circuit.contains(boxIndex2))
    {
      return;
    }
  }

  connectBoxesP1(circuits, boxIndex1, boxIndex2);
}

size_t part1(vector<Box> &input, int pairs)
{
  vector<vector<long long>> distanceMap;
  createDistanceMap(input, distanceMap);

  vector<set<size_t>> circuits;
  for (int i = 0; i < pairs; ++i)
  {
    findClosestAndConnectP1(input, distanceMap, circuits);
  }

  sort(circuits.begin(), circuits.end(), [](set<size_t> a, set<size_t> b)
       { return b.size() < a.size(); });

  size_t result = 1;
  for (size_t i = 0; i < 3 && i < circuits.size(); ++i)
  {
    result *= circuits[i].size();
  }
  return result;
}

Box parseLine(const string &line)
{
  vector<int> tokens;
  string token;
  istringstream tokenStream(line);

  while (getline(tokenStream, token, ','))
  {
    if (!token.empty())
    {
      tokens.push_back(stoi(token));
    }
  }

  return Box{tokens[0], tokens[1], tokens[2]};
}

vector<Box> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<Box> result;
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

    auto resPart1 = part1(parsedInput, 1000);
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
