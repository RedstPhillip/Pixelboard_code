//////////////////////////////////////////////
// ---- Joystick-Klasse (Implementierung) ----
// Deklaration und Beschreibung der Attribute: Joystick.h
//////////////////////////////////////////////

#include "Joystick.h"

// Konstruktor: speichert nur die Pins und setzt Startwerte (noch kein Hardwarezugriff).
// pinSwNr = Taster, pinXNr/pinYNr = Achsen (muessen ADC1-Pins sein, z. B. 32-35).
Joystick::Joystick(uint8_t pinSwNr, uint8_t pinXNr, uint8_t pinYNr) {
  pinSw = pinSwNr;
  pinX = pinXNr;
  pinY = pinYNr;
  aktuellerZustand = HIGH;
  letzterZustand = HIGH;
  letzteAenderungZeit = 0;
  gedruecktZeit = 0;
  wurdeKurz = false;
  wurdeLang = false;
  langMeldungSchonGesendet = false;
  xWert = 0;
  yWert = 0;
}

// Konfiguriert den Taster-Pin mit internem Pull-up. Einmal in setup() aufrufen.
void Joystick::begin() {
  pinMode(pinSw, INPUT_PULLUP);
}

// Aktualisiert Taster und Achsen. Muss in jedem loop()-Durchlauf aufgerufen werden.
void Joystick::check() {
  tasterPruefen();
  achsenLesen();
}

// Liest den Taster ein, entprellt das Signal und erkennt kurze bzw. lange Tastendruecke.
void Joystick::tasterPruefen() {
  bool gelesen = digitalRead(pinSw);
  unsigned long jetzt = millis();

  if (gelesen != letzterZustand) letzteAenderungZeit = jetzt;

  if ((jetzt - letzteAenderungZeit) > ENTPRELL_ZEIT) {
    if (gelesen != aktuellerZustand) {
      aktuellerZustand = gelesen;
      if (aktuellerZustand == LOW) {
        gedruecktZeit = jetzt;
        wurdeKurz = false;
        wurdeLang = false;
        langMeldungSchonGesendet = false;
      } else {
        if (!langMeldungSchonGesendet) {
          unsigned long gedruecktDauer = jetzt - gedruecktZeit;
          if (gedruecktDauer < LANG_DRUCK_ZEIT && gedruecktDauer > ENTPRELL_ZEIT)
            wurdeKurz = true;
        }
      }
    }
  }

  if (aktuellerZustand == LOW && !langMeldungSchonGesendet) {
    if (jetzt - gedruecktZeit >= LANG_DRUCK_ZEIT) {
      wurdeLang = true;
      langMeldungSchonGesendet = true;
    }
  }

  letzterZustand = gelesen;
}

// Liest die Rohwerte (0..4095) der beiden Potentiometer ein.
void Joystick::achsenLesen() {
  xWert = analogRead(pinX);
  yWert = analogRead(pinY);
}

// Liefert true, solange der Taster gedrueckt ist.
bool Joystick::tasterGedrueckt() {
  return (aktuellerZustand == LOW);
}

// Liefert einmalig true nach einem kurzen Tastendruck (< 1 s), danach wieder false.
bool Joystick::tasterWurdeKurzGedrueckt() {
  if (wurdeKurz) { wurdeKurz = false; return true; }
  return false;
}

// Liefert einmalig true, sobald der Taster 1 s lang gedrueckt gehalten wurde.
bool Joystick::tasterWurdeLangeGedrueckt() {
  if (wurdeLang) { wurdeLang = false; return true; }
  return false;
}

// Liefert den zuletzt gelesenen Rohwert der X-Achse (0..4095).
int Joystick::getX() {
  return xWert;
}

// Liefert den zuletzt gelesenen Rohwert der Y-Achse (0..4095).
int Joystick::getY() {
  return yWert;
}
