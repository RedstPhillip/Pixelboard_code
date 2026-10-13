//////////////////////////////////////////////
// ---- Aufgabe 1: Laufschrift mit Variable (1 Panel an GPIO 16) ----
// Text "Zeit: X s" laeuft von rechts nach links ueber das Panel (Klasse Laufschrift, Library LEDText).
// Die Zahl (Sekundenzaehler) zaehlt jede Sekunde hoch – auch waehrend der Text gerade durchlaeuft.
// Serial Monitor: jede Sekunde "Laufschrift: Zeit: X s" zum Vergleich mit dem Panel.
// Umgebung in PlatformIO: env:aufgabe1 (siehe documentation/platformio.md)
//////////////////////////////////////////////

#include <Arduino.h>
#include <FastLED.h>
#include "Laufschrift.h"

#define PANEL_PIN 16

const uint8_t HELLIGKEIT = 20;       // 0..255, niedrig halten (Strom!)
const uint32_t MAX_STROM_MA = 500;   // FastLED begrenzt die Helligkeit auf diesen Strom (USB-tauglich)

Laufschrift laufschrift(50);         // 50 ms pro Spalte
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
  FastLED.addLeds<WS2812B, PANEL_PIN, GRB>(laufschrift.getLeds(), laufschrift.getAnzahlLeds());
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
