#include <iostream>
#include <sstream>
#include <string>

#include "Tree.h"


using namespace std;


int main()
{
    string input;

    while(1) {
        cout << "\nBefehle:" << endl;
        cout << " > treecheck <filename>" << endl;
        cout << " > treecheck <filename> <subtree>" << endl;
        cout << " < exit" << endl;
        getline(cin, input);

        if (input.empty()) continue;

        // Erster Teil des Inputs wird in commands gesteckt
        istringstream iss(input);
        string command;
        iss >> command;

        // Leerzeichen nach dem Command wird entfernt & Rest in args gepackt
        string args;
        getline(iss, args);
        args.erase(0, args.find_first_not_of(" \t"));   // Löscht das Leerzeichen nach dem Command

        if(command == "treecheck"){
            if(!args.empty()) {
                treecheck(args);
            } else {
                cerr << "Keine Argumente (filename, subtree) angegeben." << endl;
                continue;
            }
        } else if (command == "exit") {
            break;;
        } else {
            cerr << "Eingabe fehlerhaft." << endl;
        }
    }
    return 0;
};