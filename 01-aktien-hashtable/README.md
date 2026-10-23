# Aktienverwaltung mit Hashtabelle

Konsolenprogramm zur Verwaltung von Aktien und ihren Kursdaten auf Basis einer selbst implementierten Hashtabelle.

## Aufgabe

Ein Programm zur Verwaltung von Aktienkursen, bei dem keine fertige Hashtable-Klasse verwendet werden darf, sondern die Hashtabelle **selbst implementiert** werden muss.

- Aktien mit Name, WKN und Kürzel anlegen und löschen
- Kursdaten der letzten 30 Tage (Date, Close, Volume, Open, High, Low) aus einer CSV-Datei importieren (Format von nasdaq.com)
- Eine Aktie wahlweise über ihren Namen oder ihr Kürzel finden und den aktuellsten Kurs anzeigen
- Die Schlusskurse der letzten 30 Tage als ASCII-Grafik plotten
- Die gesamte Tabelle in eine Datei speichern und wieder laden

Vorgaben zur Datenstruktur: eine selbst überlegte Hashfunktion, quadratische Sondierung als Kollisionsbehandlung, ein effizientes Löschverfahren und eine Tabellengröße, die für maximal 1000 Aktien passt. Außerdem sollte der Aufwand mit einem normalen Array und einer verketteten Liste verglichen werden.

## Umsetzung

### Zwei Tabellen: Hashtabelle + Mapping-Tabelle

```
Name ──hash──▶ HashTable    [ Status | Stock{name, wkn, kürzel, kursdaten[]} ]
                    ▲
Kürzel ─hash─▶ MappingTable [ Status | kürzel | index in HashTable ]
```

- Die **HashTable** speichert die Aktien mit dem Namen als Schlüssel.
- Die **MappingTable** bildet das Kürzel auf die Position in der HashTable ab. So lässt sich eine Aktie über beide Schlüssel finden, ohne das Objekt doppelt abzulegen.
- `search()` probiert zuerst den Namen und fällt dann auf das Kürzel zurück. Der Benutzer muss also nicht angeben, was er eingibt.

### Hashfunktion

```cpp
hash(key) = ( Σ  ascii(key[i])^i  mod M )  mod M     
```

`i` ist die Position des Zeichens (ab 1) und M = 1657 ist die Tabellengröße. Durch die Potenz pro Position ergibt sich für Anagramme (z. B. `"ABC"` / `"CBA"`) ein anderer Wert. Bei einer reinen ASCII-Summe wäre das nicht so.

### Kollisionsbehandlung: alternierende quadratische Sondierung

```
pos_i = (h + i²)  für gerade i
pos_i = (h − i²)  für ungerade i
```

Gelöschte Einträge werden nicht geleert, sondern als `DEL` markiert, damit Sondierungsketten nicht abreißen.

### Warum so?

| Entscheidung | Begründung |
|---|---|
| Offene Adressierung statt Verkettung | Alles in einem zusammenhängenden `std::vector`, keine Zeiger, einfaches Speichern/Laden |
| Tabellengröße **1657** (Primzahl) | Bei maximal 1000 Aktien bleibt der Füllgrad unter ~60 %, sodass Sondierungsketten kurz bleiben. Eine Primzahl verteilt die Hashwerte gleichmäßiger |
| Kursdaten als `std::vector<KursDaten>` | Neuester Eintrag vorne: `SEARCH` liest `front()` in O(1), `PLOT` läuft die 30 Einträge einmal durch |
| Separate Mapping-Tabelle | Zweiter Suchschlüssel in O(1), ohne Daten zu duplizieren |
| Status `OCC` / `DEL` / `EMP` | Löschen ohne die Suche nach anderen Elementen zu zerstören |

## Laufzeit

| Operation | Durchschnitt | Worst Case |
|---|---|---|
| `add`, `search`, `del` | O(1) | O(M) bei vielen Kollisionen |
| `save`, `load`, `list` | O(M + Kursdaten) | |
| `plot` | O(Anzahl Kurstage) | |

Vergleich für Einfügen, Suchen und Löschen bei n Aktien:

| Datenstruktur | Einfügen | Suchen | Löschen |
|---|---|---|---|
| Unsortiertes Array | O(1) am Ende | O(n) | O(n), Suche + Verschieben |
| Einfach verkettete Liste | O(1) am Anfang | O(n) | O(n), Suche |
| **Hashtabelle** (niedriger Füllgrad) | **O(1)** | **O(1)** | **O(1)** |

Bei 1000 Aktien braucht eine Suche im Array oder in der Liste im Schnitt ~500 Vergleiche. In der Hashtabelle sind es bei einem Füllgrad von ~60 % nur wenige Sondierungsschritte.

## Bauen & Starten

```bash
g++ -std=c++17 -Iinclude main.cpp src/*.cpp -o aktien
./aktien
```

Beispiel-Sitzung (Befehle sind interaktiv):

```
Befehl eingeben: ADD
Name: Apple
Kuerzel: AAPL
WKN: 865985
CSV-Datei (name.csv): data/apple.csv
Aktie von Apple hinzugefuegt.

Befehl eingeben: PLOT
Name oder Kuerzel: Apple
    *
******
******* *    *
**************
**************
***************
***************
****************
*****************
******************
Min: 209.68, Max: 247.1
```

Befehle: `ADD`, `DEL`, `IMPORT`, `SEARCH`, `PLOT`, `SAVE`, `LOAD`, `LIST`, `QUIT`

**Testdaten** in [`data/`](data/): Kursdaten von Apple, Meta und Tesla (Format wie beim Export von nasdaq.com) sowie `ALLDATA.csv`, ein mit `SAVE` erzeugter Stand, der mit `LOAD` wieder geladen werden kann.

## Was wir heute anders machen würden

- **Mapping-Tabelle speichert den Hashwert statt der tatsächlichen Position.** Nach einer Kollision zeigt das Kürzel daher auf den falschen Slot. Richtig wäre, die nach dem Sondieren gefundene Position zu speichern.
- **Hashfunktion:** `pow(ascii, i)` überschreitet ab etwa 5 Zeichen den `int`-Bereich. Besser wäre das Horner-Schema mit Modulo in jedem Schritt: `h = (h * 31 + c) % M`.
- **Sondierung:** Bei `M ≡ 1 (mod 4)` erreicht die alternierende Sondierung von einem Startplatz aus nur etwa die Hälfte der Plätze (~829). Bei den geforderten 1000 Aktien kann ein Einfügen deshalb scheitern, obwohl noch Platz frei ist. 1657 ist zwar eine Primzahl, für die alternierende Sondierung müsste sie aber bei Division durch 4 den Rest 3 haben (z. B. 1663). Dann wären alle Plätze erreichbar.
