//////////////////////////////////////////////
// ---- Laufschrift-Klasse (Deklaration) ----
// Laesst einen Text ueber ein LED-Panel (WS2812, 8x32, vertikales Zick-Zack) laufen.
// Bibliotheken: FastLED, LEDMatrix und LEDText (Aaron Liddiment), siehe platformio.ini
// Verdrahtung siehe documentation/hardware.md, wird in src/aufgabe1/main.cpp verwendet
// Implementierung: Laufschrift.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>

// ---- Panelgroesse und Ausrichtung ----
// Ein negatives Vorzeichen spiegelt die Achse. Steht der Text auf dem Kopf, das Vorzeichen von
// PANEL_HOEHE aendern; laeuft er spiegelverkehrt, das Vorzeichen von PANEL_BREITE aendern.
const int16_t PANEL_BREITE = 32;  // LEDs waagrecht
const int16_t PANEL_HOEHE = -8;   // LEDs senkrecht (negativ: LED 0 sitzt oben links, wie in Test T7 ermittelt)

class Laufschrift {
  private:
    cLEDMatrix<PANEL_BREITE, PANEL_HOEHE, VERTICAL_ZIGZAG_MATRIX> matrix;  // LED-Speicher + Koordinaten
    cLEDText text;                                                          // Laufschrift-Logik (LEDText)

    static const uint16_t MAX_TEXT_LAENGE = 64;      // max. Zeichen inkl. fuehrender Leerzeichen
    unsigned char textPuffer[MAX_TEXT_LAENGE + 1];   // eigene Kopie des Textes (LEDText merkt sich nur den Zeiger)
    uint16_t textLaenge;                             // aktuelle Laenge im Puffer
    bool textGesetzt;                                // true, sobald setText() einmal aufgerufen wurde

    unsigned long schrittZeit;         // ms pro Verschiebung um 1 Spalte (kleiner = schneller)
    unsigned long letzterSchrittZeit;  // Zeitpunkt der letzten Verschiebung
    bool durchlaufFertig;              // true, wenn der Text komplett durchgelaufen ist

  public:
    // Konstruktor: speichert nur die Laufgeschwindigkeit (noch kein Hardwarezugriff).
    // schrittZeitMs = Zeit in ms, nach der der Text um eine Spalte weiterlaeuft.
    Laufschrift(unsigned long schrittZeitMs);

    // Liefert den LED-Speicher des Panels. Wird fuer FastLED.addLeds() in setup() gebraucht.
    CRGB* getLeds();

    // Liefert die Anzahl der LEDs des Panels (32 * 8 = 256).
    int getAnzahlLeds();

    // Stellt Schriftart und Textfarbe ein. Einmal in setup() nach FastLED.addLeds() aufrufen.
    // rot/gruen/blau = Textfarbe (0..255)
    void begin(uint8_t rot, uint8_t gruen, uint8_t blau);

    // Uebernimmt einen neuen Text und startet den Durchlauf von vorne (Panel rechts leer).
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
