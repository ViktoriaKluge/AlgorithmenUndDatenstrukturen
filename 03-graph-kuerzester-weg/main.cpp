#include <iostream>
#include <string>
#include <sstream>

#include "Graph.h"

using namespace std;

int main()
{
    string input;
    string filename;
    Graph g;
    
    while(1) {
        cout << "Befehl:" << endl;
        cout << "<filename> \"Start\" \"Ziel\"" << endl;
        cout << "<0> Beenden" << endl;
        
        getline(cin, input);
        if (input.empty()) continue;

        istringstream iss(input);
        string command;
        iss >> command;
        
        if (command == "0") break;

        filename = command;

        // Leerzeichen nach dem Command wird entfernt & Rest in args gepackt
        string args;
        getline(iss, args);

        if(!args.empty()) {
            g.wayfinder(filename,args);
            
        } else {
            cerr << "Keine Argumente (\"Start\" \"Ziel\") angegeben." << endl;
            continue;
        }
    }

    return 0;
}
