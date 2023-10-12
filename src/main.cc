#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <map>

using namespace std;

typedef vector<pair<int, pair<vector<int>, string>>> termList_t;

vector<vector<string>> feasibleSolutions(vector<vector<vector<string>>>
                                             petrickMethodResult)
{
    vector<vector<string>> result;

    for (auto &minterms : petrickMethodResult[0])
    {
        if (minterms.size() < 3)
            result.push_back(minterms);
    }

    return result;
}

vector<vector<vector<string>>>
petrickMethod(
    const pair<vector<pair<string, vector<int>>>,
               vector<pair<string, vector<int>>>> &piarOfPrimeImplicant)
{
    vector<pair<string, vector<int>>> epi = piarOfPrimeImplicant.first;
    vector<pair<string, vector<int>>> nEpi = piarOfPrimeImplicant.second;
    vector<int> epiMinterms;
    map<int, vector<string>> mintermMap;
    vector<vector<vector<string>>> result;

    for (auto &epiPair : epi)
    {
        for (auto &minterm : epiPair.second)
        {
            epiMinterms.push_back(minterm);
        }
    }

    for (auto &nEpiPair : nEpi)
    {
        vector<int> nEpiMinterms;
        for (auto it = nEpiPair.second.begin();
             it != nEpiPair.second.end(); ++it)
        {
            if (!(find(epiMinterms.begin(),
                       epiMinterms.end(), *it) != epiMinterms.end()))
            {
                nEpiMinterms.push_back(*it);
            }
        }
        nEpiPair.second = nEpiMinterms;
    }

    for (auto &nEpiPair : nEpi)
    {
        for (auto &minterm : nEpiPair.second)
        {
            mintermMap[minterm].push_back(nEpiPair.first);
        }
    }

    for (auto &minterms : mintermMap)
    {
        vector<vector<string>> feasibleSolutions;
        for (auto &minterm : minterms.second)
        {
            vector<string> feasibleSolution;
            feasibleSolution.push_back(minterm);
            feasibleSolutions.push_back(feasibleSolution);
        }
        result.push_back(feasibleSolutions);
    }

    // while (result.size() != 1)
    for (size_t x = 0; x < 2; x++)
    {
        vector<vector<string>> feasibleSolutions;
        for (size_t i = 0; i < result[0].size(); i++)
        {
            for (size_t j = 0; j < result[1].size(); j++)
            {
                vector<string> feasibleSolution;
                for (auto &value : result[0][i])
                {
                    if (!(find(feasibleSolution.begin(),
                               feasibleSolution.end(),
                               value) != feasibleSolution.end()))
                    {
                        feasibleSolution.push_back(value);
                    }
                }
                for (auto &value : result[1][j])
                {
                    if (!(find(feasibleSolution.begin(),
                               feasibleSolution.end(),
                               value) != feasibleSolution.end()))
                    {
                        feasibleSolution.push_back(value);
                    }
                }
                sort(feasibleSolution.begin(), feasibleSolution.end());
                if (!(find(feasibleSolutions.begin(),
                           feasibleSolutions.end(),
                           feasibleSolution) != feasibleSolutions.end()))
                {
                    feasibleSolutions.push_back(feasibleSolution);
                }
            }
        }
        result.push_back(feasibleSolutions);

        for (size_t i = 0; i < 2; i++)
        {
            result[i].clear();
        }

        vector<vector<vector<string>>> clearResult;
        for (size_t i = 2; i < result.size(); i++)
        {
            clearResult.push_back(result[i]);
        }
        result = clearResult;
    }

    return result;
}

pair<vector<pair<string, vector<int>>>, vector<pair<string, vector<int>>>>
findEssentialPrimeImplicant(const termList_t &termList)
{
    vector<int> epiIndex;
    vector<pair<string, vector<int>>> epi;
    vector<pair<string, vector<int>>> nEPI;
    map<int, int> mintermsMap; // m幾 在第幾個minterm行
    for (size_t i = 0; i < termList.size(); i++)
    {
        for (size_t j = 0; j < ((termList[i].second).first).size(); j++)
        {
            int targetIndex = (((termList[i].second).first)[j]);
            if (mintermsMap.find(targetIndex) != mintermsMap.end())
            {
                mintermsMap[targetIndex] += (i * termList.size());
            }
            else
            {
                mintermsMap[targetIndex] = i;
            }
        }
    }

    for (auto &minterms : mintermsMap)
    {
        if (minterms.second < termList.size() - 1)
        {
            int mintermIndex = minterms.second;
            string notation = (termList[minterms.second].second).second;
            pair<string,
                 vector<int>>
                epiPair(notation, termList[mintermIndex].second.first);
            epi.push_back(epiPair);
            epiIndex.push_back(minterms.second);
        }
    }

    for (size_t i = 0; i < termList.size(); i++)
    {
        if (!(count(epiIndex.begin(), epiIndex.end(), i)))
        {
            string notation = (termList[i].second).second;
            pair<string, vector<int>> nEpiPair(notation,
                                               termList[i].second.first);
            nEPI.push_back(nEpiPair);
        }
    }

    pair<vector<pair<string, vector<int>>>, vector<pair<string, vector<int>>>>
        result(epi, nEPI);
    return result;
}

