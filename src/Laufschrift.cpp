//////////////////////////////////////////////
// ---- Laufschrift-Klasse (Implementierung) ----
// Deklaration und Beschreibung: Laufschrift.h
//////////////////////////////////////////////

#include "Laufschrift.h"
#include <FontMatrise.h>  // Schriftart 5x7 Pixel aus der LEDText-Bibliothek

// Anzahl Leerzeichen vor dem Text, damit er rechts in ein leeres Panel hineinlaeuft
// und der Rest des vorherigen Textes hinausgeschoben wird (32 Spalten / 6 Spalten pro Zeichen).
static const uint8_t FUEHRENDE_LEERZEICHEN = 6;

// Konstruktor: speichert nur die Laufgeschwindigkeit und setzt Startwerte (noch kein Hardwarezugriff).
Laufschrift::Laufschrift(unsigned long schrittZeitMs) {
  schrittZeit = schrittZeitMs;
  letzterSchrittZeit = 0;
  textLaenge = 0;
  textGesetzt = false;
  durchlaufFertig = false;
  textPuffer[0] = '\0';
}

// Liefert den LED-Speicher des Panels (Zeiger auf die erste LED) fuer FastLED.addLeds().
CRGB* Laufschrift::getLeds() {
  return matrix[0];
}

// Liefert die Anzahl der LEDs des Panels.
int Laufschrift::getAnzahlLeds() {
  return matrix.Size();
}

// Stellt Schriftart, Textbereich (ganzes Panel) und Textfarbe ein.
void Laufschrift::begin(uint8_t rot, uint8_t gruen, uint8_t blau) {
  text.SetFont(MatriseFontData);
  // Textbereich: ganze Breite, Schrifthoehe + 1 (= 8), Ursprung links unten
  text.Init(&matrix, matrix.Width(), text.FontHeight() + 1, 0, 0);
  text.SetTextColrOptions(COLR_RGB | COLR_SINGLE, rot, gruen, blau);
}

// Kopiert den neuen Text (mit fuehrenden Leerzeichen) in den eigenen Puffer und startet den Durchlauf neu.
void Laufschrift::setText(const char* neuerText) {
  textLaenge = 0;
  for (uint8_t i = 0; i < FUEHRENDE_LEERZEICHEN; i++) {
    textPuffer[textLaenge++] = ' ';
  }
  while (*neuerText != '\0' && textLaenge < MAX_TEXT_LAENGE) {
    textPuffer[textLaenge++] = *neuerText++;
  }
  textPuffer[textLaenge] = '\0';

  // SetText() muss nach SetTextColrOptions() kommen, sonst wird die Farbe nicht uebernommen
  text.SetText(textPuffer, textLaenge);
  textGesetzt = true;
  durchlaufFertig = false;
}

// Ueberschreibt den Text im Puffer (nach den fuehrenden Leerzeichen), Laenge und Position bleiben gleich.
// Funktioniert, weil LEDText bei jedem UpdateText() die sichtbaren Zeichen neu aus dem Puffer liest.
void Laufschrift::aendereText(const char* neuerText) {
  if (!textGesetzt) return;

  for (uint16_t i = FUEHRENDE_LEERZEICHEN; i < textLaenge; i++) {
    if (*neuerText != '\0') {
      textPuffer[i] = *neuerText++;
    } else {
      textPuffer[i] = ' ';
    }
  }
}

// Schiebt den Text alle schrittZeit ms um eine Spalte weiter und sendet das Bild an das Panel.
void Laufschrift::update() {
  if (!textGesetzt) return;

  unsigned long jetzt = millis();
  if (jetzt - letzterSchrittZeit < schrittZeit) return;
  letzterSchrittZeit = jetzt;

  // UpdateText() liefert -1, wenn das Textende erreicht ist (Bild bleibt dann unveraendert)
  if (text.UpdateText() == -1) {
    durchlaufFertig = true;
  } else {
    FastLED.show();
  }
}

// Liefert einmalig true nach einem kompletten Durchlauf und setzt das Flag zurueck.
bool Laufschrift::wurdeDurchlaufBeendet() {
  if (durchlaufFertig) {
    durchlaufFertig = false;
    return true;
  }
  return false;
}
