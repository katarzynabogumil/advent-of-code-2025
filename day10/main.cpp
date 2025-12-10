#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <unordered_map>

using namespace std;

bool validateDiagram(const vector<int> &joltage, vector<int> &diagram)
{
  for (size_t i = 0; i < joltage.size(); ++i)
  {
    if (diagram[i] != joltage[i])
    {
      return false;
    }
  }

  return true;
}

bool validateDiagramOverflow(const vector<int> &joltage, vector<int> &diagram)
{
  for (size_t i = 0; i < joltage.size(); ++i)
  {
    if (diagram[i] > joltage[i])
    {
      return false;
    }
  }

  return true;
}

int findIndexMissingInFurtherButtons(const vector<int> &joltage, vector<int> &diagram, vector<vector<int>> &buttons, size_t index)
{
  set<int> furtherButtons;
  for (size_t i = index; i < buttons.size(); ++i)
  {
    for (auto &val : buttons[i])
    {
      furtherButtons.insert(val);
    }
  }

  for (size_t i = 0; i < joltage.size(); ++i)
  {
    if (diagram[i] < joltage[i])
    {
      if (!furtherButtons.contains(i))
      {
        return i;
      }
    }
  }

  return -1;
}

long inspectButtonP2(vector<vector<int>> &buttons, vector<int> &joltage, vector<int> diagram, size_t index, vector<int> pushedButtons)
{
  if (index == buttons.size())
  {
    if (validateDiagram(joltage, diagram))
    {
      long pushes = 0;
      for (auto &val : pushedButtons)
      {
        pushes += val;
      }
      return pushes;
    }
    else
    {
      return LONG_MAX;
    }
  }

  long min = LONG_MAX;
  while (validateDiagramOverflow(joltage, diagram))
  {
    auto missingIndex = findIndexMissingInFurtherButtons(joltage, diagram, buttons, index + 1);
    if (missingIndex != -1)
    {
      auto diff = joltage[missingIndex] - diagram[missingIndex];
      pushedButtons[index] += diff;
      for (auto val : buttons[index])
      {
        diagram[val] += diff;
      }
    }

    auto newSolution = inspectButtonP2(buttons, joltage, diagram, index + 1, pushedButtons);
    if (newSolution < min)
    {
      min = newSolution;
    }

    ++pushedButtons[index];
    for (auto val : buttons[index])
    {
      ++diagram[val];
    }
  }

  return min;
}

struct WeightedJoltage
{
  size_t index;
  int weight;
  int joltage;
};

vector<vector<int>> sortButtons(vector<vector<int>> buttons, vector<int> &joltage)
{
  vector<set<int>> buttonSets;
  for (size_t i = 0; i < buttons.size(); ++i)
  {
    set<int> s(buttons[i].begin(), buttons[i].end());
    buttonSets.push_back(s);
  }

  vector<WeightedJoltage> joltageWeights;
  for (size_t i = 0; i < joltage.size(); ++i)
  {
    int counter = 0;
    for (size_t j = 0; j < buttonSets.size(); ++j)
    {
      if (buttonSets[j].contains(i))
      {
        ++counter;
      }
    }
    joltageWeights.push_back(WeightedJoltage{i, counter, joltage[i]});
  }

  sort(joltageWeights.begin(), joltageWeights.end(), [](const WeightedJoltage &a, const WeightedJoltage &b)
       { 
        if (a.weight == b.weight) {
          return a.joltage < b.joltage;
        }
        return a.weight < b.weight; });

  sort(buttons.begin(), buttons.end(), [joltageWeights](const vector<int> &a, const vector<int> &b)
       {
    set<int> sA(a.begin(), a.end());
    set<int> sB(b.begin(), b.end());

    for (auto &val : joltageWeights)
    {
      if (sA.contains(val.index) && sB.contains(val.index))
      {
        return a.size() > b.size();
      } else if (sA.contains(val.index))
      {
        return true;
      }
      else if (sB.contains(val.index))
      {
        return false;
      }
    }

      return a.size() > b.size(); });

  return buttons;
}

