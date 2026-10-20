//////////////////////////////////////////////
// ---- Snake-Klasse (Implementierung) ----
// Deklaration und Benutzung: Snake.h
//////////////////////////////////////////////

#include "Snake.h"

// Konstruktor: legt das Spiel an, ohne es zu starten (noch kein Hardwarezugriff).
Snake::Snake() {
  laenge = 0;
  richtung = RECHTS;
  naechsteRichtung = RECHTS;
  futter = { 0, 0 };
  gameOver = true;   // bis neustart() aufgerufen wird
  tempoMs = START_TEMPO_MS;
  letzterSchritt = 0;
}

// Startet ein neues Spiel: Schlange (3 Glieder) waagerecht in der Mitte, Kopf rechts, faehrt nach rechts.
void Snake::neustart() {
  laenge = START_LAENGE;
  for (int i = 0; i < laenge; i++) {
    koerper[i] = { BREITE / 2 - i, HOEHE / 2 };
  }
  richtung = RECHTS;
  naechsteRichtung = RECHTS;
  gameOver = false;
  tempoMs = START_TEMPO_MS;
  letzterSchritt = millis();
  neuesFutter();
}

// Liefert true, wenn der Punkt (x, y) von der Schlange belegt ist.
bool Snake::istAufSchlange(int x, int y) {
  for (int i = 0; i < laenge; i++) {
    if (koerper[i].x == x && koerper[i].y == y) return true;
  }
  return false;
}

// Setzt das Futter auf ein zufaelliges freies Feld (so lange wuerfeln, bis es nicht auf der Schlange liegt).
void Snake::neuesFutter() {
  do {
    futter = { (int)random(BREITE), (int)random(HOEHE) };
  } while (istAufSchlange(futter.x, futter.y));
}

// Merkt sich die naechste Richtung. Direkte Umkehr wird ignoriert, weil die Schlange sonst in sich selbst faehrt.
// Verglichen wird mit der aktuellen Richtung (nicht mit naechsteRichtung), damit zwei schnelle Eingaben
// innerhalb eines Schritts (z. B. oben, dann links) keine Umkehr erlauben.
void Snake::setzeRichtung(Richtung neueRichtung) {
  if ((richtung + 2) % 4 == neueRichtung) return;
  naechsteRichtung = neueRichtung;
}

// Bewegt die Schlange um ein Feld, sobald tempoMs vergangen ist.
void Snake::bewegen() {
  if (gameOver || millis() - letzterSchritt < tempoMs) return;
  letzterSchritt = millis();
  richtung = naechsteRichtung;

  // neuer Kopf
  Punkt kopf = koerper[0];
  switch (richtung) {
    case RECHTS: kopf.x++; break;
    case UNTEN:  kopf.y++; break;
    case LINKS:  kopf.x--; break;
    case OBEN:   kopf.y--; break;
  }

  // Kollision mit Wand oder eigenem Koerper
  if (kopf.x < 0 || kopf.x >= BREITE || kopf.y < 0 || kopf.y >= HOEHE || istAufSchlange(kopf.x, kopf.y)) {
    gameOver = true;
    return;
  }

  // Beim Fressen waechst die Schlange: das letzte Glied bleibt stehen (wird vor dem Verschieben verdoppelt)
  bool frisst = (kopf.x == futter.x && kopf.y == futter.y);
  if (frisst && laenge < MAX_LAENGE) {
    koerper[laenge] = koerper[laenge - 1];
    laenge++;
  }

  // Koerper von hinten nach vorne nachziehen, dann neuen Kopf setzen
  for (int i = laenge - 1; i > 0; i--) {
    koerper[i] = koerper[i - 1];
  }
  koerper[0] = kopf;

  if (frisst) {
    if (tempoMs > MIN_TEMPO_MS) tempoMs -= BESCHLEUNIGUNG_MS;
    if (laenge < MAX_LAENGE) neuesFutter();
  }
}

// Zeichnet Schlange und Futter auf das Pixelboard. Sichtbar erst nach board.anzeigen().
void Snake::zeichnen(Pixelboard &board, CRGB schlangeFarbe, CRGB futterFarbe) {
  for (int i = 0; i < laenge; i++) {
    board.setzePixel(koerper[i].x, koerper[i].y, schlangeFarbe);
  }
  board.setzePixel(futter.x, futter.y, futterFarbe);
}

// Liefert true nach einer Wand- oder Selbstkollision.
bool Snake::istGameOver() {
  return gameOver;
}

// Liefert die aktuelle Laenge der Schlange.
int Snake::getLaenge() {
  return laenge;
}
