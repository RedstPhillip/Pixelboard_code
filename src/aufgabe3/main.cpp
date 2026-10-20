//////////////////////////////////////////////
// ---- Aufgabe 3: Snake auf dem Pixelboard 32x16 (2 Panels, oben GPIO 25, unten GPIO 26) ----
// Umgebung in PlatformIO: env:aufgabe3 (siehe documentation/platformio.md)
// Steuerung mit Joystick (SW 14, X 34, Y 35, siehe documentation/hardware.md):
//   Stick links/rechts/oben/unten -> Richtung der Schlange
//   Stick kurz druecken           -> Neustart nach Game Over
// Die Schlange faehrt von selbst; Futter (orange) macht sie laenger und schneller.
//////////////////////////////////////////////

#include <Arduino.h>
#include "Pixelboard.h"
#include "Joystick.h"
#include "Snake.h"

// ---- Joystick (siehe documentation/hardware.md) ----
#define JOY_SW 14
#define JOY_X  34
#define JOY_Y  35

// Totzone um die Ruhestellung (ca. 1800..2000): erst ausserhalb davon zaehlt eine Auslenkung
const int SCHWELLE_KLEIN = 1500;   // darunter: links bzw. oben
const int SCHWELLE_GROSS = 2500;   // darueber: rechts bzw. unten

Pixelboard board;
Joystick joystick(JOY_SW, JOY_X, JOY_Y);
Snake snake;

// Liest den Joystick und gibt die Richtung an die Schlange weiter.
// Gewertet wird die Achse, die weiter aus der Mitte ausgelenkt ist – so funktioniert auch schraeges Druecken.
// In der Totzone passiert nichts (Schlange faehrt geradeaus weiter).
void richtungVomJoystick() {
  const int MITTE = 2048;
  int x = joystick.getX();
  int y = joystick.getY();
  bool xAusgelenkt = (x < SCHWELLE_KLEIN || x > SCHWELLE_GROSS);
  bool yAusgelenkt = (y < SCHWELLE_KLEIN || y > SCHWELLE_GROSS);

  if (xAusgelenkt && (!yAusgelenkt || abs(x - MITTE) >= abs(y - MITTE))) {
    snake.setzeRichtung(x > SCHWELLE_GROSS ? Snake::RECHTS : Snake::LINKS);
  } else if (yAusgelenkt) {
    snake.setzeRichtung(y > SCHWELLE_GROSS ? Snake::UNTEN : Snake::OBEN);
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());
  board.begin();
  joystick.begin();
  snake.neustart();
  Serial.println("Snake gestartet. Joystick lenkt, kurz druecken = Neustart nach Game Over.");
}

void loop() {
  joystick.check();

  if (snake.istGameOver()) {
    if (joystick.tasterWurdeKurzGedrueckt()) {
      snake.neustart();
      Serial.println("Neustart");
    }
  } else {
    joystick.tasterWurdeKurzGedrueckt();   // Tastendruecke waehrend des Spiels verwerfen
    richtungVomJoystick();
    snake.bewegen();
    if (snake.istGameOver()) {
      Serial.print("Game Over – Laenge ");
      Serial.println(snake.getLaenge());
    }
  }

  board.loeschen();
  if (snake.istGameOver()) {
    snake.zeichnen(board, CRGB::Red, CRGB::Red);
  } else {
    snake.zeichnen(board, CRGB::Green, CRGB::Orange);
  }
  board.anzeigen();
}
