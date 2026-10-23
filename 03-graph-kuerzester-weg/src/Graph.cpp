#include "Graph.h"
#include "Utility.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>    // für std::quoted
#include <unordered_set>

inline void Graph::addEdge(const string& from, const string& to, int duration, const string& lane) {
    net[from].push_back({to, duration, lane});
    net[to].push_back({from, duration, lane});
};

void Graph::loadFile(const string& path) {
    ifstream in(path);
    if (!in) throw runtime_error("Datei '" + path + "' nicht gefunden!");

    net.clear();
    loadedFile = path;
    string line;

    while(getline(in, line)) {
        if (line.empty()) continue;

        stringstream ls(line);
        string lane;
        getline(ls, lane, ':');
        trim(lane);

        vector<string> stations;
        vector<int> durations;

        // while ls >> ws - überspringt leerzeichen außerhalb " "
        while (ls >> ws) {
            if (ls.peek() == '"'){
                string tok;
                ls >> quoted(tok);
                stations.push_back(toLowerCase(tok));
            } else {
                int duration;
                ls >> duration;
                durations.push_back(duration);
            }
        }

        for (size_t i = 0; i + 1 < stations.size(); ++i) {
            int duration =  (i < durations.size()) ? durations[i] : 0;
            if (duration == 0) {
                throw invalid_argument("Dauer 0 zwischen " + stations[i] + " und " + stations[i+1] + "\n");
            }
            addEdge(stations[i], stations[i+1], duration, lane);
        }
    }
}


void Graph::wayfinder(const string& filename, const string& args){
    string start, end;

    if (filename != loadedFile) {
        try {
            loadFile(filename);
        } catch (const exception& e) {
            cerr << e.what() << endl;
            loadedFile.clear();   // halb geladenes Netz beim nächsten Aufruf neu einlesen
            return;
        }
    }

    istringstream iss(args);
    iss >> quoted(start)
        >> quoted(end);

    start = toLowerCase(start);
    end = toLowerCase(end);

    if ( (net.find(start) == net.end()) || (net.find(end) == net.end())){
        cerr << "Fehlerhafte Argumente: \"Start\" oder \"Ziel\" nicht gefunden." << endl;
        return;
    }

    if (start == end) {
        cout << "Start und Ziel sind identisch, Fahrzeit: 0 Min" << endl;
        return;
    }

    shortestWay(start,end);

}

void Graph::shortestWay(const string& start, const string& end){
    int minway=0;
    string currentStopName = start;
    unordered_set<string> visited;
    vector<WayTo> ToVisit; // Connection nextStop; vector<Connection> pastWay;
    vector<Connection> currentWay;
    Connection currentConn;
    visited.reserve(mapSize);

    while (currentStopName!=end){
        auto it = net.find(currentStopName);
        vector<Connection> connsFrom = it->second;

        visited.insert(currentStopName);

        for (auto c : connsFrom){
            bool found = false;
            auto result = visited.find(c.destination);

            if ( result == visited.end()){

                for (int i=0; i<ToVisit.size();i++){
                    if (ToVisit.at(i).nextStop.destination == c.destination){
                        found = true;
                        if (ToVisit.at(i).nextStop.duration > (c.duration+minway)){
                            c.duration+=minway;
                            ToVisit.at(i).nextStop=c;
                            ToVisit.at(i).pastWay=currentWay;
                        } else break;
                    }
                }

                if (!found){
                    WayTo tempToVisit;
                    c.duration+=minway;
                    tempToVisit.nextStop = c;
                    tempToVisit.pastWay = currentWay;
                    ToVisit.push_back(tempToVisit);
                }

            }

        }

        if (ToVisit.empty()){
            cerr << "Kein Weg von " << start << " nach " << end << " gefunden." << endl;
            break;
        }

        sort(ToVisit.begin(),ToVisit.end(),[](auto const& A, auto const& B) {
            return A.nextStop.duration < B.nextStop.duration;
        });

        currentConn = ToVisit.at(0).nextStop;
        currentWay = ToVisit.at(0).pastWay;
        currentWay.push_back(currentConn);
        currentStopName=currentConn.destination;
        minway = currentConn.duration;
        ToVisit.erase(ToVisit.begin());
    }

    printPath(start,currentWay);

}


void Graph::printPath(const string& start, const vector<Connection>& Way){

    string Lane = Way.at(0).lane;

    cout << "Start: " << start << endl;
    cout << "Linie " << Lane << ":";
    for (int i=0;i<Way.size();i++){

        if (Lane != Way.at(i).lane){
            Lane = Way.at(i).lane;
            cout << "\nUmsteigen zu " << Lane << ":";

        }

        cout << " -> " << Way.at(i).destination;
    }
    cout << endl;
    cout << "Fahrzeit: " << Way.back().duration << " Min" << endl;

}


