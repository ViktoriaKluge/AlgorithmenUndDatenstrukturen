
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Stock.h"
#include "Common.h"
#include "MappingTable.h"


struct hashitem {
    Status status;
    Stock stock;
};

class HashTable
{
    public:
        HashTable(int size, MappingTable& mt);
        virtual ~HashTable();

        std::vector<hashitem> htable;

        int hash(const std::string &key);

        Stock* search(const std::string &key);
        bool save(const std::string &filename);
        bool load(const std::string &filename);
        bool add(const Stock &stock);
        bool del(const std::string &key);
        int check(const std::string &key, const int &index);

        void listStocks() const;
        
    protected:

    private:
        int tableElements;
        MappingTable& mapTable;
};

#endif // HASHTABLE_H
