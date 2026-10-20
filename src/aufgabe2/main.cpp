//////////////////////////////////////////////
// ---- Aufgabe 2: Testprogramm x/y-Koordinaten (2 Panels, oben GPIO 25, unten GPIO 26) ----
// Umgebung in PlatformIO: env:aufgabe2 (siehe documentation/platformio.md)
// Prueft Pixelboard::setzePixel() auf der Gesamtflaeche 32x16. Zwei Phasen in Endlosschleife
// (im Serial Monitor angekuendigt):
//   0: Ecken      – (0,0) rot, (31,0) gruen, (0,15) blau, (31,15) weiss,
//                   Panelgrenze: links in Zeile 7 gelb (oben) und Zeile 8 tuerkis (unten) (5 s)
//   1: Zeilenlauf – ein Punkt laeuft zeilenweise von links oben nach rechts unten (ueber beide Panels)
// Koordinaten: (0,0) = links oben, x nach rechts (0..31), y nach unten (0..15).
//////////////////////////////////////////////

#include <Arduino.h>
#include "Pixelboard.h"

Pixelboard board;   // Standard-Aufbau: oben nicht gedreht, unten gedreht

// ---- Ablaufsteuerung ----
uint8_t phase = 0;
unsigned long phasenStart = 0;
unsigned long letzterSchritt = 0;
int schritt = 0;

// Startet die naechste Phase (nach der letzten wieder Phase 0) und meldet sie im Serial Monitor.
void naechstePhase() {
  phase = (phase + 1) % 2;
  phasenStart = millis();
  schritt = 0;
  board.loeschen();
  board.anzeigen();
  Serial.print("Phase ");
  Serial.println(phase == 0 ? "0: Ecken" : "1: Zeilenlauf");
}

// Phase 0: markiert die vier Ecken der Gesamtflaeche und die Panelgrenze (5 s).
void eckenTest() {
  if (schritt == 0) {
    const int rechts = Pixelboard::BREITE - 1;
    const int unten = Pixelboard::HOEHE - 1;
    board.setzePixel(0, 0, CRGB::Red);                // links oben
    board.setzePixel(rechts, 0, CRGB::Green);         // rechts oben
    board.setzePixel(0, unten, CRGB::Blue);           // links unten
    board.setzePixel(rechts, unten, CRGB::White);     // rechts unten
    for (int x = 1; x <= 4; x++) {
      board.setzePixel(x, 7, CRGB::Yellow);           // unterste Zeile oberes Panel
      board.setzePixel(x, 8, CRGB::Cyan);             // oberste Zeile unteres Panel
    }
    board.anzeigen();
    schritt = 1;
  }
  if (millis() - phasenStart >= 5000) naechstePhase();
}

// Phase 1: ein Punkt laeuft Zeile fuer Zeile von links nach rechts, von oben nach unten (beide Panels).
void zeilenLauf() {
  if (millis() - letzterSchritt < 15) return;
  letzterSchritt = millis();
  if (schritt >= Pixelboard::BREITE * Pixelboard::HOEHE) { naechstePhase(); return; }

  int x = schritt % Pixelboard::BREITE;
  int y = schritt / Pixelboard::BREITE;
  board.loeschen();
  board.setzePixel(x, y, CRGB::Orange);
  board.anzeigen();
  schritt++;
}

void setup() {
  Serial.begin(115200);
  board.begin();      // oben GPIO 25, unten GPIO 26, Helligkeit 20, max. 500 mA

  phase = 1;          // naechstePhase() springt auf Phase 0
  naechstePhase();
}

void loop() {
  switch (phase) {
    case 0: eckenTest();  break;
    case 1: zeilenLauf(); break;
  }
}
