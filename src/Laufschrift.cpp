//////////////////////////////////////////////
// ---- Laufschrift-Klasse (Implementierung) ----
// Deklaration und Beschreibung: Laufschrift.h
//////////////////////////////////////////////

#include "Laufschrift.h"
#include <FontMatrise.h>  // Schriftart 5x7 Pixel aus der LEDText-Bibliothek

// Konstruktor: legt den Streifen (1 oder 2 Panels) an, speichert die Laufgeschwindigkeit und setzt Startwerte.
// Fuehrende Leerzeichen: so viele, dass der Text rechts in einen leeren Streifen hineinlaeuft und der Rest
// des vorherigen Textes hinausgeschoben wird (Streifenbreite / 6 Spalten pro Zeichen + 1: 1 Panel 6, 2 Panels 11).
Laufschrift::Laufschrift(unsigned long schrittZeitMs, uint8_t anzahlPanels) : streifen(anzahlPanels) {
  fuehrendeLeerzeichen = streifen.Width() / 6 + 1;
  schrittZeit = schrittZeitMs;
  letzterSchrittZeit = 0;
  textLaenge = 0;
  textGesetzt = false;
  durchlaufFertig = false;
  textPuffer[0] = '\0';
}

// Liefert den LED-Speicher von Panel panelNr fuer FastLED.addLeds().
CRGB* Laufschrift::getLeds(uint8_t panelNr) {
  return streifen.getLeds(panelNr);
}

// Legt fest, ob Panel panelNr um 180° gedreht eingebaut ist (Weitergabe an den Streifen).
void Laufschrift::setGedreht(uint8_t panelNr, bool istGedreht) {
  streifen.setGedreht(panelNr, istGedreht);
}

// Stellt Schriftart, Textbereich (ganzer Streifen) und Textfarbe ein.
void Laufschrift::begin(uint8_t rot, uint8_t gruen, uint8_t blau) {
  text.SetFont(MatriseFontData);
  // Textbereich: ganze Streifenbreite (32 oder 64), Schrifthoehe + 1 (= 8), Ursprung links unten
  text.Init(&streifen, streifen.Width(), text.FontHeight() + 1, 0, 0);
  text.SetTextColrOptions(COLR_RGB | COLR_SINGLE, rot, gruen, blau);
}

// Kopiert den neuen Text (mit fuehrenden Leerzeichen) in den eigenen Puffer und startet den Durchlauf neu.
void Laufschrift::setText(const char* neuerText) {
  textLaenge = 0;
  for (uint8_t i = 0; i < fuehrendeLeerzeichen; i++) {
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

  for (uint16_t i = fuehrendeLeerzeichen; i < textLaenge; i++) {
    if (*neuerText != '\0') {
      textPuffer[i] = *neuerText++;
    } else {
      textPuffer[i] = ' ';
    }
  }
}

// Schiebt den Text alle schrittZeit ms um eine Spalte weiter und sendet das Bild an die Panels
// (FastLED.show() sendet an alle angemeldeten Panels).
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
