#ifndef MAPPINGTABLE_H
#define MAPPINGTABLE_H

#include <string>
#include <vector>

#include "Common.h"

struct MapItem  {
    Status status;
    std::string key;
    int nameIndex;
};


class MappingTable
{
    public:
        MappingTable(int size);
        virtual ~MappingTable();
        std::vector<MapItem> mtable;
        
        bool add(const std::string &key,const int &nameIndex);
        bool del(const std::string &key);
        int check(const std::string &key,const int &index);

        int hash(const std::string &key);

        void reset();
    protected:

    private:

};

#endif // MAPPINGTABLE_H
