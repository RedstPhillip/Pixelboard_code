//////////////////////////////////////////////
// ---- Snake-Klasse (Deklaration) ----
// Einfaches Snake-Spiel auf einem LED-Panel 32x8 (1 Panel).
// Koordinaten: (0,0) = links oben, x nach rechts, y nach unten.
// Verwendung siehe src/aufgabe3/main.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>

struct Punkt {
  int x;
  int y;
};

class Snake {
  public:
    static const int MAX_KOERPER = 256; // genug fuer 32x8

  private:
    Punkt koerper[MAX_KOERPER];
    int laenge;
    // Richtung: 0 = rechts, 1 = unten, 2 = links, 3 = oben
    int richtung;
    int naechsteRichtung;
    Punkt futter;
    bool futterVorhanden;
    bool gameOver;
    bool gestartet;
    unsigned long letzteBewegung;
    unsigned long geschwindigkeitMs; // Bewegung alle X ms
    int breite;
    int hoehe;

    // erzeugt neues Futter, das nicht auf der Schlange liegt
    void erzeugeFutter();

  public:
    Snake(int b = 32, int h = 8);
    // initialisiert Spiel (neustart)
    void init(int startX = 16, int startY = 4);
    // setzt naechste Richtung (nur 90 Grad erlaubt: nicht direkt umkehren)
    void setzeRichtung(int neueRichtung); // 0 rechts, 1 unten, 2 links, 3 oben
    // bewegt Schlange, prueft Kollisionen, frisst Futter
    void bewegen();
    // zeichnet auf LedPanel (ueber setzePixel)
    void zeichnen(class LedPanel &panel, CRGB schlangeFarbe, CRGB futterFarbe);
    // Game Over?
    bool istGameOver();
    // gestartet?
    bool istGestartet();
    // Starten/Pausenumschalter o.ae. nicht gebraucht, init startet neu
    int getLaenge();
    unsigned long getGeschwindigkeitMs();
    void setzeGeschwindigkeitMs(unsigned long v);
};

