//////////////////////////////////////////////
// ---- Aufgabe 2: Testprogramm x/y-Koordinaten (1 Panel an GPIO 16) ----
// Umgebung in PlatformIO: env:aufgabe2 (siehe documentation/platformio.md)
// Prueft LedPanel::setzePixel(). Zwei Phasen in Endlosschleife (im Serial Monitor angekuendigt):
//   0: Ecken      – (0,0) rot, (31,0) gruen, (0,7) blau, (31,7) weiss (5 s)
//   1: Zeilenlauf – ein Punkt laeuft zeilenweise von links oben nach rechts unten
// Koordinaten: (0,0) = links oben, x nach rechts, y nach unten.
//////////////////////////////////////////////

#include <Arduino.h>
#include <FastLED.h>
#include "LedPanel.h"

#define PANEL_PIN 16

const uint8_t HELLIGKEIT = 20;       // 0..255, niedrig halten (Strom!)
const uint32_t MAX_STROM_MA = 500;   // FastLED begrenzt die Helligkeit auf diesen Strom (USB-tauglich)

LedPanel panel;

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
  panel.loeschen();
  panel.anzeigen();
  Serial.print("Phase ");
  Serial.println(phase == 0 ? "0: Ecken" : "1: Zeilenlauf");
}

// Phase 0: markiert die vier Ecken in verschiedenen Farben (5 s).
void eckenTest() {
  if (schritt == 0) {
    panel.setzePixel(0, 0, CRGB::Red);                                        // links oben
    panel.setzePixel(LedPanel::BREITE - 1, 0, CRGB::Green);                   // rechts oben
    panel.setzePixel(0, LedPanel::HOEHE - 1, CRGB::Blue);                     // links unten
    panel.setzePixel(LedPanel::BREITE - 1, LedPanel::HOEHE - 1, CRGB::White); // rechts unten
    panel.anzeigen();
    schritt = 1;
  }
  if (millis() - phasenStart >= 5000) naechstePhase();
}

// Phase 1: ein Punkt laeuft Zeile fuer Zeile von links nach rechts, von oben nach unten.
void zeilenLauf() {
  if (millis() - letzterSchritt < 30) return;
  letzterSchritt = millis();
  if (schritt >= LedPanel::ANZAHL) { naechstePhase(); return; }

  int x = schritt % LedPanel::BREITE;
  int y = schritt / LedPanel::BREITE;
  panel.loeschen();
  panel.setzePixel(x, y, CRGB::Orange);
  panel.anzeigen();
  schritt++;
}

void setup() {
  Serial.begin(115200);
  panel.begin<PANEL_PIN>();
  FastLED.setBrightness(HELLIGKEIT);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_STROM_MA);

  phase = 1;          // naechstePhase() springt auf Phase 0
  naechstePhase();
}

void loop() {
  switch (phase) {
    case 0: eckenTest();  break;
    case 1: zeilenLauf(); break;
  }
}
