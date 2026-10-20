//////////////////////////////////////////////
// ---- Laufschrift-Klasse (Deklaration) ----
// Laesst einen Text ueber 1 oder 2 LED-Panels (WS2812, 8x32, vertikales Zick-Zack) laufen.
// Bei 2 Panels laeuft der Text zuerst ueber das obere Panel und danach ueber das untere weiter
// (beide Panels bilden einen 64 Spalten langen Streifen, siehe PanelStreifen.h).
// Bibliotheken: FastLED, LEDMatrix und LEDText (Aaron Liddiment), siehe platformio.ini
// Verdrahtung siehe documentation/hardware.md, wird in src/aufgabe1/main.cpp verwendet
// Implementierung: Laufschrift.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include "PanelStreifen.h"

class Laufschrift {
  private:
    PanelStreifen streifen;  // LED-Speicher + Umrechnung x/y -> LED (1 oder 2 Panels)
    cLEDText text;           // Laufschrift-Logik (LEDText)

    static const uint16_t MAX_TEXT_LAENGE = 64;      // max. Zeichen inkl. fuehrender Leerzeichen
    unsigned char textPuffer[MAX_TEXT_LAENGE + 1];   // eigene Kopie des Textes (LEDText merkt sich nur den Zeiger)
    uint16_t textLaenge;                             // aktuelle Laenge im Puffer
    uint8_t fuehrendeLeerzeichen;                    // Leerzeichen vor dem Text (Streifenbreite / 6 + 1)
    bool textGesetzt;                                // true, sobald setText() einmal aufgerufen wurde

    unsigned long schrittZeit;         // ms pro Verschiebung um 1 Spalte (kleiner = schneller)
    unsigned long letzterSchrittZeit;  // Zeitpunkt der letzten Verschiebung
    bool durchlaufFertig;              // true, wenn der Text komplett durchgelaufen ist

  public:
    // Konstruktor: speichert Laufgeschwindigkeit und Panelanzahl (noch kein Hardwarezugriff).
    // schrittZeitMs = Zeit in ms, nach der der Text um eine Spalte weiterlaeuft.
    // anzahlPanels  = 1 (nur unteres Panel) oder 2 (oben, dann unten).
    Laufschrift(unsigned long schrittZeitMs, uint8_t anzahlPanels);

    // Liefert den LED-Speicher von Panel panelNr (0 = unten, 1 = oben) fuer FastLED.addLeds() in setup().
    CRGB* getLeds(uint8_t panelNr);

    // Legt fest, ob Panel panelNr (0 = unten, 1 = oben) um 180° gedreht eingebaut ist. Vor setText() aufrufen.
    void setGedreht(uint8_t panelNr, bool istGedreht);

    // Stellt Schriftart und Textfarbe ein. Einmal in setup() nach FastLED.addLeds() aufrufen.
    // rot/gruen/blau = Textfarbe (0..255)
    void begin(uint8_t rot, uint8_t gruen, uint8_t blau);

    // Uebernimmt einen neuen Text und startet den Durchlauf von vorne (Streifen rechts leer).
    // Texte laenger als MAX_TEXT_LAENGE werden abgeschnitten.
    void setText(const char* neuerText);

    // Tauscht den Text waehrend des Durchlaufs aus, ohne neu zu starten (z. B. hochzaehlende Zahl).
    // Die Laenge bleibt die des letzten setText(): kuerzere Texte werden mit Leerzeichen aufgefuellt,
    // laengere abgeschnitten. Ohne vorheriges setText() wirkungslos.
    void aendereText(const char* neuerText);

    // Schiebt den Text nach Ablauf der Schrittzeit um eine Spalte weiter und zeigt ihn an.
    // Nicht blockierend (millis()). Muss in jedem loop()-Durchlauf aufgerufen werden.
    void update();

    // Liefert einmalig true, wenn der Text komplett durchgelaufen ist, danach wieder false.
    // Guter Zeitpunkt, um mit setText() einen neuen Text (z. B. neuen Zaehlerstand) zu setzen.
    bool wurdeDurchlaufBeendet();
};
