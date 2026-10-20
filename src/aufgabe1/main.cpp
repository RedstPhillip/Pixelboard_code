//////////////////////////////////////////////
// ---- Aufgabe 1: Laufschrift mit Variable (1 oder 2 Panels, oben GPIO 25, unten GPIO 26) ----
// Text "Zeit: X s" laeuft von rechts nach links (Klasse Laufschrift, Library LEDText).
// 2 Panels: zuerst ueber das obere Panel, danach laeuft er unten weiter. 1 Panel: nur unten.
// Die Zahl (Sekundenzaehler) zaehlt jede Sekunde hoch – auch waehrend der Text gerade durchlaeuft.
// Serial Monitor: jede Sekunde "Laufschrift: Zeit: X s" zum Vergleich mit dem Panel.
// Umgebung in PlatformIO: env:aufgabe1 (siehe documentation/platformio.md)
//////////////////////////////////////////////

#include <Arduino.h>
#include <FastLED.h>
#include "Laufschrift.h"

// ---- Panels (siehe documentation/hardware.md) ----
#define ANZAHL_PANELS 2              // 1 = nur unteres Panel (erster Schritt), 2 = oben + unten
#define PIN_UNTEN 26                 // Data-Pin unteres Panel (Panel 0)
#define PIN_OBEN 25                  // Data-Pin oberes Panel  (Panel 1)
const bool UNTEN_GEDREHT = false;    // unteres Panel: LED 0 links oben (wie Test T7)
const bool OBEN_GEDREHT = true;      // oberes Panel ist um 180° gedreht (Anschluesse zeigen zueinander)

const uint8_t HELLIGKEIT = 20;       // 0..255, niedrig halten (Strom!)
const uint32_t MAX_STROM_MA = 500;   // FastLED begrenzt die Helligkeit auf diesen Strom (USB-tauglich)

Laufschrift laufschrift(50, ANZAHL_PANELS);  // 50 ms pro Spalte
char laufschriftText[32];            // Text mit aktuellem Zaehlerstand
unsigned long letzteSekunde = 0;     // zuletzt angezeigter Zaehlerstand

// Schreibt den aktuellen Sekundenzaehler in laufschriftText. Feste Laenge 12 Zeichen
// (mit Leerzeichen aufgefuellt), damit die Zahl waehrend des Durchlaufs wachsen kann (bis 9999 s).
// Parameter: keine | Rueckgabe: keine
void laufschriftTextErzeugen() {
  char zeitText[24];
  letzteSekunde = millis() / 1000;
  snprintf(zeitText, sizeof(zeitText), "Zeit: %lu s", letzteSekunde);
  snprintf(laufschriftText, sizeof(laufschriftText), "%-12s", zeitText);
  Serial.print("Laufschrift: ");
  Serial.println(zeitText);
}

// Startet einen neuen Durchlauf mit dem aktuellen Zaehlerstand.
// Parameter: keine | Rueckgabe: keine
void laufschriftNeuStarten() {
  laufschriftTextErzeugen();
  laufschrift.setText(laufschriftText);
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, PIN_UNTEN, GRB>(laufschrift.getLeds(0), PanelStreifen::LEDS_PRO_PANEL);
  if (ANZAHL_PANELS >= 2) {
    FastLED.addLeds<WS2812B, PIN_OBEN, GRB>(laufschrift.getLeds(1), PanelStreifen::LEDS_PRO_PANEL);
  }
  laufschrift.setGedreht(0, UNTEN_GEDREHT);
  laufschrift.setGedreht(1, OBEN_GEDREHT);
  FastLED.setBrightness(HELLIGKEIT);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_STROM_MA);
  FastLED.clear(true);

  laufschrift.begin(0, 255, 0);      // gruen
  laufschriftNeuStarten();
}

void loop() {
  laufschrift.update();

  if (laufschrift.wurdeDurchlaufBeendet()) {
    laufschriftNeuStarten();                    // neuer Durchlauf
  } else if (millis() / 1000 != letzteSekunde) {
    laufschriftTextErzeugen();                  // Zahl im laufenden Text hochzaehlen
    laufschrift.aendereText(laufschriftText);
  }
}
