#include <cmath>  // Für pow
#include "HashTable.h"
#include "Common.h"
#include "Stock.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 


HashTable::HashTable(int size, MappingTable& mt) : tableElements(0), mapTable(mt) {
    htable.resize(TABLESIZE);
    for (int i=0;i<TABLESIZE;i++)
    {
        htable[i].status = EMP;
    }

}
HashTable::~HashTable() {

}


int HashTable::hash(const std::string &key)
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

Stock* HashTable::search(const std::string &key) {

    int index = hash(key);
    int i = 0; 
    int pos = index;

    while (i < TABLESIZE && htable[pos].status != EMP) {
    
        int offset = (i % 2 == 0) ? (i * i) : -(i * i);
        pos = ((index + offset) % TABLESIZE + TABLESIZE) % TABLESIZE; // Hierbei handelt es sich um einen gängigen Trick

        if (htable[pos].status == OCC && htable[pos].stock.name == key)  return &htable[pos].stock;

        i++;
    }

    
    if (i==TABLESIZE) return nullptr;
    // Wenn nichts gefunden wird -> mtable check
    int mappingIndex = mapTable.check(key,index);
    if (mappingIndex >= 0) {
        return &htable[mappingIndex].stock;
    }
    return nullptr;
}



bool HashTable::save(const std::string &filename) {
    std::ofstream out(filename, std::ios::trunc);
    if (!out.is_open()) { return false; }
    
    for(int i = 0; i < TABLESIZE; i++){
        if(htable[i].status == EMP || htable[i].status == DEL) continue;

        if(htable[i].status == OCC){
            out << htable[i].stock.name << "," << htable[i].stock.wkn << "," << htable[i].stock.key << ",";
            for (auto &kd : htable[i].stock.kursdaten) {
                out << kd.date << "," << kd.close << "," << kd.volume << "," << kd.open << "," << kd.high << "," << kd.low<< ",";
            }     
        }
        out << "\n";
    }
    out.close();
    return true;
}

bool HashTable::load(const std::string &filename) {
    std::ifstream in(filename);
    std::string line;

    if (!in.is_open()) {
        std::cerr << "Fehler beim Oeffnen der Datei " << filename << std::endl;
        return false;
    }

    htable.clear();
    htable.resize(TABLESIZE);

    for (int i=0;i<TABLESIZE;i++)
    {
        htable[i].status = EMP;
    }
    mapTable.reset();


    while (std::getline(in, line)) {
        std::stringstream ss(line);

        Stock tempStock;
        KursDaten kd;
        std::string token;

        std::getline(ss, tempStock.name, ',');  
        std::getline(ss, tempStock.wkn, ',');
        std::getline(ss, tempStock.key, ',');
        
        while (std::getline(ss, token, ',')) {
            kd.date = token;
            std::getline(ss, token, ',');
            kd.close = std::stod(token);
            std::getline(ss, token, ',');
            kd.volume = std::stol(token);
            std::getline(ss, token, ',');
            kd.open = std::stod(token);
            std::getline(ss, token, ',');
            kd.high = std::stod(token);
            std::getline(ss, token, ',');
            kd.low = std::stod(token);
            tempStock.kursdaten.push_back(kd);
        }   
        add(tempStock);
        
    }
    in.close();
    return true;
}

bool HashTable::add(const Stock &stock)
{
    int index = hash(stock.name);
    int i = 0;
    int pos = index;
    while (i < TABLESIZE && htable[pos].status != EMP) {
        int offset = (i % 2 == 0) ? (i * i) : -(i * i);
        pos = ((index + offset) % TABLESIZE + TABLESIZE) % TABLESIZE; 
        i++;
    }

    if (i==TABLESIZE) return false;

    htable[pos].status = OCC;
    htable[pos].stock = stock;
    tableElements++;

    if (tableElements==TABLESIZE) std::cout<<"Hashtabelle voll.\n";

    return mapTable.add(stock.key,index);

}

bool HashTable::del(const std::string &key)
{
    int index = hash(key);
    int i = 0;
    int pos = index;
    int indexFromMapping=0;
    

    while (i < TABLESIZE && htable[pos].status != EMP) 
    {
        int offset = (i % 2 == 0) ? (i * i) : -(i * i);
        pos = ((index + offset) % TABLESIZE + TABLESIZE) % TABLESIZE; 
        if (htable[pos].status == OCC && htable[pos].stock.name == key)
        {
            htable[pos].status = DEL;
            tableElements--;
           return mapTable.del(htable[pos].stock.key);
        }
        
        i++;
    }


    indexFromMapping = mapTable.check(key,index);
    if (indexFromMapping>0) 
        {
            htable[indexFromMapping].status = DEL;
            tableElements--;
            return mapTable.del(htable[indexFromMapping].stock.key);
        ;
        }

    return false;
}


void HashTable::listStocks() const {
    int count = 0;
    bool found = false;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Liste der Aktien:" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    for (const auto &item : htable) {
        if (item.status == OCC) {
            if (count % 5 != 0) {
                std::cout << ",";
            }
            std::cout << item.stock.name;
            count++;
            found = true;
            if (count % 5 == 0) std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    if (!found) {
        std::cout << "Keine Aktien vorhanden." << std::endl;
    }
    std::cout << std::endl; std::cout << std::endl;
}
