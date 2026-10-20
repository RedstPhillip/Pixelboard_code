//////////////////////////////////////////////
// ---- PanelStreifen-Klasse (Deklaration) ----
// Fasst 1 oder 2 LED-Panels (je 32x8, WS2812, Vertical Zig-Zag) zu EINEM langen Streifen zusammen,
// damit LEDText eine Laufschrift darueber zeichnen kann (wird von der Klasse Laufschrift verwendet).
//
// Streifen-Koordinaten (wie LEDMatrix/LEDText): x nach rechts, y = 0 unten, y = 7 oben.
//   x  0..31 -> Panel 0 (unteres Panel)
//   x 32..63 -> Panel 1 (oberes Panel)
// Ein Text, der von rechts nach links laeuft, kommt also zuerst oben und laeuft dann unten weiter.
//
// Jedes Panel hat einen eigenen LED-Speicher-Bereich (eigener Data-Pin) und kann um 180° gedreht sein.
// Verdrahtung siehe documentation/hardware.md, Implementierung: PanelStreifen.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>

class PanelStreifen : public cLEDMatrixBase {
  public:
    static const int PANEL_BREITE = 32;
    static const int PANEL_HOEHE = 8;
    static const int LEDS_PRO_PANEL = PANEL_BREITE * PANEL_HOEHE;
    static const uint8_t MAX_PANELS = 2;

  private:
    CRGB leds[MAX_PANELS * LEDS_PRO_PANEL];  // Panel 0: Index 0..255, Panel 1: Index 256..511
    uint8_t anzahlPanels;                    // 1 oder 2
    bool gedreht[MAX_PANELS];                // true: Panel ist um 180° gedreht eingebaut

  public:
    // Konstruktor: legt einen Streifen aus anzahl Panels an (1 oder 2, wird darauf begrenzt),
    // alle LEDs schwarz, kein Panel gedreht. Noch kein Hardwarezugriff.
    PanelStreifen(uint8_t anzahl);

    // Legt fest, ob Panel panelNr (0 = unten, 1 = oben) um 180° gedreht eingebaut ist.
    // Ungueltige panelNr wird ignoriert.
    void setGedreht(uint8_t panelNr, bool istGedreht);

    // Liefert den Anfang des LED-Speichers von Panel panelNr fuer FastLED.addLeds() (256 LEDs ab hier).
    CRGB* getLeds(uint8_t panelNr);

    // Liefert die Anzahl der Panels im Streifen.
    uint8_t getAnzahlPanels();

    // Rechnet eine Streifen-Koordinate (x, y mit y = 0 unten) in den Index im LED-Speicher um.
    // Wird von LEDMatrix/LEDText fuer jedes Pixel aufgerufen (Koordinaten sind dort schon geprueft).
    virtual uint16_t mXY(uint16_t x, uint16_t y) override;
};
