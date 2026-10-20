# UML-Klassendiagramme

Zurück zur [Übersicht](index.md).

## Klasse `Taster` (Vorjahr, Code gelöscht – nur noch als Referenz für den Umbau)

```mermaid
classDiagram
  class Taster {
    -uint8_t pin
    -bool aktuellerZustand
    -bool letzterZustand
    -unsigned long letzteAenderungZeit
    -unsigned long gedruecktZeit
    -bool wurdeKurz
    -bool wurdeLang
    -bool langMeldungSchonGesendet
    -const unsigned long entprellZeit = 50
    -const unsigned long langDruckZeit = 1000
    +Taster(uint8_t pinNr)
    +check() void
    +tasterGedrueckt() bool
    +tasterWurdeKurzGedrueckt() bool
    +tasterWurdeLangeGedrueckt() bool
  }
```

## Klasse `Joystick` (`src/Joystick.h`, `src/Joystick.cpp`)

Umbau der Klasse `Taster`: Tasterlogik übernommen (Pin heißt jetzt `pinSw`, Einlesen in private
Methode `tasterPruefen()` ausgelagert), ergänzt um zwei Achsen. Keine Vererbung, keine Komposition.

```mermaid
classDiagram
  class Joystick {
    -uint8_t pinSw
    -uint8_t pinX
    -uint8_t pinY
    -bool aktuellerZustand
    -bool letzterZustand
    -unsigned long letzteAenderungZeit
    -unsigned long gedruecktZeit
    -bool wurdeKurz
    -bool wurdeLang
    -bool langMeldungSchonGesendet
    -static const unsigned long ENTPRELL_ZEIT = 50
    -static const unsigned long LANG_DRUCK_ZEIT = 1000
    -int xWert
    -int yWert
    -tasterPruefen() void
    -achsenLesen() void
    +Joystick(uint8_t pinSwNr, uint8_t pinXNr, uint8_t pinYNr)
    +begin() void
    +check() void
    +tasterGedrueckt() bool
    +tasterWurdeKurzGedrueckt() bool
    +tasterWurdeLangeGedrueckt() bool
    +getX() int
    +getY() int
  }
```

Objekt im Testprogramm: `joystick(14, 34, 35)`.

## Klasse `LedPanel` (`src/LedPanel.h`, `src/LedPanel.cpp`)

Ein LED-Panel 32×8 (WS2812, Vertical Zig-Zag) mit x/y-Koordinatensystem: (0,0) = links oben, x nach rechts, y nach unten.

```mermaid
classDiagram
  class LedPanel {
    +static const int BREITE = 32
    +static const int HOEHE = 8
    +static const int ANZAHL = 256
    -CRGB leds[ANZAHL]
    -xyZuIndex(int x, int y) int
    +LedPanel()
    +begin~PIN~() void
    +setzePixel(int x, int y, CRGB farbe) void
    +loeschen() void
    +anzeigen() void
  }
```

Objekt im Testprogramm: `panel`, angemeldet mit `panel.begin<16>()`.

## Klasse `Laufschrift` (`src/Laufschrift.h`, `src/Laufschrift.cpp`) – Aufgabe 1

Lässt einen Text über **1 oder 2 Panels** laufen. Nutzt **LEDText** (Laufschrift, Schrift `FontMatrise` 5×7) und
zeichnet in einen `PanelStreifen` (eigene Unterklasse von `cLEDMatrixBase` aus LEDMatrix). Unabhängig von `LedPanel`.
Bei 2 Panels läuft der Text **zuerst über das obere Panel und dann unten weiter** (Streifen 64×8).

```mermaid
classDiagram
  class cLEDMatrixBase {
    <<LEDMatrix-Library>>
    #int16_t m_Width
    #int16_t m_Height
    #CRGB* m_LED
    +mXY(uint16_t x, uint16_t y)* uint16_t
  }
  class PanelStreifen {
    +static const int PANEL_BREITE = 32
    +static const int PANEL_HOEHE = 8
    +static const int LEDS_PRO_PANEL = 256
    +static const uint8_t MAX_PANELS = 2
    -CRGB leds[512]
    -uint8_t anzahlPanels
    -bool gedreht[2]
    +PanelStreifen(uint8_t anzahl)
    +setGedreht(uint8_t panelNr, bool istGedreht) void
    +getLeds(uint8_t panelNr) CRGB*
    +getAnzahlPanels() uint8_t
    +mXY(uint16_t x, uint16_t y) uint16_t
  }
  class Laufschrift {
    -PanelStreifen streifen
    -cLEDText text
    -static const uint16_t MAX_TEXT_LAENGE = 64
    -unsigned char textPuffer[65]
    -uint16_t textLaenge
    -uint8_t fuehrendeLeerzeichen
    -bool textGesetzt
    -unsigned long schrittZeit
    -unsigned long letzterSchrittZeit
    -bool durchlaufFertig
    +Laufschrift(unsigned long schrittZeitMs, uint8_t anzahlPanels)
    +getLeds(uint8_t panelNr) CRGB*
    +setGedreht(uint8_t panelNr, bool istGedreht) void
    +begin(uint8_t rot, uint8_t gruen, uint8_t blau) void
    +setText(const char* neuerText) void
    +aendereText(const char* neuerText) void
    +update() void
    +wurdeDurchlaufBeendet() bool
  }
  cLEDMatrixBase <|-- PanelStreifen : Vererbung
  Laufschrift *-- PanelStreifen : Komposition
```

### `PanelStreifen` (`src/PanelStreifen.h`, `src/PanelStreifen.cpp`)

