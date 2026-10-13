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

Lässt einen Text über **ein** Panel laufen. Nutzt **LEDMatrix** (Koordinaten → LED-Nummer, Vertical Zig-Zag) und
**LEDText** (Laufschrift, Schrift `FontMatrise` 5×7). Unabhängig von `LedPanel` (eigener LED-Speicher in `matrix`).
`matrix` ist vom Typ `cLEDMatrix<PANEL_BREITE, PANEL_HOEHE, VERTICAL_ZIGZAG_MATRIX>` mit 32 und −8
(negative Höhe = LED 0 links oben, passend zu T7).

```mermaid
classDiagram
  class Laufschrift {
    -cLEDMatrix matrix
    -cLEDText text
    -static const uint16_t MAX_TEXT_LAENGE = 64
    -unsigned char textPuffer[65]
    -uint16_t textLaenge
    -bool textGesetzt
    -unsigned long schrittZeit
    -unsigned long letzterSchrittZeit
    -bool durchlaufFertig
    +Laufschrift(unsigned long schrittZeitMs)
    +getLeds() CRGB*
    +getAnzahlLeds() int
    +begin(uint8_t rot, uint8_t gruen, uint8_t blau) void
    +setText(const char* neuerText) void
    +aendereText(const char* neuerText) void
    +update() void
    +wurdeDurchlaufBeendet() bool
  }
```

| Methode | Beschreibung |
|---|---|
| `Laufschrift(schrittZeitMs)` | Speichert die Geschwindigkeit (ms pro Spalte). Kein Hardwarezugriff. |
| `getLeds()` / `getAnzahlLeds()` | LED-Speicher bzw. Anzahl (256) für `FastLED.addLeds<WS2812B, PIN, GRB>(…)` in `setup()`. |
| `begin(rot, gruen, blau)` | Schriftart, Textbereich (ganzes Panel) und Textfarbe setzen. Einmal nach `addLeds`. |
| `setText(text)` | Kopiert den Text (6 führende Leerzeichen, max. 64 Zeichen) in den eigenen Puffer, startet den Durchlauf neu. |
| `aendereText(text)` | Tauscht den Text **während** des Durchlaufs aus (kein Neustart). Länge bleibt gleich: kürzer → mit Leerzeichen aufgefüllt, länger → abgeschnitten. Geht, weil LEDText die sichtbaren Zeichen bei jedem Schritt neu aus dem Puffer liest. |
| `update()` | Alle `schrittZeit` ms eine Spalte weiter + `FastLED.show()`. Nicht blockierend, in jedem `loop()` aufrufen. |
| `wurdeDurchlaufBeendet()` | Einmalig `true`, wenn der Text komplett durchgelaufen ist. |

Objekt in Aufgabe 1: `laufschrift(50)`, angemeldet an GPIO 16.
