//////////////////////////////////////////////
// ---- LedPanel-Klasse (Implementierung) ----
// Deklaration und Koordinatensystem: LedPanel.h
//////////////////////////////////////////////

#include "LedPanel.h"

// Konstruktor: setzt alle LEDs im Speicher auf schwarz (noch kein Hardwarezugriff).
LedPanel::LedPanel() {
  loeschen();
}

// Rechnet eine x/y-Koordinate in den LED-Index (0..255) um.
// Liefert -1, wenn die Koordinate ausserhalb des Panels liegt.
//
// Verdrahtung (Vertical Zig-Zag, von vorne gesehen, per Test T7 bestaetigt):
//   LED 0 sitzt links oben. Spalte 0 laeuft nach unten (Index 0..7),
//   Spalte 1 nach oben (Index 8..15), Spalte 2 wieder nach unten usw.
//
//   Gerade Spalte   (x = 0, 2, ...): laeuft nach unten -> Index = x * HOEHE + y
//   Ungerade Spalte (x = 1, 3, ...): laeuft nach oben  -> Index = x * HOEHE + (HOEHE - 1 - y)
int LedPanel::xyZuIndex(int x, int y) {
  if (x < 0 || x >= BREITE || y < 0 || y >= HOEHE) return -1;

  int spaltenStart = x * HOEHE;
  if (x % 2 == 0) {
    return spaltenStart + y;
  } else {
    return spaltenStart + (HOEHE - 1 - y);
  }
}

// Setzt die LED an Position (x, y) auf die angegebene Farbe. Ausserhalb des Panels: wird ignoriert.
void LedPanel::setzePixel(int x, int y, CRGB farbe) {
  int index = xyZuIndex(x, y);
  if (index >= 0) leds[index] = farbe;
}

// Schaltet alle LEDs aus (im Speicher). Sichtbar erst nach anzeigen().
void LedPanel::loeschen() {
  fill_solid(leds, ANZAHL, CRGB::Black);
}

// Uebertraegt die Farbwerte an die LEDs (FastLED.show() – aktualisiert alle angemeldeten Panels).
void LedPanel::anzeigen() {
  FastLED.show();
}
