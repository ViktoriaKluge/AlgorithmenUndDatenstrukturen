#include "Stock.h"

#include <iostream>
#include <fstream>
#include <sstream>

Stock::Stock() {}
Stock::~Stock() {}

Stock::Stock(std::string &name, std::string &wkn, std::string &key) : name(name), wkn(wkn), key(key) {}

void Stock::printData() const {
    if (kursdaten.empty()) {
        std::cout << "Keine Daten vorhanden." << std::endl;

    }
    const KursDaten &kd = kursdaten.front();

    std::cout   << "--------------------------------" << std::endl
                << "Name:   " << name << std::endl
                << "Datum:  " << kd.date << std::endl
                << "Close:  " << kd.close << std::endl
                << "Volume: " << kd.volume << std::endl
                << "Open:   " << kd.open << std::endl
                << "High:   " << kd.high << std::endl
                << "Low:    " << kd.low << std::endl
                << "--------------------------------" << std::endl;
}

bool Stock::import(std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Fehler beim Oeffnen der Datei " << filename << std::endl;
        return false;
    }
    kursdaten.clear();
    std::string line;
    std::getline(file, line);   // Headerzeile überspringen (Tableheader)
    while(std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        KursDaten kd;
        std::string token;
        
        std::getline(ss, kd.date, ',');     // liest string bis zum Komma, speichert alles davor in kd.date
        std::getline(ss, token, ',');       // Close/Last (entferne führendes $)
        if (!token.empty() && token[0] == '$') token = token.substr(1);
        kd.close = std::stod(token);        // stod = string to double

        std::getline(ss, token, ',');       // Volume
        kd.volume = std::stol(token);       // stol = string to long

        std::getline(ss, token, ',');       // Open
        if (!token.empty() && token[0] == '$') token = token.substr(1);
        kd.open = std::stod(token);

        std::getline(ss, token, ',');       // High
        if (!token.empty() && token[0] == '$') token = token.substr(1);
        kd.high = std::stod(token);

        std::getline(ss, token, ',');       // Low
        if (!token.empty() && token[0] == '$') token = token.substr(1);
        kd.low = std::stod(token);

        kursdaten.push_back(kd);
    }
    file.close();
    return true;
}

void Stock::plotData() const {
    if (kursdaten.empty()) {
        std::cout << "Keine Daten zum Plotten vorhanden." << std::endl;
        return;
    }

    // 1) Minimal- und Maximalwert ermitteln
    double minClose = kursdaten.front().close;
    double maxClose = kursdaten.front().close;
    for (const auto &kd : kursdaten) {
        if (kd.close < minClose) minClose = kd.close;
        if (kd.close > maxClose) maxClose = kd.close;
    }

    // 2) Plot-Höhe festlegen
    const int plotHeight = 10;
    int plotWidth = kursdaten.size();

    double range = maxClose - minClose;
    if (range == 0) range = 1;

    // 3) 2D-Gitter erzeugen
    std::vector<std::string> grid(plotHeight, std::string(plotWidth, ' '));

    // 4) Balken zeichnen: von links nach rechts sollen die ältesten Daten kommen,
    //    obwohl intern die neuesten vorne liegen. Deshalb iterieren wir rückwärts.
    for (int i = 0; i < plotWidth; i++) {
        // Der Index iPlot zeigt vom Ende des Vektors zum Anfang
        int iPlot = (plotWidth - 1) - i;
        double normalized = (kursdaten[iPlot].close - minClose) / range;
        int barHeight = static_cast<int>(normalized * (plotHeight - 1)) + 1;

        // Von unten nach oben füllen
        for (int row = plotHeight - 1; row >= plotHeight - barHeight; row--) {
            grid[row][i] = '*';
        }
    }

    // 5) Ausgabe
    for (const auto &line : grid) {
        std::cout << line << std::endl;
    }

    // 6) Legende
    std::cout << "Min: " << minClose << ", Max: " << maxClose << std::endl;
}
