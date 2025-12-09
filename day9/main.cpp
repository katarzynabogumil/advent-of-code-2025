#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

struct Point
{
  int x;
  int y;
};

using namespace std;

long long calculateArea(Point point1, Point point2)
{
  long long side1 = abs(point1.x - point2.x) + 1;
  long long side2 = abs(point1.y - point2.y) + 1;
  return side1 * side2;
}

long long part2(vector<Point> &input)
{
  long long maxArea = 0;

  for (size_t i = 0; i < input.size() - 1; ++i)
  {
    for (size_t j = i + 1; j < input.size(); ++j)
    {
      Point p1 = input[i];
      Point p2 = input[j];

      auto area = calculateArea(p1, p2);
      if (area <= maxArea)
      {
        continue;
      }

      bool isEmptyInside = false;
      bool isCorner1In = false;
      bool isCorner2In = false;

      for (size_t k = 0; k < input.size(); ++k)
      {
        if (k == i || k == j)
        {
          continue;
        }

        Point p3 = input[k];
        Point p4;

        if (k + 1 == input.size())
        {
          p4 = input[0];
        }
        else
        {
          p4 = input[k + 1];
        }

        for (int x = min(p3.x, p4.x); x <= max(p3.x, p4.x); ++x)
        {
          for (int y = min(p3.y, p4.y); y <= max(p3.y, p4.y); ++y)
          {
            if (y > min(p1.y, p2.y) && y < max(p1.y, p2.y) &&
                x > min(p1.x, p2.x) && x < max(p1.x, p2.x))
            {
              isEmptyInside = true;
              goto end;
            }

            if ((x == p1.x && y == p2.y) ||
                (p1.x > p2.x && x > p1.x && y == p2.y) ||
                (p1.x < p2.x && x < p1.x && y == p2.y))
            {
              isCorner1In = true;
            }

            if ((x == p2.x && y == p1.y) ||
                (x == p2.x && p1.y > p2.y && y > p1.y) ||
                (x == p2.x && p1.y < p2.y && y < p1.y))
            {
              isCorner2In = true;
            }
          }
        }
      }
    end:;

      if (isEmptyInside || !isCorner1In || !isCorner2In)
      {
        continue;
      }

      maxArea = area;
    }
  }

  return maxArea;
}

long long part1(const vector<Point> &input)
{
  long long maxArea = 0;

  for (size_t i = 0; i < input.size(); ++i)
  {
    for (size_t j = i + 1; j < input.size(); ++j)
    {
      auto area = calculateArea(input[i], input[j]);
      if (area > maxArea)
      {
        maxArea = area;
      }
    }
  }

  return maxArea;
}

Point parseLine(const string &line)
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

  return Point{tokens[0], tokens[1]};
}

vector<Point> parseFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<Point> result;
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
