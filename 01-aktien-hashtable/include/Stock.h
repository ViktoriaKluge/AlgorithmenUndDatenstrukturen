#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>

struct KursDaten {
    std::string date;
    double close;
    long volume;
    double open;
    double high;
    double low;

};

class Stock
{
    public:
        Stock();
        virtual ~Stock();

        Stock(std::string &name, std::string &wkn, std::string &key);

        std::string name;
        std::string wkn;
        std::string key;
        std::vector<KursDaten> kursdaten; 

        bool import(std::string &filename);
        void printData() const;
        
        void plotData() const;
    protected:

    private:
};

#endif // STOCK_H
