#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

inline void trim(string& s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){
        return !isspace(ch);
    }));
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){
        return !isspace(ch);
    }).base(), s.end());
}

inline string toLowerCase(string s){
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c){
        return tolower(c);
    });
    return s;
}

#endif // UTILITY_H
