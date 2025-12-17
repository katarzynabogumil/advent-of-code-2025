#include <cstdlib>
#include <climits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

const int BEST_FITS = 10;

struct Point
{
  size_t x;
  size_t y;
};

string generateKey(Point p)
{
  return format("{},{}", p.x, p.y);
}

struct Shape
{
  size_t idx;
  size_t sizeX;
  size_t sizeY;
  long long assessedSize;
  unordered_map<string, Point> shape;
  string toString() const
  {
    vector<string> keyList;
    for (auto &[key, point] : shape)
    {
      keyList.push_back(key);
    }

    sort(keyList.begin(), keyList.end(), [](string a, string b)
         { return a < b; });

    return accumulate(keyList.begin(), keyList.end(), string(""));
  }
};

bool operator==(const Shape &s1, const Shape &s2)
{
  return s1.toString() == s2.toString();
}

struct MergedShapes
{
  vector<size_t> indice;
  vector<Shape> shapes;
};

void printShape(Shape &shape)
{
  cout << format("\n");
  for (size_t y = 0; y < shape.sizeY; ++y)
  {
    for (size_t x = 0; x < shape.sizeX; ++x)
    {
      if (shape.shape.find(generateKey(Point{x, y})) != shape.shape.end())
      {
        cout << format("#");
      }
      else
      {
        cout << format(".");
      }
    }
    cout << format("\n");
  }
  cout << format("\n");
}

Shape flip(const Shape &s)
{
  unordered_map<string, Point> newShape;

  for (auto &[key, point] : s.shape)
  {
    Point p;
    p.x = s.sizeX - point.x - 1;
    p.y = point.y;

    newShape[generateKey(p)] = p;
  }

  return Shape{s.idx, s.sizeX, s.sizeY, s.assessedSize, newShape};
}

Shape rotate(const Shape &initialShape, int counter)
{
  Shape shape = initialShape;

  for (auto i = 0; i < counter; ++i)
  {
    unordered_map<string, Point> newShape;

    for (auto &[key, point] : shape.shape)
    {
      Point p;
      p.x = shape.sizeY - point.y - 1;
      p.y = point.x;
      newShape[generateKey(p)] = p;
    }

    shape.shape = newShape;
    auto tempSize = shape.sizeX;
    shape.sizeX = shape.sizeY;
    shape.sizeY = tempSize;
  }

  return shape;
}

long long assessSize(unordered_map<string, Point> shape, size_t sizeX, size_t sizeY)
{
  // idea to check how har all points are from the center,
  // but priotitised long, narrow shapes
  long long sum = 0;
  // for (auto &[key, point] : shape)
  // {
  //   long long x = point.x - ((sizeX - 1) / 2) % 1;
  //   long long y = point.y - ((sizeY - 1) / 2) % 1;
  //   sum += abs(x) * abs(y);
  // }
  return sizeX * sizeY * max(sizeX, sizeY) + sum;
}

bool checkSize(const Shape &s, size_t x, size_t y)
{
  for (auto &[key, point] : s.shape)
  {
    if (point.x >= x || point.y >= y)
    {
      goto nextloop;
    }
  }

  return true;

nextloop:;

  for (auto &[key, point] : s.shape)
  {
    if (point.x >= y || point.y >= x)
    {
      return false;
    }
  }

  return true;
}

vector<Shape> merge(const Shape &s1, Shape &s2)
{
  vector<Shape> posibleShapes;
  for (size_t vx = 0; vx <= max(s1.sizeX, s2.sizeX) + 1; ++vx)
  {
    for (size_t vy = 0; vy <= max(s1.sizeY, s2.sizeY) + 1; ++vy)
    {
      size_t maxX = 0;
      size_t maxY = 0;

      auto newShape = s1.shape;
      for (auto &[key2, point2] : s2.shape)
      {
        Point newPoint = Point{point2.x + vx, point2.y + vy};
        if (newShape.find(generateKey(newPoint)) != newShape.end())
        {
          goto next;
        }

        if (newPoint.x > maxX)
        {
          maxX = newPoint.x;
        }
        if (newPoint.y > maxY)
        {
          maxY = newPoint.y;
        }

        newShape[generateKey(newPoint)] = newPoint;
      }

      posibleShapes.push_back(Shape{
          0,
          maxX + 1,
          maxY + 1,
          assessSize(newShape, maxX + 1, maxY + 1),
          newShape,
      });

    next:;
    }
  }

  sort(posibleShapes.begin(), posibleShapes.end(), [](Shape a, Shape b)
       { return a.assessedSize < b.assessedSize; });

  // cout << format("posibleShapes \n");
  // for (auto &shape : posibleShapes)
  // {
  //   cout << format("shape sizeX: {}, sizeY: {}, idx: {}, size: {}\n", shape.sizeX, shape.sizeY, shape.idx, shape.assessedSize);
  //   for (auto &[key, point] : shape.shape)
  //   {
  //     cout << format("[{},{}] ", point.x, point.y);
  //   }
  //   cout << format("\n");
  //   printShape(shape);
  // }

  return posibleShapes;
}

