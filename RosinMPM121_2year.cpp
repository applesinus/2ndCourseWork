#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;



struct variable
{
    string name;
    int size;
    bool visibility;
    int visibilityLevelID;
};



variable variableSaver(string line, int visibilityLevelID)
{
    variable newVar;
    int nameStart = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] != ' ' && line[i] != '\t') {
            nameStart = i;
            break;
        }
    }
    newVar.name = line.substr(nameStart, line.find("=") - nameStart);
    newVar.size = stoi(line.substr(line.find("(") + 1, line.find(")") - line.find("(")));
    newVar.visibility = true;
    newVar.visibilityLevelID = visibilityLevelID;
    return newVar;
}



int main()
{
    cout << "Hello!\n\tName: Dmitry Rosin\n\tGroup: MPM-121\n\tTask: 11\n\tWelcome to my 2nd year coursework!\n\n";

    cout << "==================================================\n\n\t\t     INFO:\n\n";
    cout << "This program is a Memory manager, \n";
    cout << "If you wanna stop the program, say \'n\'\n";
    cout << "\n==================================================\n\n";

    char willContinue;

    do {
        ifstream file;
        int visibilityLevelID = 0;
        int N;
        vector<variable> stack;
        vector<int> openedVisibilityLevels;
        int memoryUsed = 0;


        cout << "=====\n\n";
        cout << "Enter the name of new file (with extension like .txt): ";
        string fileName;
        cin >> fileName;
        file.open(fileName);
        cout << "\n\tNEW FILE HAS BEEN OPENED\n\n";

        cout << "Enter a limitation of memory for this file: ";
        cin >> N;


        string line;
        while (getline(file, line))
        {
            if (line.find("{") != string::npos)
            {
                visibilityLevelID++;
                openedVisibilityLevels.push_back(visibilityLevelID);
            }

            if (line.find("}") != string::npos)
            {
                for (int i = 0; i < stack.size(); i++)
                {
                    if (stack[i].visibilityLevelID == openedVisibilityLevels.back())
                    {
                        stack[i].visibility = false;
                    }
                }

                openedVisibilityLevels.pop_back();
                if (openedVisibilityLevels.size() > 0)
                {
                    visibilityLevelID = openedVisibilityLevels.back();
                }
            }

            if (line.find("new") != string::npos)
            {
                int newSize = memoryUsed + stoi(line.substr(line.find("(") + 1, line.find(")") - line.find("(")));

                if (newSize <= N)
                {
                    variable newVar = variableSaver(line, visibilityLevelID);
                    stack.push_back(newVar);
                    memoryUsed += newVar.size;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (!stack[i].visibility)
                        {
                            memoryUsed -= stack[i].size;
                            newSize -= stack[i].size;
                            stack.erase(stack.begin()+i);
                            i--;
                        }
                    }

                    if (newSize <= N)
                    {
                        variable newVar = variableSaver(line, visibilityLevelID);
                        stack.push_back(newVar);
                        memoryUsed += newVar.size;
                    }
                    else
                    {
                        int nameStart = 0;
                        for (int i = 0; i < line.size(); i++) {
                            if (line[i] != ' ' && line[i] != '\t') {
                                nameStart = i;
                                break;
                            }
                        }
                        cout << "ERROR: OUT OF MEMORY. CAN'T SAVE " << line.substr(nameStart, line.find("=") - nameStart) << endl;
                    }
                }
            }

            if (line.find("ShowVar") != string::npos)
            {
                cout << "\nHere is visible variables:\n";
                for (int i = 0; i < stack.size(); i++) {
                    if (stack[i].visibility)
                    {
                        cout << stack[i].name << ": size=" << stack[i].size << endl;
                    }
                }
                cout << endl;
            }
        }


        file.close();


        cout << "\nIs there any additional files?\nPlease type only \'y\' for yes and \'n\' for no.\nAnswer: ";
        cin >> willContinue;


        while (willContinue != 'y' && willContinue != 'n') {
            cout << "\nIncorrect Input\n";
            cout << "Is there any additional files?\nPlease type only \'y\' for yes and \'n\' for no.\nAnswer: ";
            cin >> willContinue;
        }


        cout << "\n\tEND OF THE FILE\n\n";
    }
    while (willContinue == 'y');
    

    return 0;
}

