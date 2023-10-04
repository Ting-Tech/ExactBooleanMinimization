#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

void commendHandler(ifstream &inputFile, ofstream &outputFile,
                    const bool &debugMode)
{
    char ob;
    string line;
    int i = 0, o = 0, p = 0;
    vector<char> ilb;
    vector<vector<string>> sheet;
    vector<string> logicSheet;
    vector<string> trueCombination;

    while (getline(inputFile, line))
    {
        string command;
        stringstream ss(line);

        ss >> command;

        if (command == ".i")
        {
            ss >> i;
        }

        else if (command == ".o")
            ss >> o;

        else if (command == ".ilb")
        {
            for (size_t c = 0; c < i; c++)
            {
                char sign;
                ss >> sign;
                ilb.push_back(sign);
            }
        }

        else if (command == ".ob")
            ss >> ob;

        else if (command == ".p")
            ss >> p;

        else if (command[0] == '1' ||
                 command[0] == '-' ||
                 command[0] == '0')
        {
            trueCombination.push_back(command);
        }

        else if (command == ".e")
        {
        }

        else
            continue;
    }
    if (debugMode == true)
    {
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