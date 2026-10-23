#include <iostream>
#include <algorithm> // für std::transform
#include <cctype>    // für std::toupper

#include "Common.h"

#include "HashTable.h"
#include "MappingTable.h"
#include "Stock.h"

using namespace std;

MappingTable mtable(TABLESIZE);
HashTable htable(TABLESIZE, mtable);

HashTable* pHtable = &htable;
MappingTable* pMtable = &mtable;

int main()
{

    string command;

    while (true) {
        cout << "\nMenu:\n"
             << "ADD\n"
             << "DEL\n"
             << "IMPORT\n"
             << "SEARCH\n"
             << "PLOT\n"
             << "SAVE\n"
             << "LOAD\n"
             << "LIST\n"
             << "QUIT\n";
        cout << "Befehl eingeben: ";
        getline(cin, command);  // Befehl einlesen

        // Umwandlung in Großbuchstaben
        transform(command.begin(), command.end(), command.begin(), ::toupper);

        if (command.substr(0, 3) == "ADD") {
            string name;
            string key;
            string wkn;
            string csv;

            cout << "Name: ";
            getline(cin, name);

            cout << "Kuerzel: ";
            getline(cin, key);

            cout << "WKN: ";
            getline(cin, wkn);

            cout << "CSV-Datei (name.csv): ";
            getline(cin, csv);

            if (csv.size() < 4 || csv.substr(csv.size()-4) != ".csv") {
                cerr << "Fehler: Nur CSV-Datein erlaubt!" << endl;
                continue;
            }

            // Stock Objekt erstellen
            Stock newStock;
            newStock.name = name;
            newStock.wkn = wkn;
            newStock.key = key;

            // Import der Kursdaten
            if(!newStock.import(csv)) {
                cerr << "Fehler: Import aus " << csv << " fehlgeschlagen!" << endl;
                continue;
            }

            // Stock in die Hash-Tabelle einfügen
            if (!htable.add(newStock)) {
                cerr << "Fehler: Hinfuegen von " << name << " fehlgeschlagen!" << endl;
                continue;
            }
            else
            {
                cout << "Aktie von " << name << " hinzugefuegt." << endl;
            }
        }

        else if (command.substr(0,3) == "DEL") {

            std::string input;

            cout << "Name oder Kuerzel: ";
            getline(cin, input);
            
            if (!htable.del(input)) {
                cerr << "Fehler: Loeschen von " << input << " fehlgeschlagen!" << endl;
                continue;
            }
            else
            {
                cout << "Aktie von " << input << " geloescht." << endl;
            }
           
        }
        
        else if (command.substr(0, 7) == "IMPORT") {
            std::string input;
            std::string filename;

            cout << "Name oder Kuerzel: ";
            getline(cin, input);
            cout << "CSV-Datei (name.csv): ";
            getline(cin, filename);

            // Überprüfung der Dateiendung
            if (filename.size() < 4 || filename.substr(filename.size()-4) != ".csv") {
                cerr << "Fehler: Nur CSV-Datein erlaubt!" << endl;
                continue;
            }
            Stock* stockPtr = htable.search(input);
            if (stockPtr == nullptr) {
                cerr << "Fehler: " << input << " nicht gefunden!" << endl;
                continue;
            }

            // Kursdaten importieren
            if (!stockPtr->import(filename)) {
                cerr << "Fehler: Import aus " << filename << " fehlgeschlagen!" << endl;
                continue;
            }

            cout << "Kursdaten aus " << input << " zur Aktie " << stockPtr->name << " importiert." << endl;
        }
        else if (command.substr(0, 7) == "SEARCH") {
            std::string input;

            cout << "Name oder Kuerzel: ";
            getline(cin, input);
            Stock* tempPtr=htable.search(input);
            if (tempPtr==nullptr) {
                cerr << "Fehler: " << input << " nicht gefunden!" << endl;
                continue;
            }
            else
            {
                cout << "Aktie: " << tempPtr->name << endl;
                cout << "--------------------------------" << endl;
                cout << "Datum: " << tempPtr->kursdaten.front().date << endl;
                cout << "Close: " << tempPtr->kursdaten.front().close << endl;
                cout << "Volume: " << tempPtr->kursdaten.front().volume << endl;
                cout << "Open: " << tempPtr->kursdaten.front().open << endl;
                cout << "High: " << tempPtr->kursdaten.front().high << endl;
                cout << "--------------------------------" << endl;
            }
    

        }
        else if (command.substr(0, 4) == "PLOT") {
            std::string input;
            std::cout << "Name oder Kuerzel: ";
            getline(std::cin, input);
            
            Stock* stockPtr = htable.search(input);
            if (stockPtr == nullptr) {
                std::cerr << "Fehler: " << input << " nicht gefunden!" << std::endl;
                continue;
            }
            
            stockPtr->plotData();
        }
        else if (command.substr(0, 4) == "SAVE") {
            string filename;
            
            cout << "Dateiname: ";
            getline(cin, filename);

            if(filename.empty()) {
                cerr << "Fehler: Fehlerhafte Eingabe!" << endl;
                continue;
            }
            
            if(filename.size() < 4 || filename.substr(filename.size()-4) != ".csv") {
                filename += ".csv";
            }

            if(htable.save(filename)) {
                cout << "Daten in " << filename << " gespeichert." << endl;
            }
            else {
                cerr << "Fehler: Speichern in " << filename << " fehlgeschlagen! :(" << endl;
            }
        }
        else if (command.substr(0, 4) == "LOAD") {
            string filename;

            cout << "Dateiname: ";
            getline(cin, filename);

            if(filename.empty()) {
                cerr << "Fehler: Fehlerhafte Eingabe!" << endl;
                continue;
            }

            if(filename.size() < 4 || filename.substr(filename.size()-4) != ".csv") {
                filename += ".csv";
            }
            
            if(htable.load(filename)) {
                cout << "Daten aus " << filename << " geladen." << endl;
            }
            else {
                cerr << "Fehler: Laden aus " << filename << " fehlgeschlagen! :(" << endl;
            }
        }
        else if (command == "LIST") {
            htable.listStocks();
        }
        else if (command == "QUIT") {
                break;
        }
        else {
            cout << "Unbekannter Befehl.\n";
        }
    }
}
