//////////////////////////////////////////////
// ---- LedPanel-Klasse (Implementierung) ----
// Deklaration und Koordinatensystem: LedPanel.h
//////////////////////////////////////////////

#include "LedPanel.h"

// Konstruktor: alle LEDs im Speicher schwarz (noch kein Hardwarezugriff).
// gedreht180 = true, wenn das Panel um 180° gedreht eingebaut ist.
LedPanel::LedPanel(bool gedreht180) {
  gedreht = gedreht180;
  loeschen();
}

// Rechnet eine x/y-Koordinate in den LED-Index (0..255) um, beruecksichtigt die Drehung.
// Liefert -1, wenn die Koordinate ausserhalb des Panels liegt.
//
// 1. Drehung: Ein um 180° gedrehtes Panel ist in x UND y gespiegelt
//    -> x = 31 - x, y = 7 - y, danach wie ein normales Panel weiterrechnen.
// 2. Zick-Zack (Vertical Zig-Zag, nicht gedreht, von vorne gesehen, per Test T7 bestaetigt):
//    LED 0 sitzt links oben. Spalte 0 laeuft nach unten (Index 0..7),
//    Spalte 1 nach oben (Index 8..15), Spalte 2 wieder nach unten usw.
//      Gerade Spalte   (x = 0, 2, ...): laeuft nach unten -> Index = x * HOEHE + y
//      Ungerade Spalte (x = 1, 3, ...): laeuft nach oben  -> Index = x * HOEHE + (HOEHE - 1 - y)
int LedPanel::xyZuIndex(int x, int y) {
  if (x < 0 || x >= BREITE || y < 0 || y >= HOEHE) return -1;

  if (gedreht) {
    x = (BREITE - 1) - x;
    y = (HOEHE - 1) - y;
  }

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

// Schaltet alle LEDs dieses Panels aus (im Speicher). Sichtbar erst nach anzeigen().
void LedPanel::loeschen() {
  fill_solid(leds, ANZAHL, CRGB::Black);
}

// Uebertraegt die Farbwerte an die LEDs (FastLED.show() – aktualisiert alle angemeldeten Panels).
void LedPanel::anzeigen() {
  FastLED.show();
}
