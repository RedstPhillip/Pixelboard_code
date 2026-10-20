//////////////////////////////////////////////
// ---- Schritt 1: Stand-Alone-Programm ohne FreeRTOS ----
// Gleiche Funktion wie das Gesamtprogramm, aber alles im normalen Arduino-loop():
//   - Joystick wird in jedem loop()-Durchlauf abgefragt
//   - langer Joystick-Druck (>= 1 s) schaltet den Modus weiter: A -> B -> C -> A ...
//   - jede Sekunde wird der Buchstabe des aktiven Modus ausgegeben
// Kein delay(1000), sondern millis(): sonst wuerde der Joystick 1 s lang nicht abgefragt.
// Pins: Joystick SW 14, X 34, Y 35 (wie im Projekt Taster_Klasse)
// Umgebung in PlatformIO: env:schritt1_ohne_freertos
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
int aktiverModus = 0;              // 0 = A, 1 = B, 2 = C

// ---- Sekundentakt ----
const unsigned long AUSGABE_ABSTAND = 1000;  // ms
unsigned long letzteAusgabe = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Schritt 1 (ohne FreeRTOS): langer Joystick-Druck (>= 1 s) schaltet A -> B -> C um");
  joystick.begin();
}

void loop() {
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
}
