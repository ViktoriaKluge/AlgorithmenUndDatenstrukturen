#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

struct Connection {
    string destination;
    int duration;
    string lane;
};

struct WayTo {
    Connection nextStop;
    vector<Connection> pastWay;
};


class Graph
{
    public:
        inline void addEdge(const string& from, const string& to, int duration, const string& lane);
        void loadFile(const string& path);
        void wayfinder(const string& filename, const string& args);

    protected: 

    private:
        unordered_map<string, vector<Connection>> net;
        string loadedFile;
        int mapSize;
        void shortestWay(const string& start, const string& end);

        void printPath(const string& start, const vector<Connection>&);
        
};

#endif // GRAPH_H
