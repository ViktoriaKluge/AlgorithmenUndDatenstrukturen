# Treecheck: Binärer Suchbaum & AVL-Prüfung

Konsolenprogramm, das aus einer Datei einen binären Suchbaum aufbaut, prüft, ob er die AVL-Bedingung erfüllt, und darin nach Werten oder ganzen Teilbäumen sucht.

## Aufgabe

Die Baumstruktur muss selbst implementiert werden und die Auswertung über rekursive Funktionen erfolgen.

**Teil 1: `treecheck <datei>`**
- Ganzzahlen (eine pro Zeile) der Reihe nach in einen binären Suchbaum einfügen. Duplikate werden verworfen
- Für jeden Knoten den Balance-Faktor `bal(k) = h(rechts) − h(links)` ausgeben und Verletzungen der AVL-Bedingung (|bal| > 1) markieren
- Ausgeben, ob der Baum ein AVL-Baum ist, sowie Minimum, Maximum und Durchschnitt. Diese Werte müssen durch Traversieren des Baums ermittelt werden, nicht aus der Eingabedatei
- Das Ausgabeformat war exakt vorgegeben (`bal(key) = x (AVL violation!)`, `AVL: yes/no`, `min: x, max: y, avg: z`)

**Teil 2: `treecheck <datei> <suchdatei>`**
- Enthält die Suchdatei nur einen Wert: ausgeben, ob er gefunden wurde und alle Knoten auf dem Pfad von der Wurzel dorthin
- Enthält sie mehrere Werte: prüfen, ob die Struktur des daraus gebauten Baums im Hauptbaum vorkommt
- In beiden Fällen muss im aufgebauten Baum gesucht werden, nicht in der Eingabeliste

## Umsetzung

Der Baum besteht aus klassischen Knoten mit `value`, `left` und `right`. Alle Operationen sind rekursiv umgesetzt, weil sich die Baumstruktur so direkt im Code widerspiegelt.

| Funktion | Vorgehen |
|---|---|
| `insert` | Rekursiv links/rechts absteigen bis `nullptr`, dort neuen Knoten anlegen. Übergabe als `Node*&`, damit der Zeiger im Elternknoten selbst verändert wird |
| `calcBalance` | Liefert die Höhe eines Teilbaums. Auf dem Rückweg wird `bal = h(rechts) − h(links)` berechnet und bei \|bal\| > 1 eine Verletzung gemeldet |
| `minVal` / `maxVal` | Immer ganz nach links bzw. rechts absteigen, dank Suchbaum-Eigenschaft |
| `countNode` / `sumVal` | Vollständige Traversierung für den Durchschnitt |
| `searchValue` / `printWay` | Binäre Suche, der Pfad wird beim Absteigen ausgegeben |
| `checkSubtree` | Wurzel des Suchbaums im Hauptbaum per binärer Suche finden, dann beide Bäume parallel vergleichen |

### Warum so?

- **AVL-Prüfung in einem Durchlauf:** `calcBalance` berechnet Höhe und Balance gleichzeitig (Post-Order). Würde man die Höhe für jeden Knoten getrennt berechnen, käme man auf O(n²).
- **Subtree-Suche nutzt die Suchbaum-Eigenschaft:** Statt jeden Knoten als mögliche Wurzel zu testen, wird die Wurzel des Teilbaums gezielt gesucht.
- **Ein Wert vs. mehrere Werte** wird automatisch erkannt (Suchbaum hat keine Kinder). Es braucht also nur einen Befehl.

## Laufzeit

n = Knoten im Hauptbaum, m = Knoten im Suchbaum, h = Höhe (AVL: log n, entartet: n)

| Operation | Laufzeit |
|---|---|
| `insert`, `searchValue`, `printWay`, `minVal`, `maxVal` | O(h) |
| Baum aus Datei aufbauen | O(n · h) |
| `calcBalance`, `countNode`, `sumVal` | O(n), jeder Knoten genau einmal |
| `checkSubtree` | O(m · h): für jeden der m Knoten im Suchbaum höchstens ein Abstieg im Hauptbaum |

## Bauen & Starten

```bash
g++ -std=c++17 -Iinclude main.cpp src/*.cpp -o treecheck
./treecheck
```

Beispiel mit den Testdateien aus [`data/`](data/):

```
> treecheck data/tree2
bal(22) = 0
bal(7) = 0
bal(8) = 0
bal(2) = 0
bal(3) = -1
bal(5) = 0
AVL: yes
min: 2, max: 22, avg: 7.83333

> treecheck data/tree1
...
bal(23) = 2 (AVL violation!)
bal(9) = 2 (AVL violation!)
bal(5) = 3 (AVL violation!)
AVL: no
min: 3, max: 79, avg: 24.3

> treecheck data/tree1 data/val
23 found 5, 17, 23

> treecheck data/tree1 data/stree2
Subtree found!
```

| Datei | Inhalt |
|---|---|
| `tree1` | Unbalancierter Baum (10 Knoten) |
| `tree2` | AVL-Baum (6 Knoten) |
| `val` | Einzelner Suchwert |
| `stree1` | Teilbaum, der **nicht** in `tree1` vorkommt |
| `stree2` | Teilbaum, der in `tree1` vorkommt |

## Was wir heute anders machen würden

- Knoten werden mit `new` angelegt, aber nie freigegeben. Mit `std::unique_ptr` wäre das automatisch gelöst.
- Wir sind von positiven Schlüsseln ausgegangen und haben `-1` als Markierung für einen leeren Baum verwendet. Für beliebige Integer wäre `nullptr` als leerer Baum die sauberere Lösung.