long long part2(const vector<vector<int>> &joltageRows, vector<vector<vector<int>>> &buttonRows)
{
  long long sum = 0;

  for (size_t i = 0; i < joltageRows.size(); ++i)
  {
    auto joltage = joltageRows[i];
    vector<int> diagram;
    for (size_t j = 0; j < joltage.size(); ++j)
    {
      diagram.push_back(0);
    }

    size_t index = 0;
    auto buttons = sortButtons(buttonRows[i], joltage);

    vector<int> pushedButtons;
    for (size_t j = 0; j < buttons.size(); ++j)
    {
      pushedButtons.push_back(0);
    }

    sum += inspectButtonP2(buttons, joltage, diagram, index, pushedButtons);
  }

  return sum;
}

size_t inspectButton(vector<vector<int>> &buttons, vector<bool> &instruction, vector<bool> diagram, size_t index, set<int> pushedButtons)
{
  if (index == buttons.size())
  {
    bool isValid = true;
    for (size_t i = 0; i < instruction.size(); ++i)
    {
      if (instruction[i] != diagram[i])
      {
        isValid = false;
        break;
      }
    }

    if (isValid)
    {
      return pushedButtons.size();
    }
    else
    {
      return UINTPTR_MAX;
    }
  }

  // button[index] is off
  auto newSolution1 = inspectButton(buttons, instruction, diagram, index + 1, pushedButtons);

  auto button = buttons[index];
  pushedButtons.insert(index);
  for (auto val : button)
  {
    diagram[val] = !diagram[val];
  }

  // button[index] is on
  auto newSolution2 = inspectButton(buttons, instruction, diagram, index + 1, pushedButtons);

  return min(newSolution1, newSolution2);
}

size_t part1(const vector<vector<bool>> &instructions, vector<vector<vector<int>>> &buttonRows)
{
  size_t sum = 0;

  for (size_t i = 0; i < instructions.size(); ++i)
  {
    auto instruction = instructions[i];
    vector<bool> diagram;
    for (size_t j = 0; j < instruction.size(); ++j)
    {
      diagram.push_back(false);
    }

    set<int> pushedButtons;
    size_t index = 0;
    sum += inspectButton(buttonRows[i], instruction, diagram, index, pushedButtons);
  }

  return sum;
}

void parseLine(string &line, vector<vector<bool>> &diagrams, vector<vector<vector<int>>> &buttons, vector<vector<int>> &joltage)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(line);

  while (getline(tokenStream, token, ' '))
  {
    if (!token.empty())
    {
      tokens.push_back(token);
    }
  }

  vector<bool> diagram;
  vector<vector<int>> buttonsLine;
  vector<int> joltageLine;

  for (size_t i = 0; i < tokens.size(); ++i)
  {
    if (i == 0)
    {
      for (size_t j = 1; j < tokens[i].size() - 1; ++j)
      {
        auto light = tokens[i][j];
        if (light == '#')
        {
          diagram.push_back(true);
        }
        else if (light == '.')
        {
          diagram.push_back(false);
        }
      }
      continue;
    }

    string numToken;
    istringstream numTokenStream(tokens[i].substr(1, tokens[i].size() - 1));

    vector<int> button;

    while (getline(numTokenStream, numToken, ','))
    {
      if (!numToken.empty())
      {
        int num = stoi(numToken);
        if (i == tokens.size() - 1)
        {
          joltageLine.push_back(num);
        }
        else
        {
          button.push_back(num);
        }
      }
    }

    if (button.size() > 0)
    {
      buttonsLine.push_back(button);
    }
  }

  diagrams.push_back(diagram);
  buttons.push_back(buttonsLine);
  joltage.push_back(joltageLine);
}

void parseFile(string filename, vector<vector<bool>> &diagrams, vector<vector<vector<int>>> &buttons, vector<vector<int>> &joltage)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file: " + filename);
  }

  string line;
  while (getline(file, line))
  {
    if (!line.empty())
    {
      parseLine(line, diagrams, buttons, joltage);
    }
  }
}

int main()
{
  try
  {
    auto start = chrono::high_resolution_clock::now();

    vector<vector<bool>> diagrams;
    vector<vector<vector<int>>> buttons;
    vector<vector<int>> joltage;

    parseFile("input.txt", diagrams, buttons, joltage);

    auto resPart1 = part1(diagrams, buttons);
    auto timePart1 = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    cout << format("Part 1: {} in {}μs\n", resPart1, timePart1.count());

    auto resPart2 = part2(joltage, buttons);
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