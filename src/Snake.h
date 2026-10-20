//////////////////////////////////////////////
// ---- Snake-Klasse (Deklaration) ----
// Snake-Spiel auf dem Pixelboard 32x16 (2 Panels uebereinander).
// Koordinaten wie Pixelboard: (0,0) = links oben, x nach rechts, y nach unten.
//
// Benutzung (siehe src/aufgabe3/main.cpp):
//   snake.neustart();                        // in setup() und nach Game Over
//   snake.setzeRichtung(Snake::OBEN);        // z. B. vom Joystick
//   snake.bewegen();                         // in jedem loop(), bewegt sich selbst im Takt
//   snake.zeichnen(board, CRGB::Green, CRGB::Orange);
//
// Implementierung: Snake.cpp
//////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "Pixelboard.h"

struct Punkt {
  int x;
  int y;
};

class Snake {
  public:
    // Bewegungsrichtungen (Reihenfolge wichtig: Gegenrichtung = (richtung + 2) % 4)
    enum Richtung { RECHTS = 0, UNTEN = 1, LINKS = 2, OBEN = 3 };

    static const int BREITE = Pixelboard::BREITE;            // 32
    static const int HOEHE = Pixelboard::HOEHE;              // 16
    static const int MAX_LAENGE = BREITE * HOEHE;            // 512
    static const int START_LAENGE = 3;
    static const unsigned long START_TEMPO_MS = 150;         // Bewegung alle 150 ms
    static const unsigned long MIN_TEMPO_MS = 80;            // schneller wird es nicht
    static const unsigned long BESCHLEUNIGUNG_MS = 2;        // pro gefressenem Futter

  private:
    Punkt koerper[MAX_LAENGE];   // koerper[0] = Kopf
    int laenge;
    Richtung richtung;           // aktuelle Bewegungsrichtung
    Richtung naechsteRichtung;   // wird beim naechsten Schritt uebernommen
    Punkt futter;
    bool gameOver;
    unsigned long tempoMs;       // Zeit zwischen zwei Schritten
    unsigned long letzterSchritt;

    // Setzt das Futter auf ein zufaelliges freies Feld (nicht auf die Schlange).
    void neuesFutter();

    // Liefert true, wenn der Punkt (x, y) von der Schlange belegt ist.
    bool istAufSchlange(int x, int y);

  public:
    // Konstruktor: legt das Spiel an, ohne es zu starten (noch kein Hardwarezugriff).
    Snake();

    // Startet ein neues Spiel: Schlange (3 Glieder) in der Mitte, faehrt nach rechts, neues Futter.
    void neustart();

    // Merkt sich die naechste Richtung. Direkte Umkehr (z. B. rechts -> links) wird ignoriert.
    void setzeRichtung(Richtung neueRichtung);

    // Bewegt die Schlange um ein Feld, sobald tempoMs vergangen ist (nicht blockierend, in jedem loop()).
    // Prueft Wand- und Selbstkollision (-> Game Over) und frisst Futter (-> laenger und schneller).
    void bewegen();

    // Zeichnet Schlange und Futter auf das Pixelboard. Sichtbar erst nach board.anzeigen().
    void zeichnen(Pixelboard &board, CRGB schlangeFarbe, CRGB futterFarbe);

    // Liefert true nach einer Wand- oder Selbstkollision.
    bool istGameOver();

    // Liefert die aktuelle Laenge der Schlange (z. B. als Punktestand).
    int getLaenge();
};
