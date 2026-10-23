# Kürzester Weg im Wiener Öffi-Netz

Findet die schnellste Verbindung zwischen zwei Stationen in einem Verkehrsnetz und zeigt Linien, Umstiege und Gesamtfahrzeit an.

## Aufgabe

- Ein Verkehrsnetz aus einer Textdatei einlesen, eine Linie pro Zeile:
  ```
  U1: "Leopoldau" 2 "Grossfeldsiedlung" 1 "Aderklaaer Strasse" 1 "Rennbahnweg" ...
  ```
  Die Zahl zwischen zwei Stationen ist die Fahrzeit in Minuten (= Kantengewicht). Linien können auch zyklisch sein, also dieselbe Station mehrfach anfahren.
- Das Netz darf nicht hardcodiert sein, das Programm soll mit jeder Datei in diesem Format funktionieren.
- Den kostengünstigsten Weg von Start nach Ziel finden und ausgeben: befahrene Kanten, verwendete Linien, Umstiege und Gesamtfahrzeit.

## Umsetzung

### Datenstruktur: Adjazenzliste

```cpp
unordered_map<string, vector<Connection>> net;   // Station → ausgehende Verbindungen
struct Connection { string destination; int duration; string lane; };
```

- Jede Station ist ein Knoten. Jede Fahrt zwischen zwei benachbarten Stationen wird als zwei gerichtete Kanten eingetragen (hin und zurück).
- Wird eine Station von mehreren Linien angefahren, hat sie Kanten aller Linien. Umstiege ergeben sich dadurch automatisch, ohne Sonderbehandlung.
- Stationsnamen werden in Kleinbuchstaben gespeichert, damit die Eingabe nicht von der Groß-/Kleinschreibung abhängt.
- Das Netz wird nur neu eingelesen, wenn sich der Dateiname ändert. Mehrere Abfragen hintereinander sind daher schnell.

### Algorithmus: Dijkstra

1. Start als besucht markieren, alle Nachbarn mit ihrer Fahrzeit in eine `ToVisit`-Liste legen
2. Liste nach Gesamtfahrzeit sortieren und die günstigste Station als nächste nehmen
3. Deren Nachbarn eintragen. Ist eine Station schon in der Liste, wird sie nur ersetzt, wenn der neue Weg kürzer ist
4. Wiederholen, bis das Ziel an der Reihe ist

Zu jedem Eintrag wird der bisher gefahrene Weg (`pastWay`) mitgeführt. Am Ziel liegt damit der komplette Pfad inklusive Linie jeder Kante vor, und Umstiege lassen sich direkt am Linienwechsel erkennen.

### Warum so?

| Entscheidung | Begründung |
|---|---|
| Dijkstra | Alle Gewichte sind positiv (Fahrzeit > 0), daher liefert Dijkstra garantiert den kürzesten Weg. Fahrzeit 0 wird beim Einlesen als Fehler abgelehnt |
| Adjazenzliste statt Matrix | Das Netz ist dünn besetzt (jede Station hat nur wenige Nachbarn), eine Matrix wäre zum Großteil leer |
| `unordered_map` mit Stationsnamen | Direkter Zugriff in O(1) über den Namen, ohne Namen erst auf Indizes abbilden zu müssen |
| Linie an jeder Kante | Ermöglicht die geforderte Ausgabe von Linien und Umstiegen |

## Laufzeit

V = Stationen, E = Verbindungen

| Schritt | Laufzeit |
|---|---|
| Einlesen | O(E) |
| Suche (unsere Variante mit sortiertem `vector`) | O(V · (V log V + E)) im Worst Case: pro Schritt wird die ganze Liste sortiert und linear durchsucht |
| Zum Vergleich: Dijkstra mit Priority Queue | O((V + E) log V) |

Für das Wiener Netz (einige hundert Stationen) ist das in Millisekunden erledigt.

## Bauen & Starten

```bash
g++ -std=c++17 -Iinclude main.cpp src/*.cpp -o find_path
./find_path
```

Eingabe: `<datei> "Start" "Ziel"`, Beenden mit `0`

```
data/wvn.txt "Leopoldau" "Westbahnhof"
Start: leopoldau
Linie U1: -> grossfeldsiedlung -> aderklaaer strasse -> rennbahnweg -> kagraner platz
          -> kagran -> alte donau -> kaisermuehlen-vic -> donauinsel -> vorgartenstrasse
          -> praterstern -> nestroyplatz -> schwedenplatz -> stephansplatz
Umsteigen zu U3: -> herrengasse -> volkstheater -> neubaugasse -> zieglergasse -> westbahnhof
Fahrzeit: 25 Min
```

[`data/wvn.txt`](data/wvn.txt) enthält das Wiener U-Bahn- und Straßenbahnnetz.

## Was wir heute anders machen würden

- **`std::priority_queue` statt sortiertem `vector`.** Das bringt die Laufzeit auf O((V + E) log V).
- **Vorgänger speichern statt ganzen Pfad kopieren.** Ein `predecessor`-Map pro Station spart das Kopieren von `pastWay` bei jedem Eintrag. Der Pfad wird am Ende rückwärts rekonstruiert.
- **Umstiegszeit berücksichtigen.** Aktuell kostet ein Linienwechsel nichts, in der Realität dauert er einige Minuten.
