//////////////////////////////////////////////
// ---- Aufgabe 3: Snake-Spiel (1 Panel an GPIO 16) ----
// Umgebung in PlatformIO: env:aufgabe3 (siehe documentation/platformio.md)
// Steuerung mit Joystick (SW 14, X 34, Y 35):
//   X-Achse -> links/rechts
//   Y-Achse -> hoch/runter (je nach Verdrahtung, wird per Schwellwert ausgewertet)
//   Taster (kurz) -> Neustart nach Game Over
// Koordinaten: (0,0) = links oben, x nach rechts, y nach unten.
//////////////////////////////////////////////

#include <Arduino.h>
#include <FastLED.h>
#include "LedPanel.h"
#include "Joystick.h"
#include "Snake.h"

#define PANEL_PIN 16

// Joystick-Pins wie in hardware.md
#define JOY_SW 14
#define JOY_X  34
#define JOY_Y  35

const uint8_t HELLIGKEIT = 20;
const uint32_t MAX_STROM_MA = 500;

LedPanel panel;
Joystick joystick(JOY_SW, JOY_X, JOY_Y);
Snake snake(32, 8);

// Schwellwerte fuer Richtungswechsel (Rohwerte 0..4095).
// Gemessene Ruhestellung des Joysticks liegt bei ca. 1800..2000 (Test T1),
// daher asymmetrische Schwellen statt "Mitte 2048".
const int SCHWELLE_UNTEN = 1500; // kleiner = links/oben
const int SCHWELLE_OBEN = 2500;  // groesser = rechts/unten

unsigned long letzterRichtungsWechsel = 0;
const unsigned long RICHTUNG_DELAY_MS = 80;

void richteNachJoystick() {
  if (millis() - letzterRichtungsWechsel < RICHTUNG_DELAY_MS) return;

  int x = joystick.getX();
  int y = joystick.getY();

  // Rechts
  if (x > SCHWELLE_OBEN) {
    snake.setzeRichtung(0); // rechts
    letzterRichtungsWechsel = millis();
    return;
  }
  // Links
  if (x < SCHWELLE_UNTEN) {
    snake.setzeRichtung(2); // links
    letzterRichtungsWechsel = millis();
    return;
  }
  // Unten (je nach Hardware kann Y invertiert sein; pruefen)
  if (y > SCHWELLE_OBEN) {
    snake.setzeRichtung(1); // unten
    letzterRichtungsWechsel = millis();
    return;
  }
  // Oben
  if (y < SCHWELLE_UNTEN) {
    snake.setzeRichtung(3); // oben
    letzterRichtungsWechsel = millis();
    return;
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());

  panel.begin<PANEL_PIN>();
  FastLED.setBrightness(HELLIGKEIT);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_STROM_MA);

  joystick.begin();
  snake.init(16, 4);

  Serial.println("Snake gestartet. Mit Joystick steuern. Taster kurz fuer Neustart nach Game Over.");
}

void loop() {
  joystick.check();

  if (snake.istGameOver()) {
    // Game Over anzeigen: alles kurz rot blinken? oder nur stehen lassen
    panel.loeschen();
    // einfach Snake in Rot zeichnen
    snake.zeichnen(panel, CRGB::Red, CRGB::Red);
    panel.anzeigen();
    // Neustart mit kurzem Tastendruck
    if (joystick.tasterWurdeKurzGedrueckt()) {
      snake.init(16, 4);
      Serial.println("Neustart");
    }
    delay(10);
    return;
  }

  richteNachJoystick();
  snake.bewegen();
  panel.loeschen();
  snake.zeichnen(panel, CRGB::Green, CRGB::Orange);
  panel.anzeigen();
  delay(1);
}