Fasst die Panels zu einem Streifen zusammen. Koordinaten wie bei LEDMatrix/LEDText: x nach rechts, **y = 0 unten**.

| Streifen-x | Panel | Data-Pin (Aufgabe 1) |
|---|---|---|
| 0..31 | Panel 0 = **unten** | GPIO 26 |
| 32..63 | Panel 1 = **oben** (180° gedreht) | GPIO 25 |

`mXY(x, y)` – Umrechnung Streifen-Koordinate → Index im LED-Speicher:
1. Panel = x / 32, lokales x = x % 32
2. `yOben = 7 − y` (LEDText zählt von unten, die Verdrahtung von oben)
3. Panel gedreht (180°): `lokalX = 31 − lokalX`, `yOben = 7 − yOben`
4. Zick-Zack wie `LedPanel::xyZuIndex` (T7): gerade Spalte `x·8 + yOben`, ungerade `x·8 + (7 − yOben)`
5. Index = Panel · 256 + Ergebnis

Ungedreht ergibt das für jedes Pixel denselben Index wie vorher `cLEDMatrix<32, −8, VERTICAL_ZIGZAG_MATRIX>`.

### Methoden `Laufschrift`

| Methode | Beschreibung |
|---|---|
| `Laufschrift(schrittZeitMs, anzahlPanels)` | Speichert Geschwindigkeit (ms pro Spalte) und legt den Streifen an (1 oder 2 Panels). Kein Hardwarezugriff. |
| `getLeds(panelNr)` | LED-Speicher von Panel 0 (unten) bzw. 1 (oben) für `FastLED.addLeds<WS2812B, PIN, GRB>(…, 256)`. |
| `setGedreht(panelNr, gedreht)` | Panel als um 180° gedreht markieren. |
| `begin(rot, gruen, blau)` | Schriftart, Textbereich (ganzer Streifen, 32 oder 64 breit) und Textfarbe setzen. Einmal nach `addLeds`. |
| `setText(text)` | Kopiert den Text (mit führenden Leerzeichen: 6 bei 1 Panel, 11 bei 2 Panels; max. 64 Zeichen) in den Puffer, startet neu. |
| `aendereText(text)` | Tauscht den Text **während** des Durchlaufs aus (kein Neustart). Länge bleibt gleich: kürzer → mit Leerzeichen aufgefüllt, länger → abgeschnitten. |
| `update()` | Alle `schrittZeit` ms eine Spalte weiter + `FastLED.show()` (sendet an alle Panels). Nicht blockierend. |
| `wurdeDurchlaufBeendet()` | Einmalig `true`, wenn der Text komplett durchgelaufen ist. |

Objekt in Aufgabe 1: `laufschrift(50, ANZAHL_PANELS)`; unten GPIO 26, oben GPIO 25 (gedreht).

## Klasse `Snake` (`src/Snake.h`, `src/Snake.cpp`) – Aufgabe 3

Einfaches Snake-Spiel auf **einem** Panel. Nutzt das x/y-Koordinatensystem von `LedPanel`
(`zeichnen()` bekommt ein `LedPanel&` übergeben), kein eigener LED-Speicher.

Hilfsstruktur `Punkt` mit `x` und `y`.

```mermaid
classDiagram
  class Punkt {
    +int x
    +int y
  }

  class Snake {
    +static const int MAX_KOERPER = 256
    -Punkt koerper[256]
    -int laenge
    -int richtung
    -int naechsteRichtung
    -Punkt futter
    -bool futterVorhanden
    -bool gameOver
    -bool gestartet
    -unsigned long letzteBewegung
    -unsigned long geschwindigkeitMs
    -int breite
    -int hoehe
    -erzeugeFutter() void
    +Snake(int b, int h)
    +init(int startX, int startY) void
    +setzeRichtung(int neueRichtung) void
    +bewegen() void
    +zeichnen(LedPanel& panel, CRGB schlangeFarbe, CRGB futterFarbe) void
    +istGameOver() bool
    +istGestartet() bool
    +getLaenge() int
    +getGeschwindigkeitMs() unsigned long
    +setzeGeschwindigkeitMs(unsigned long v) void
  }

  Snake --> LedPanel : zeichnet auf
  Snake --> Punkt : Koerper und Futter
```

| Methode | Beschreibung |
|---|---|
| `Snake(b, h)` | Legt Spielfeldgröße fest (Standard 32×8). Kein Hardwarezugriff, noch kein Spielstart. |
| `init(startX, startY)` | Startet neu: 3 Segmente waagerecht, Kopf bei `(startX, startY)`, Richtung rechts, Futter wird zufällig gesetzt. |
| `setzeRichtung(r)` | 0 = rechts, 1 = unten, 2 = links, 3 = oben. Direkte Umkehr wird ignoriert. |
| `bewegen()` | Nicht blockierend: bewegt die Schlange alle `geschwindigkeitMs`. Prüft Wand- und Selbstkollision, frisst Futter (Schlange wächst, wird ab 80 ms etwas schneller). |
| `zeichnen(panel, …)` | Zeichnet Schlange (grün) und Futter (orange) über `setzePixel()`; nichts sichtbar bis `panel.anzeigen()`. |
| `istGameOver()` | `true` nach Wand- oder Selbstkollision. |

Objekt in Aufgabe 3: `snake(32, 8)`; Steuerung über `Joystick` (Schwellen 1500/2500, Totzone dazwischen),
Neustart nach Game Over per kurzem Tastendruck.
