#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <map>

using namespace std;

vector<pair<vector<int>, string>>
simplification(const vector<pair<int, string>> &numCombinations)
{
    vector<pair<vector<int>, string>> result;

    for (size_t i = 0; i < (numCombinations.size() - 1); i++)
    {
        for (size_t j = (i + 1); j < numCombinations.size(); j++)
        {
            if (numCombinations[j].first ==
                (numCombinations[i].first + 1))
            {
                int differentIndex = 0, differentCount = 0;
                for (size_t k = 0;
                     k < (numCombinations[i].second).length();
                     k++)
                {
                    if ((numCombinations[i].second)[k] !=
                        (numCombinations[j].second)[k])
                    {
                        differentCount++;
                        differentIndex = k;
                    }
                    else if (differentCount > 1)
                        break;
                }
                if (differentCount == 1)
                {
                    vector<int> resultIndex;
                    string resultString;
                    resultIndex.push_back(numCombinations[i].first);
                    resultIndex.push_back(numCombinations[j].first);
                    resultString = numCombinations[i].second;
                    resultString[differentIndex] = '-';
                    result.push_back(pair<vector<int>,
                                          string>(resultIndex, resultString));
                }
            }
            else if (numCombinations[j].first > (i + 1))
                break;
        }
    }
    return result;
}

vector<pair<int, string>> sortCombination(vector<string> combinations)
{
    vector<pair<int, string>> numVec;

    for (size_t i = 0; i < combinations.size(); i++)
    {
        for (size_t j = i + 1; j < combinations.size(); j++)
        {
            if (combinations[i] == combinations[j])
            {
                combinations.erase(combinations.begin() + j);
            }
        }
    }

    for (size_t i = 0; i < combinations.size(); i++)
    {
        int numOf1 = 0;
        for (size_t j = 0; j < combinations[i].length(); j++)
        {
            if (combinations[i][j] == '1')
                numOf1++;
        }

        numVec.push_back(pair<int, string>(numOf1,
                                           combinations[i]));
    }

    sort(numVec.begin(), numVec.end());

    return numVec;
}

void exhaustiveMethod(string combination, vector<string> &combinations)
{
    bool dontcareExist = false;
    int dontcareIndex = 0;
    for (size_t i = 0; i < combination.length(); i++)
    {
        if (combination[i] == '-')
        {
            dontcareExist = true;
            dontcareIndex = i;
            break;
        }
    }

    if (dontcareExist)
    {
        combination[dontcareIndex] = '0';
        exhaustiveMethod(combination, combinations);
        combination[dontcareIndex] = '1';
        exhaustiveMethod(combination, combinations);
    }
    else
    {
        combinations.push_back(combination);
    }
}

void debugOutput(const vector<string> &trueCombination,
                 const vector<string> &dontCareCombination,
                 const vector<pair<int, string>> &numMap,
                 vector<pair<vector<int>, string>> &threeLitteralTerms)
{
    for (auto &trueCom : trueCombination)
    {
        cout << trueCom << endl;
    }

    cout << endl;

    for (auto &dontCare : dontCareCombination)
    {
        cout << dontCare << endl;
    }

    cout << endl;

    for (auto &num : numMap)
    {
        cout << num.first << " " << num.second << endl;
    }

    cout << endl;

    for (auto &pair : threeLitteralTerms)
    {
        for (auto &num : pair.first)
        {
            cout << num << " ";
        }
        cout << pair.second << endl;
    }
}

void commendHandler(ifstream &inputFile, ofstream &outputFile,
                    const bool &debugMode)
{
    char ob;
    string line;
    int i = 0, o = 0, p = 0;
    vector<char> ilb;
    vector<string> trueCombination;
    vector<string> dontCareCombination;
    vector<pair<int, string>> numVec;
    vector<pair<vector<int>, string>> threeLitteralTerms;

    while (getline(inputFile, line))
    {
        string command;
        stringstream ss(line);

        ss >> command;

        if (command == ".i") // 讀取有幾個邏輯變數
        {
            ss >> i;
        }

        else if (command == ".o") // 讀取有幾個輸出結果
            ss >> o;

        else if (command == ".ilb") // 讀取邏輯變數符號
        {
            for (size_t c = 0; c < i; c++)
            {
                char sign;
                ss >> sign;
                ilb.push_back(sign);
            }
        }

        else if (command == ".ob") // 讀取輸出結果符號
            ss >> ob;

        else if (command == ".p") // 讀取會有幾組邏輯組合
            ss >> p;

        // 讀取邏輯組合
        else if (command[0] == '1' ||
                 command[0] == '-' ||
                 command[0] == '0')
        {
            char logicSign;
            ss >> logicSign;

            if (logicSign == '1')
                exhaustiveMethod(command, trueCombination);
            else if (logicSign == '-')
                exhaustiveMethod(command, dontCareCombination);
        }

        else if (command == ".e")
        {
            for (auto &combination : dontCareCombination)
            {
                trueCombination.push_back(combination);
            }
            numVec = sortCombination(trueCombination);
            threeLitteralTerms = simplification(numVec);
        }

        else
            continue;
    }
    if (debugMode == true)
    {
        debugOutput(trueCombination,
                    dontCareCombination,
                    numVec,
                    threeLitteralTerms);
    }
}

int main(int argc, char *argv[])
{
    ifstream inputFile;
    ofstream outputFile;
    inputFile.open(argv[1]);
    outputFile.open(argv[2]);

    if (argc == 3)
    {
        commendHandler(inputFile, outputFile, false);
    }
    else if (argc == 4)
    {
        if (string(argv[3]) == "debug")
            commendHandler(inputFile, outputFile, true);
    }
    else
    {
        cout << "Input fail" << endl;
        return 1;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}