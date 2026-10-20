//////////////////////////////////////////////
// ---- Schritt 3: Gesamtprogramm (FreeRTOS-Geruest mit Moduswechsel per Joystick) ----
// Erstes Testprogramm fuer das Pixelboard-Hauptprogramm:
//   - Joystick-Task: fragt den Joystick ab (immer aktiv)
//   - Master-Task:   schaltet bei langem Joystick-Druck (>= 1 s) auf den naechsten Modus um,
//                    indem er den aktiven Modus-Task suspendet und den naechsten resumed (immer aktiv)
//   - Modus-Tasks A, B, C: geben im Sekundentakt "A" bzw. "B" bzw. "C" aus;
//                    es laeuft immer nur einer davon, die anderen sind suspended
// Pins: Joystick SW 14, X 34, Y 35 (wie im Projekt Taster_Klasse)
// Umgebung in PlatformIO: env:schritt3_gesamtprogramm
//////////////////////////////////////////////

#include <Arduino.h>
#include "Joystick.h"

// ---- Joystick ----
#define PIN_SW 14
#define PIN_X 34
#define PIN_Y 35
Joystick joystick(PIN_SW, PIN_X, PIN_Y);

// ---- Kommunikation Joystick-Task -> Master-Task ----
volatile bool langerDruckErkannt = false;

// ---- Handles der Modus-Tasks (damit der Master sie suspenden/resumen kann) ----
const int ANZAHL_MODI = 3;
TaskHandle_t modusHandles[ANZAHL_MODI] = {NULL, NULL, NULL};
const char MODUS_NAMEN[ANZAHL_MODI] = {'A', 'B', 'C'};
int aktiverModus = 0;              // 0 = A, 1 = B, 2 = C

// ---- Mutex fuer Serial ----
// Verhindert, dass sich Ausgaben verschiedener Tasks ineinander verschachteln.
// Ausserdem nimmt der Master den Mutex vor dem Suspenden: so wird ein Modus-Task nie
// mitten in einem Serial.println() angehalten (sonst bliebe Serial fuer alle blockiert).
SemaphoreHandle_t serialMutex;

// ---- Joystick-Task (immer aktiv) ----
// Fragt alle 10 ms den Joystick ab und meldet einen langen Druck an den Master.
void joystickTask(void *pvParameters) {
  // Setup-Teil des Tasks
  joystick.begin();
  delay(5);                        // damit am Anfang alle Tasks einmal aufgerufen werden

  for (;;) {
    joystick.check();
    if (joystick.tasterWurdeLangeGedrueckt()) {
      langerDruckErkannt = true;
    }
    delay(10);
  }
}

// ---- Master-Task (immer aktiv) ----
// Startet mit Modus A und schaltet bei jedem langen Druck zum naechsten Modus (A -> B -> C -> A ...).
void masterTask(void *pvParameters) {
  // Setup-Teil des Tasks
  delay(5);                        // alle Tasks laufen einmal an, erst dann sind die Handles sicher gueltig
  for (int i = 0; i < ANZAHL_MODI; i++) {
    if (i != aktiverModus) vTaskSuspend(modusHandles[i]);
  }

  for (;;) {
    if (langerDruckErkannt) {
      langerDruckErkannt = false;

      xSemaphoreTake(serialMutex, portMAX_DELAY);
      vTaskSuspend(modusHandles[aktiverModus]);
      aktiverModus = (aktiverModus + 1) % ANZAHL_MODI;
      vTaskResume(modusHandles[aktiverModus]);
      Serial.print("--- Moduswechsel: jetzt Modus ");
      Serial.println(MODUS_NAMEN[aktiverModus]);
      xSemaphoreGive(serialMutex);
    }
    delay(20);
  }
}

// ---- Modus-Tasks A, B, C ----
// Gibt den Buchstaben aus und wartet 1 s (Sekundentakt).
void ausgeben(char buchstabe) {
  xSemaphoreTake(serialMutex, portMAX_DELAY);
  Serial.println(buchstabe);
  xSemaphoreGive(serialMutex);
}

void taskA(void *pvParameters) {
  delay(5);
  for (;;) {
    ausgeben('A');
    delay(1000);
  }
}

void taskB(void *pvParameters) {
  delay(5);
  for (;;) {
    ausgeben('B');
    delay(1000);
  }
}

void taskC(void *pvParameters) {
  delay(5);
  for (;;) {
    ausgeben('C');
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("FreeRTOS-Geruest: langer Joystick-Druck (>= 1 s) schaltet A -> B -> C um");

  serialMutex = xSemaphoreCreateMutex();

  // xTaskCreate(Funktion, Name, Stackgroesse in Byte, Parameter, Prioritaet 0..3, Handle)
  // Zuerst die Modus-Tasks, damit ihre Handles existieren, bevor der Master startet.
  xTaskCreate(taskA, "TaskA", 2048, NULL, 1, &modusHandles[0]);
  xTaskCreate(taskB, "TaskB", 2048, NULL, 1, &modusHandles[1]);
  xTaskCreate(taskC, "TaskC", 2048, NULL, 1, &modusHandles[2]);
  xTaskCreate(joystickTask, "Joystick", 2048, NULL, 2, NULL);
  xTaskCreate(masterTask, "Master", 2048, NULL, 2, NULL);
}

void loop() {
  // Die Arbeit erledigen die Tasks – der Arduino-loop-Task wird nicht gebraucht
  vTaskDelete(NULL);
}
