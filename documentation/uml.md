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