vector<Shape> getBestMergedShapes(const Shape &shape1, Shape &shape2)
{
  vector<Shape> newShapes;

  auto merged = merge(shape1, shape2);
  newShapes.insert(newShapes.end(), merged.begin(), merged.end());

  auto shape2Flipped = flip(shape2);
  auto mergedWithFlipped = merge(shape1, shape2Flipped);
  newShapes.insert(newShapes.end(), mergedWithFlipped.begin(), mergedWithFlipped.end());

  for (auto i = 0; i < 4; ++i)
  {
    auto shape2Rotated = rotate(shape2, i);
    auto mergedWithRotated = merge(shape1, shape2Rotated);
    newShapes.insert(newShapes.end(), mergedWithRotated.begin(), mergedWithRotated.end());

    auto shape2FlippedRotated = rotate(shape2Flipped, i);
    auto mergedWithFlippedRotated = merge(shape1, shape2FlippedRotated);
    newShapes.insert(newShapes.end(), mergedWithFlippedRotated.begin(), mergedWithFlippedRotated.end());
  }

  sort(newShapes.begin(), newShapes.end(), [](Shape a, Shape b)
       { return a.assessedSize < b.assessedSize; });
  newShapes.erase(unique(newShapes.begin(), newShapes.end()), newShapes.end());

  vector<Shape> result(newShapes.begin(), newShapes.begin() + BEST_FITS);
  return result;
}

bool checkShapes(vector<Shape> &shapes, int sizeX, int sizeY)
{
  vector<Shape> newShapes = shapes;

  while (newShapes.size() > 1)
  {
    for (size_t i = 0; i < newShapes.size(); ++i)
    {
      vector<Shape> bestFits;
      for (size_t j = 0; j < newShapes.size(); ++j)
      {
        if (i == j)
        {
          continue;
        }

        auto shape1 = newShapes[i];
        auto shape2 = newShapes[j];

        auto newShapes = getBestMergedShapes(shape1, shape2);
        for (auto &s : newShapes)
        {
          s.idx = j;
        }

        bestFits.insert(bestFits.end(), newShapes.begin(), newShapes.end());
      }

      sort(bestFits.begin(), bestFits.end(), [](Shape a, Shape b)
           { return a.assessedSize < b.assessedSize; });

      vector<Shape> firstBestFits(bestFits.begin(), bestFits.begin() + BEST_FITS);

      for (auto &fit : firstBestFits)
      {
        if (checkSize(fit, sizeX, sizeY))
        {
          auto nextShapes = newShapes;
          nextShapes.erase(nextShapes.begin() + fit.idx);
          nextShapes.erase(nextShapes.begin() + i);
          nextShapes.push_back(fit);

          if (checkShapes(nextShapes, sizeX, sizeY))
          {
            return true;
          }
        }
      }

      return false;
    }
  }

  return checkSize(newShapes[0], sizeX, sizeY);
}

bool checkRow(unordered_map<size_t, vector<MergedShapes>> &bestBasicFitMap, vector<Shape> &shapes, vector<int> &regionsSize, vector<int> regionPresents)
{
  auto sizeX = regionsSize[0];
  auto sizeY = regionsSize[1];

  vector<Shape> newShapes;

  // For all basic shapes in the row get their best fits that fit in the frame,
  // merge them to new shapes
  auto presentsLeft = reduce(regionPresents.begin(), regionPresents.end());

  while (presentsLeft > 0)
  {
    for (size_t i = 0; i < regionPresents.size(); ++i)
    {
      if (regionPresents[i] == 0)
      {
        continue;
      }

      if (presentsLeft == 1)
      {
        newShapes.push_back(shapes[i]);
        --regionPresents[i];
        --presentsLeft;
      }

      auto bestFits = bestBasicFitMap[i];
      for (size_t j = 0; j < bestFits.size(); ++j)
      {
        auto fit = bestFits[j];
        if (regionPresents[fit.indice[0]] < 1 || regionPresents[fit.indice[1]] < 1 ||
            (fit.indice[0] == fit.indice[1] && regionPresents[fit.indice[0]] < 2))
        {
          continue;
        }

        for (size_t k = 0; k < fit.shapes.size(); ++k)
        {
          if (checkSize(fit.shapes[k], sizeX, sizeY))
          {
            newShapes.push_back(fit.shapes[k]);
            --regionPresents[fit.indice[0]];
            --regionPresents[fit.indice[1]];
            presentsLeft -= 2;
            goto end1;
          }
        }

        return false;
      }
    }
  end1:;
  }

  return checkShapes(newShapes, sizeX, sizeY);
}

