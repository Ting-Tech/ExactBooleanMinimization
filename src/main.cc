#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <map>

using namespace std;

typedef vector<pair<int, pair<vector<int>, string>>> termList;

termList simplification(const termList &_termList)
{
    termList result;

    for (size_t i = 0; i < (_termList.size() - 1); i++)
    {
        for (size_t j = (i + 1); j < _termList.size(); j++)
        {
            if (_termList[j].first ==
                (_termList[i].first + 1))
            {
                int differentIndex = 0, differentCount = 0;
                for (size_t k = 0;
                     k < ((_termList[i].second).second).length();
                     k++)
                {
                    if ((_termList[i].second).second[k] !=
                        (_termList[j].second).second[k])
                    {
                        differentCount++;
                        differentIndex = k;
                    }
                    else if (differentCount > 1)
                        break;
                }
                if (differentCount == 1)
                {
                    vector<int> resultMidterms;
                    string resultLiteralTerm;

                    for (auto &terms : (_termList[i].second).first)
                    {
                        resultMidterms.push_back(terms);
                    }

                    for (auto &terms : (_termList[j].second).first)
                    {
                        resultMidterms.push_back(terms);
                    }

                    sort(resultMidterms.begin(), resultMidterms.end());

                    resultLiteralTerm = (_termList[i].second).second;
                    resultLiteralTerm[differentIndex] = '-';

                    int group = 0;
                    for (size_t i = 0; i < resultLiteralTerm.length(); i++)
                    {
                        if (resultLiteralTerm[i] == '1')
                            group++;
                    }

                    pair<vector<int>, string> pairTerms(resultMidterms, resultLiteralTerm);
                    result.push_back(pair<int, pair<vector<int>, string>>(group, pairTerms));
                    sort(result.begin(), result.end());
                }
            }
            else if (_termList[j].first > (i + 1))
                break;
        }
    }

    for (size_t i = 0; i < result.size() - 1; i++)
    {
        for (size_t j = i + 1; j < result.size(); j++)
        {
            if (result[i] == result[j])
            {
                result.erase(result.begin() + j);
            }
        }
    }

    return result;
}

termList sortCombination(vector<string> combinations)
{
    termList result;

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
        int group = 0;
        vector<int> minterms;

        for (size_t j = 0; j < combinations[i].length(); j++)
        {
            if (combinations[i][j] == '1')
                group++;
        }

        minterms.push_back(stoi(combinations[i], 0, 2));
        pair<vector<int>, string> pairTerms(minterms, combinations[i]);
        result.push_back(pair<int, pair<vector<int>, string>>(group, pairTerms));
    }

    sort(result.begin(), result.end());

    return result;
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
                 termList &_termList,
                 termList &_threeLitteralTerms,
                 termList &_twoLitteralTerms)
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

    for (auto &terms : _termList)
    {
        cout << terms.first << " ";
        for (auto &literals : (terms.second).first)
        {
            cout << "m" << literals << " ";
        }
        cout << (terms.second).second << endl;
    }

    cout << endl;

    for (auto &terms : _threeLitteralTerms)
    {
        cout << terms.first << " ";
        for (auto &literals : (terms.second).first)
        {
            cout << "m" << literals << " ";
        }
        cout << (terms.second).second << endl;
    }

    cout << endl;

    for (auto &terms : _twoLitteralTerms)
    {
        cout << terms.first << " ";
        for (auto &literals : (terms.second).first)
        {
            cout << "m" << literals << " ";
        }
        cout << (terms.second).second << endl;
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
    termList _termList;
    termList _threeLitteralTerms;
    termList _twoLitteralTerms;

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
            _termList = sortCombination(trueCombination);
            _threeLitteralTerms = simplification(_termList);
            _twoLitteralTerms = simplification(_threeLitteralTerms);
        }

        else
            continue;
    }
    if (debugMode == true)
    {
        debugOutput(trueCombination,
                    dontCareCombination,
                    _termList,
                    _threeLitteralTerms,
                    _twoLitteralTerms);
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