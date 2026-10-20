//////////////////////////////////////////////
// ---- Pixelboard-Klasse (Implementierung) ----
// Deklaration, Koordinatensystem und Benutzung: Pixelboard.h
//////////////////////////////////////////////

#include "Pixelboard.h"

// Konstruktor: legt beide Panels mit ihrer Drehung an (noch kein Hardwarezugriff).
Pixelboard::Pixelboard(bool obenGedreht, bool untenGedreht)
  : oben(obenGedreht), unten(untenGedreht) {
}

// Setzt die Helligkeit aller LEDs (0..255). Wirkt ab dem naechsten anzeigen().
void Pixelboard::setHelligkeit(uint8_t helligkeit) {
  FastLED.setBrightness(helligkeit);
}

// Setzt die LED an Position (x, y) auf die angegebene Farbe.
// y 0..7 gehoert zum oberen Panel, y 8..15 zum unteren (dort lokal y - 8).
// Ungueltige x-Werte und y < 0 ignoriert LedPanel::setzePixel, y >= 16 wird hier ignoriert.
void Pixelboard::setzePixel(int x, int y, CRGB farbe) {
  if (y < LedPanel::HOEHE) {
    oben.setzePixel(x, y, farbe);
  } else if (y < HOEHE) {
    unten.setzePixel(x, y - LedPanel::HOEHE, farbe);
  }
}

// Schaltet alle LEDs beider Panels aus (im Speicher). Sichtbar erst nach anzeigen().
void Pixelboard::loeschen() {
  oben.loeschen();
  unten.loeschen();
}

// Uebertraegt die Farbwerte an beide Panels (FastLED.show() sendet an alle angemeldeten Panels).
void Pixelboard::anzeigen() {
  FastLED.show();
}
