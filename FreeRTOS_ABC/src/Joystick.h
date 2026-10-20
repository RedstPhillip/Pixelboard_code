//////////////////////////////////////////////
// ---- Joystick-Klasse (Deklaration) ----
// Umbau der Klasse Taster aus dem Vorjahr:
// Taster (SW) + 2 Potentiometer (X- und Y-Achse)
// Board: ESP32, Pinbelegung siehe documentation/hardware.md
// Implementierung: Joystick.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>

class Joystick {
  private:
    // ---- Pins ----
    uint8_t pinSw;
    uint8_t pinX;
    uint8_t pinY;

    // ---- Taster (SW) ----
    bool aktuellerZustand;
    bool letzterZustand;
    unsigned long letzteAenderungZeit;
    unsigned long gedruecktZeit;
    bool wurdeKurz;
    bool wurdeLang;
    bool langMeldungSchonGesendet;
    static const unsigned long ENTPRELL_ZEIT = 50;     // ms, fuer alle Objekte gleich
    static const unsigned long LANG_DRUCK_ZEIT = 1000; // ms

    // ---- Achsen (Rohwerte 0..4095, 12-Bit-ADC des ESP32) ----
    int xWert;
    int yWert;

    // Liest den Taster ein, entprellt das Signal und erkennt kurze bzw. lange Tastendruecke.
    void tasterPruefen();

    // Liest die Rohwerte (0..4095) der beiden Potentiometer ein.
    void achsenLesen();

  public:
    // Konstruktor: speichert nur die Pins und setzt Startwerte (noch kein Hardwarezugriff).
    // pinSwNr = Taster, pinXNr/pinYNr = Achsen (muessen ADC1-Pins sein, z. B. 32-35).
    Joystick(uint8_t pinSwNr, uint8_t pinXNr, uint8_t pinYNr);

    // Konfiguriert den Taster-Pin mit internem Pull-up. Einmal in setup() aufrufen.
    void begin();

    // Aktualisiert Taster und Achsen. Muss in jedem loop()-Durchlauf aufgerufen werden.
    void check();

    // Liefert true, solange der Taster gedrueckt ist.
    bool tasterGedrueckt();

    // Liefert einmalig true nach einem kurzen Tastendruck (< 1 s), danach wieder false.
    bool tasterWurdeKurzGedrueckt();

    // Liefert einmalig true, sobald der Taster 1 s lang gedrueckt gehalten wurde.
    bool tasterWurdeLangeGedrueckt();

    // Liefert den zuletzt gelesenen Rohwert der X-Achse (0..4095).
    int getX();

    // Liefert den zuletzt gelesenen Rohwert der Y-Achse (0..4095).
    int getY();
};
