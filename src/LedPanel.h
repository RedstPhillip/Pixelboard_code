//////////////////////////////////////////////
// ---- LedPanel-Klasse (Deklaration) ----
// Ein LED-Panel 32x8 (WS2812, Vertical Zig-Zag) mit x/y-Koordinatensystem.
// Koordinaten: (0,0) = links oben, x nach rechts (0..31), y nach unten (0..7).
// Verdrahtung siehe documentation/hardware.md, Implementierung: LedPanel.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>

class LedPanel {
  public:
    static const int BREITE = 32;
    static const int HOEHE = 8;
    static const int ANZAHL = BREITE * HOEHE;

  private:
    CRGB leds[ANZAHL];  // Farbwerte in Kabel-Reihenfolge (Index 0 = erste LED am Data-Eingang)

    // Rechnet eine x/y-Koordinate in den LED-Index (0..255) um.
    // Liefert -1, wenn die Koordinate ausserhalb des Panels liegt.
    int xyZuIndex(int x, int y);

  public:
    // Konstruktor: setzt alle LEDs im Speicher auf schwarz (noch kein Hardwarezugriff).
    LedPanel();

    // Meldet das Panel bei FastLED an. Einmal in setup() aufrufen, z. B. panel.begin<16>();
    // Der Pin ist ein Template-Parameter, weil FastLED ihn schon beim Kompilieren kennen muss.
    template<uint8_t PIN>
    void begin() {
      FastLED.addLeds<WS2812B, PIN, GRB>(leds, ANZAHL);
    }

    // Setzt die LED an Position (x, y) auf die angegebene Farbe. Ausserhalb des Panels: wird ignoriert.
    // Sichtbar erst nach anzeigen().
    void setzePixel(int x, int y, CRGB farbe);

    // Schaltet alle LEDs aus (im Speicher). Sichtbar erst nach anzeigen().
    void loeschen();

    // Uebertraegt die Farbwerte an die LEDs (FastLED.show() – aktualisiert alle angemeldeten Panels).
    void anzeigen();
};