termList_t simplification(const termList_t &inputList)
{
    termList_t result;

    for (size_t i = 0; i < (inputList.size() - 1); i++)
    {
        for (size_t j = (i + 1); j < inputList.size(); j++)
        {
            if (inputList[j].first ==
                (inputList[i].first + 1))
            {
                int differentIndex = 0, differentCount = 0;
                for (size_t k = 0;
                     k < ((inputList[i].second).second).length();
                     k++)
                {
                    if ((inputList[i].second).second[k] !=
                        (inputList[j].second).second[k])
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

                    for (auto &terms : (inputList[i].second).first)
                    {
                        resultMidterms.push_back(terms);
                    }

                    for (auto &terms : (inputList[j].second).first)
                    {
                        resultMidterms.push_back(terms);
                    }

                    sort(resultMidterms.begin(), resultMidterms.end());

                    resultLiteralTerm = (inputList[i].second).second;
                    resultLiteralTerm[differentIndex] = '-';

                    int group = 0;
                    for (size_t i = 0; i < resultLiteralTerm.length(); i++)
                    {
                        if (resultLiteralTerm[i] == '1')
                            group++;
                    }

                    pair<vector<int>, string> pairTerms(resultMidterms,
                                                        resultLiteralTerm);
                    result.push_back(pair<int, pair<vector<int>,
                                                    string>>(group, pairTerms));
                    sort(result.begin(), result.end());
                }
            }
            else if (inputList[j].first > (i + 1))
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

termList_t sortCombination(vector<string> combinations)
{
    termList_t result;

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
        result.push_back(pair<int,
                              pair<vector<int>, string>>(group, pairTerms));
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
                 termList_t &inputList,
                 termList_t &_threeLitteralTerms,
                 termList_t &_twoLitteralTerms,
                 pair<vector<pair<string, vector<int>>>,
                      vector<pair<string, vector<int>>>>
                     primeImplicantPair,
                 const vector<vector<vector<string>>> &petrickMethod)
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

    for (auto &terms : inputList)
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

    cout << endl;

    for (auto &epi : primeImplicantPair.first)
    {
        cout << epi.first << " ";
        for (auto &minterm : epi.second)
        {
            cout << minterm << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (auto &epi : primeImplicantPair.second)
    {
        cout << epi.first << " ";
        for (auto &minterm : epi.second)
        {
            cout << minterm << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (auto &feasibleSolutions : petrickMethod)
    {
        for (auto &feasibleSolution : feasibleSolutions)
        {
            for (auto &minterm : feasibleSolution)
            {
                cout << "(" << minterm << ")";
            }
            cout << " + ";
        }
        cout << endl;
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
    termList_t inputList;
    termList_t _threeLitteralTerms;
    termList_t _twoLitteralTerms;
    pair<vector<pair<string, vector<int>>>, vector<pair<string, vector<int>>>>
        primeImplicantPair;
    vector<vector<vector<string>>> petrickMethodResult;

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
            inputList = sortCombination(trueCombination);
            _threeLitteralTerms = simplification(inputList);
            _twoLitteralTerms = simplification(_threeLitteralTerms);
            primeImplicantPair = findEssentialPrimeImplicant(_twoLitteralTerms);
            petrickMethodResult = petrickMethod(primeImplicantPair);
            feasibleSolutions(petrickMethodResult);
        }

        else
            continue;
    }
    if (debugMode == true)
    {
        debugOutput(trueCombination,
                    dontCareCombination,
                    inputList,
                    _threeLitteralTerms,
                    _twoLitteralTerms,
                    primeImplicantPair,
                    petrickMethodResult);
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