unordered_map<size_t, vector<MergedShapes>> createBestBasicFitMap(vector<Shape> &shapes)
{
  unordered_map<size_t, vector<MergedShapes>> bestBasicFitMap;

  for (auto &shape1 : shapes)
  {
    vector<MergedShapes> bestBasicFits;
    for (auto &shape2 : shapes)
    {
      auto newShapes = getBestMergedShapes(shape1, shape2);

      vector<size_t> indices;
      indices.push_back(shape1.idx);
      indices.push_back(shape2.idx);

      MergedShapes basicMergedShapes = {
          indices,
          newShapes,
      };

      bestBasicFits.push_back(basicMergedShapes);
    }

    sort(bestBasicFits.begin(), bestBasicFits.end(), [](MergedShapes a, MergedShapes b)
         { return a.shapes[0].assessedSize < b.shapes[0].assessedSize; });

    bestBasicFitMap[shape1.idx] = bestBasicFits;
  }

  return bestBasicFitMap;
}

long long part1(vector<Shape> &shapes, vector<vector<int>> &regionsSizes, vector<vector<int>> &regionsPresents)
{
  unordered_map<size_t, vector<MergedShapes>> bestBasicFitMap = createBestBasicFitMap(shapes);

  long long sum = 0;
  for (size_t i = 0; i < regionsSizes.size(); ++i)
  {
    cout << format("row: {}\n", i);
    if (checkRow(bestBasicFitMap, shapes, regionsSizes[i], regionsPresents[i]))
    {
      cout << format("shape check correct for row: {}\n", i);
      ++sum;
    }
  }

  return sum;
}

long long part1AreaCheck(vector<Shape> &shapes, vector<vector<int>> &regionsSizes, vector<vector<int>> &regionsPresents)
{
  long long sum = 0;

  for (size_t i = 0; i < regionsSizes.size(); ++i)
  {
    long long presentsArea = 0;
    for (size_t j = 0; j < regionsPresents[i].size(); ++j)
    {
      auto presentsNum = regionsPresents[i][j];
      presentsArea += presentsNum * shapes[j].shape.size();
    }

    auto regionArea = regionsSizes[i][0] * regionsSizes[i][1];
    if (regionArea >= presentsArea)
    {
      cout << format("area check correct for row: {}\n", i);
      ++sum;
    }
  }

  return sum;
}

vector<int> parseNums(const string &line)
{
  istringstream iss(line);
  vector<int> result;
  int num;

  while (iss >> num)
  {
    result.push_back(num);
  }

  return result;
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

void parseFile(string &filename, vector<Shape> &shapes, vector<vector<int>> &regionsSizes, vector<vector<int>> &regionsPresents)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  vector<string> lines;
  string line;

  while (getline(file, line))
  {
    lines.push_back(line);
  }

  size_t counter = 0;
  for (size_t i = 1; i < 30; i += 5)
  {
    vector<string> rows;
    rows.push_back(lines[i]);
    rows.push_back(lines[i + 1]);
    rows.push_back(lines[i + 2]);

    unordered_map<string, Point> shape;
    for (size_t y = 0; y < rows.size(); ++y)
    {
      for (size_t x = 0; x < rows[y].size(); ++x)
      {
        if (rows[y][x] == '#')
        {
          Point p = Point{x, y};
          shape[generateKey(p)] = p;
        }
      }
    }

    shapes.push_back(Shape{
        counter++,
        rows[0].size(),
        rows.size(),
        assessSize(shape, rows[0].size(), rows.size()),
        shape,
    });
  }

  for (size_t i = 30; i < lines.size(); ++i)
  {
    if (!lines[i].empty())
    {
      auto parts = split(lines[i], ':');

      auto size = split(parts[0], 'x');
      vector<int> regionSize;
      regionSize.push_back(stoi(size[0]));
      regionSize.push_back(stoi(size[1]));
      regionsSizes.push_back(regionSize);

      regionsPresents.push_back(parseNums(parts[1]));
    }
  }

  return;
}

int main()
{
  try
  {
    auto start = chrono::high_resolution_clock::now();

    vector<Shape> shapes;
    vector<vector<int>> regionsSizes;
    vector<vector<int>> regionsPresents;
    string fileName = "input.txt";
    parseFile(fileName, shapes, regionsSizes, regionsPresents);

    auto resPart1AreaCheck = part1AreaCheck(shapes, regionsSizes, regionsPresents);
    auto timePart1AreaCheck = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1 area check: {} in {}μs\n", resPart1AreaCheck, timePart1AreaCheck.count());

    // Works for test-input, takes too long for real one.
    // Tried limiting best fit number to consider, but then didn't find all correct soltuions.
    auto resPart1 = part1(shapes, regionsSizes, regionsPresents);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    return 0;
  }
  catch (const exception &e)
  {
    cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
