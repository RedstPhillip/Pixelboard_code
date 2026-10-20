//////////////////////////////////////////////
// ---- Snake-Klasse (Implementierung) ----
// Deklaration: Snake.h
//////////////////////////////////////////////

#include "Snake.h"
#include "LedPanel.h"

Snake::Snake(int b, int h) {
  breite = b;
  hoehe = h;
  laenge = 0;
  richtung = 0;
  naechsteRichtung = 0;
  futterVorhanden = false;
  gameOver = false;
  gestartet = false;
  letzteBewegung = 0;
  geschwindigkeitMs = 150;
  futter.x = 0; futter.y = 0;
  // koerper initial leer
}

void Snake::init(int startX, int startY) {
  laenge = 3;
  // Schlange waagerecht nach rechts, Kopf an (startX,startY)
  for (int i = 0; i < laenge; i++) {
    koerper[i].x = startX - i; // i==0 Kopf
    koerper[i].y = startY;
  }
  richtung = 0;        // rechts
  naechsteRichtung = 0;
  gameOver = false;
  gestartet = true;
  letzteBewegung = millis();
  futterVorhanden = false;
  erzeugeFutter();
}

void Snake::erzeugeFutter() {
  // versuche, freies Feld zu finden
  int versuche = 0;
  while (versuche < 500) {
    int fx = random(0, breite);
    int fy = random(0, hoehe);
    // pruefe, ob auf Schlange
    bool aufSchlange = false;
    for (int i = 0; i < laenge; i++) {
      if (koerper[i].x == fx && koerper[i].y == fy) { aufSchlange = true; break; }
    }
    if (!aufSchlange) {
      futter.x = fx; futter.y = fy;
      futterVorhanden = true;
      return;
    }
    versuche++;
  }
  // falls kein freies Feld gefunden (sehr unwahrscheinlich)
  futterVorhanden = false;
}

// Setzt die naechste Richtung. Direkte Umkehr (z. B. rechts -> links) wird ignoriert.
void Snake::setzeRichtung(int neueRichtung) {
  if (neueRichtung < 0) neueRichtung = 0;
  if (neueRichtung > 3) neueRichtung = 3;
  // Gegenrichtung von naechsteRichtung ignorieren
  if ((naechsteRichtung + 2) % 4 == neueRichtung) return;
  naechsteRichtung = neueRichtung;
}

void Snake::bewegen() {
  if (!gestartet || gameOver) return;
  if (millis() - letzteBewegung < geschwindigkeitMs) return;
  letzteBewegung = millis();

  richtung = naechsteRichtung;

  // neuen Kopf berechnen
  int kopfX = koerper[0].x;
  int kopfY = koerper[0].y;
  int nx = kopfX;
  int ny = kopfY;
  if (richtung == 0) nx = kopfX + 1; // rechts
  if (richtung == 1) ny = kopfY + 1; // unten
  if (richtung == 2) nx = kopfX - 1; // links
  if (richtung == 3) ny = kopfY - 1; // oben

  // Kollision mit Wand?
  if (nx < 0 || nx >= breite || ny < 0 || ny >= hoehe) {
    gameOver = true;
    return;
  }

  // Kollision mit eigenem Koerper?
  for (int i = 0; i < laenge; i++) {
    if (koerper[i].x == nx && koerper[i].y == ny) {
      gameOver = true;
      return;
    }
  }

  // pruefen, ob Futter gefressen wird
  bool frisst = false;
  if (futterVorhanden && nx == futter.x && ny == futter.y) {
    frisst = true;
  }

  // Koerper verschieben: von hinten nach vorne.
  // Beim Fressen wird das letzte Glied verdoppelt, bevor geschoben wird –
  // sonst wuerde ein ungueltiger Speicherwert gezeichnet.
  if (frisst && laenge < MAX_KOERPER) {
    koerper[laenge] = koerper[laenge - 1];
    laenge++;
  }
  for (int i = laenge - 1; i > 0; i--) {
    koerper[i] = koerper[i - 1];
  }
  koerper[0].x = nx;
  koerper[0].y = ny;

  if (frisst) {
    // neues Futter erzeugen
    erzeugeFutter();
    // leicht schneller werden (optional)
    if (geschwindigkeitMs > 80) geschwindigkeitMs -= 2;
  }
}

void Snake::zeichnen(class LedPanel &panel, CRGB schlangeFarbe, CRGB futterFarbe) {
  if (!gestartet) return;
  // Schlange zeichnen
  for (int i = 0; i < laenge; i++) {
    panel.setzePixel(koerper[i].x, koerper[i].y, schlangeFarbe);
  }
  // Futter zeichnen
  if (futterVorhanden) {
    panel.setzePixel(futter.x, futter.y, futterFarbe);
  }
  // Game Over: Kopf kurz andersfarbig markieren (optional) – hier nicht noetig
}

bool Snake::istGameOver() { return gameOver; }
bool Snake::istGestartet() { return gestartet; }
int Snake::getLaenge() { return laenge; }
unsigned long Snake::getGeschwindigkeitMs() { return geschwindigkeitMs; }
void Snake::setzeGeschwindigkeitMs(unsigned long v) { if (v >= 50) geschwindigkeitMs = v; }

