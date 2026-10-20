//////////////////////////////////////////////
// ---- Schritt 2: Stand-Alone-Programm mit einem einzigen FreeRTOS-Task ----
// Der Code aus Schritt 1 wird in einen FreeRTOS-Task verschoben:
//   - Setup-Teil des Tasks (vor der Endlosschleife) = bisheriges setup()
//   - Endlosschleife des Tasks                      = bisheriges loop()
//   - in der Endlosschleife ein delay(), damit der Task die CPU abgibt
// Funktion: langer Joystick-Druck (>= 1 s) schaltet A -> B -> C -> A ...,
//           jede Sekunde wird der Buchstabe des aktiven Modus ausgegeben.
// Pins: Joystick SW 14, X 34, Y 35 (wie im Projekt Taster_Klasse)
// Umgebung in PlatformIO: env:schritt2_ein_task
//////////////////////////////////////////////

#include <Arduino.h>
#include "Joystick.h"

// ---- Joystick ----
#define PIN_SW 14
#define PIN_X 34
#define PIN_Y 35
Joystick joystick(PIN_SW, PIN_X, PIN_Y);

// ---- Modi ----
const int ANZAHL_MODI = 3;
const char MODUS_NAMEN[ANZAHL_MODI] = {'A', 'B', 'C'};

// ---- Einziger Task: Joystick abfragen und A/B/C ausgeben ----
void joystickAbcTask(void *pvParameters) {
  // Setup-Teil des Tasks
  const unsigned long AUSGABE_ABSTAND = 1000;  // ms
  unsigned long letzteAusgabe = 0;
  int aktiverModus = 0;            // 0 = A, 1 = B, 2 = C
  joystick.begin();
  delay(5);                        // damit am Anfang alle Tasks einmal aufgerufen werden

  for (;;) {
    joystick.check();

    // Moduswechsel bei langem Druck
    if (joystick.tasterWurdeLangeGedrueckt()) {
      aktiverModus = (aktiverModus + 1) % ANZAHL_MODI;
      Serial.print("--- Moduswechsel: jetzt Modus ");
      Serial.println(MODUS_NAMEN[aktiverModus]);
    }

    // Buchstabe des aktiven Modus im Sekundentakt ausgeben
    if (millis() - letzteAusgabe >= AUSGABE_ABSTAND) {
      letzteAusgabe = millis();
      Serial.println(MODUS_NAMEN[aktiverModus]);
    }

    delay(10);                     // Task gibt die CPU ab, Joystick wird alle 10 ms abgefragt
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Schritt 2 (ein FreeRTOS-Task): langer Joystick-Druck (>= 1 s) schaltet A -> B -> C um");

  // xTaskCreate(Funktion, Name, Stackgroesse in Byte, Parameter, Prioritaet 0..3, Handle)
  xTaskCreate(joystickAbcTask, "JoystickABC", 2048, NULL, 1, NULL);
}

void loop() {
  // Die Arbeit erledigt der Task – der Arduino-loop-Task wird nicht gebraucht
  vTaskDelete(NULL);
}
