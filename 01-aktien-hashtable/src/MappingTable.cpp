#include "MappingTable.h"
#include <cmath>

#include "Common.h"

MappingTable::MappingTable(int size) 
{
    mtable.resize(TABLESIZE);
    for (int i=0;i<TABLESIZE;i++)
    {
        mtable[i].status = EMP;
    }
}

MappingTable::~MappingTable()
{
    //dtor
}

void MappingTable::reset(){
    mtable.clear();
    mtable.resize(TABLESIZE);

    for (int i=0;i<TABLESIZE;i++)
    {
        mtable[i].status = EMP;
    }
}

bool MappingTable::add(const std::string &key,const int &nameIndex)
{
    int index = hash(key);
    int i = 0;
    int pos=index;

    while (i < TABLESIZE && mtable[pos].status != EMP) {
        int offset = (i % 2 == 0) ? (i * i) : -(i * i);
        pos = ((index + offset) % TABLESIZE + TABLESIZE) % TABLESIZE;
        i++;
    }

    
    
    if (i==TABLESIZE) return false;

    mtable[pos].status = OCC;
    mtable[pos].key = key;
    mtable[pos].nameIndex=nameIndex;

    return true;
}

bool MappingTable::del(const std::string &key)
{
    int index = hash(key);
    int i = 0;
    int pos = index;
    

    while (i < TABLESIZE && mtable[pos].status != EMP) 
    {

        int offset = (i % 2 == 0) ? (i * i) : -(i * i);
        pos = ((index + offset) % TABLESIZE + TABLESIZE) % TABLESIZE;

        if ( mtable[pos].status == OCC && mtable[pos].key == key) break;
        i++;
    }

    if (i==TABLESIZE) return false;

    mtable[pos].status = DEL;

    return true;
}

int MappingTable::check(const std::string &key,const int &index)
{
    int i = 0;
    int pos = index;
    
    while (i < TABLESIZE && mtable[pos].status != EMP) {
        int offset = (i % 2 == 0) ? (i * i) : -(i * i);
        pos = ((index + offset) % TABLESIZE + TABLESIZE) % TABLESIZE;
        if (mtable[pos].status == OCC && mtable[pos].key == key) return mtable[pos].nameIndex;
        i++;
    }
    return -1;
    
}

int MappingTable::hash(const std::string &key)
{
    long long hashValue = 0;

    for (int i=1 ;i<=key.length();i++) {
        char buchstabe = key[i - 1];
        int asciiWert = static_cast<int>(buchstabe);  // ASCII-Wert erhalten
        hashValue += (static_cast<int>(std::pow(asciiWert,i)) % TABLESIZE);
    }

    hashValue %= TABLESIZE;

    return hashValue;
}