//////////////////////////////////////////////
// ---- Pixelboard-Klasse (Deklaration) ----
// Zwei LED-Panels (je 32x8) uebereinander als EINE Flaeche 32x16 mit x/y-Koordinatensystem.
// Koordinaten: (0,0) = links oben, x nach rechts (0..31), y nach unten (0..15).
//   y 0..7  -> oberes Panel
//   y 8..15 -> unteres Panel
// Spiele verwenden nur diese Klasse und muessen nichts ueber Panels, Pins oder Drehung wissen.
//
// Benutzung:
//   Pixelboard board;                      // Standard-Aufbau (siehe documentation/hardware.md)
//   board.begin();                         // in setup(): oben GPIO 25, unten GPIO 26
//   board.setzePixel(5, 12, CRGB::Red);    // beliebig viele Pixel setzen ...
//   board.anzeigen();                      // ... und dann auf einmal anzeigen
//
// Jedes Panel ist ein LedPanel (Komposition). Implementierung: Pixelboard.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "LedPanel.h"

class Pixelboard {
  public:
    static const int BREITE = LedPanel::BREITE;      // 32
    static const int HOEHE = 2 * LedPanel::HOEHE;    // 16

    static const uint8_t PIN_OBEN_STANDARD = 25;
    static const uint8_t PIN_UNTEN_STANDARD = 26;
    static const uint8_t HELLIGKEIT_STANDARD = 20;   // 0..255, niedrig wegen Strom
    static const uint32_t MAX_STROM_MA = 500;        // Strombegrenzung durch FastLED (USB-tauglich)

  private:
    LedPanel oben;    // y 0..7
    LedPanel unten;   // y 8..15

  public:
    // Konstruktor: legt beide Panels an (noch kein Hardwarezugriff).
    // obenGedreht / untenGedreht = true, wenn das jeweilige Panel um 180° gedreht eingebaut ist.
    // Standard passt zum Aufbau: oben nicht gedreht, unten gedreht (Tests T12–T13a).
    Pixelboard(bool obenGedreht = false, bool untenGedreht = true);

    // Meldet beide Panels bei FastLED an und setzt Helligkeit (HELLIGKEIT_STANDARD) und
    // Strombegrenzung (MAX_STROM_MA). Einmal in setup() aufrufen:
    //   board.begin();            -> Standard-Pins 25 (oben) und 26 (unten)
    //   board.begin<27, 32>();    -> andere Pins
    // Pins als Template-Parameter, weil FastLED sie schon beim Kompilieren kennen muss.
    template<uint8_t PIN_OBEN = PIN_OBEN_STANDARD, uint8_t PIN_UNTEN = PIN_UNTEN_STANDARD>
    void begin() {
      oben.begin<PIN_OBEN>();
      unten.begin<PIN_UNTEN>();
      FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_STROM_MA);
      setHelligkeit(HELLIGKEIT_STANDARD);
      loeschen();
      anzeigen();
    }

    // Setzt die Helligkeit aller LEDs (0..255). Wirkt ab dem naechsten anzeigen().
    void setHelligkeit(uint8_t helligkeit);

    // Setzt die LED an Position (x, y) auf die angegebene Farbe.
    // Ausserhalb der Flaeche: wird ignoriert. Sichtbar erst nach anzeigen().
    void setzePixel(int x, int y, CRGB farbe);

    // Schaltet alle LEDs beider Panels aus (im Speicher). Sichtbar erst nach anzeigen().
    void loeschen();

    // Uebertraegt die Farbwerte an beide Panels.
    void anzeigen();
};
