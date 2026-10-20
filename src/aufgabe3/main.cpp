//////////////////////////////////////////////
// ---- Aufgabe 3: Snake auf dem Pixelboard 32x16 (2 Panels, oben GPIO 25, unten GPIO 26) ----
// Umgebung in PlatformIO: env:aufgabe3 (siehe documentation/platformio.md)
// Steuerung mit Joystick (SW 14, X 34, Y 35, siehe documentation/hardware.md):
//   Stick links/rechts/oben/unten -> Richtung der Schlange
//   Stick kurz druecken           -> Neustart nach Game Over
// Die Schlange faehrt von selbst; Futter (orange) macht sie laenger und schneller.
//
// FreeRTOS: zwei Tasks (siehe documentation/uml.md)
//   Taster-Task: liest Taster und Achsen alle 10 ms, merkt kurze Tastendruecke vor
//   Spiel-Task:  wertet die Achsen aus, bewegt die Schlange und zeichnet das Bild
// Der Spiel-Task ist der einzige, der Snake und Pixelboard benutzt (kein Mutex noetig).
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

// Taster-Task -> Spiel-Task: kurzer Druck wurde erkannt (wird vom Spiel-Task zurueckgesetzt).
volatile bool neustartAngefordert = false;

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

// ---- Taster-Task ----
// Liest Taster und Achsen alle 10 ms. Ein kurzer Druck wird nur vorgemerkt;
// ob daraus ein Neustart wird, entscheidet der Spiel-Task (nur nach Game Over).
void tasterTask(void *parameter) {
  for (;;) {
    joystick.check();
    if (joystick.tasterWurdeKurzGedrueckt()) neustartAngefordert = true;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// ---- Spiel-Task ----
// Startet nach Game Over neu, wenn ein Tastendruck vorgemerkt ist; sonst Richtung, Bewegung, Anzeige.
// Laeuft alle 10 ms (die Schlange bewegt sich selbst im Takt von Snake::bewegen()).
void spielTask(void *parameter) {
  for (;;) {
    if (neustartAngefordert) {
      neustartAngefordert = false;
      if (snake.istGameOver()) {
        snake.neustart();
        Serial.println("Neustart");
      }
    }

    if (!snake.istGameOver()) {
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

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());
  board.begin();
  joystick.begin();
  snake.neustart();
  Serial.println("Snake gestartet (FreeRTOS): Joystick lenkt, kurz druecken = Neustart nach Game Over.");

  // xTaskCreate(Funktion, Name, Stackgroesse in Byte, Parameter, Prioritaet, Handle)
  // Taster hat die hoehere Prioritaet, damit er nicht von der Anzeige blockiert wird.
  xTaskCreate(tasterTask, "Taster", 2048, NULL, 2, NULL);
  xTaskCreate(spielTask, "Spiel", 4096, NULL, 1, NULL);
}

void loop() {
  // Die Arbeit erledigen die beiden Tasks – der Arduino-loop-Task wird nicht gebraucht.
  vTaskDelete(NULL);
}
