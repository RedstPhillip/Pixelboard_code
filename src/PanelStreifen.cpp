//////////////////////////////////////////////
// ---- PanelStreifen-Klasse (Implementierung) ----
// Deklaration und Koordinaten: PanelStreifen.h
//////////////////////////////////////////////

#include "PanelStreifen.h"

// Konstruktor: setzt Groesse (Breite = Panels * 32, Hoehe 8) und LED-Speicher fuer LEDMatrix,
// alle LEDs schwarz, kein Panel gedreht.
PanelStreifen::PanelStreifen(uint8_t anzahl) {
  anzahlPanels = constrain(anzahl, 1, MAX_PANELS);
  for (uint8_t i = 0; i < MAX_PANELS; i++) {
    gedreht[i] = false;
  }
  fill_solid(leds, MAX_PANELS * LEDS_PRO_PANEL, CRGB::Black);

  // Attribute der Basisklasse cLEDMatrixBase (LEDMatrix-Library)
  m_Width = anzahlPanels * PANEL_BREITE;
  m_Height = PANEL_HOEHE;
  m_Type = VERTICAL_ZIGZAG_MATRIX;
  m_LED = leds;
}

// Legt fest, ob Panel panelNr um 180° gedreht eingebaut ist.
void PanelStreifen::setGedreht(uint8_t panelNr, bool istGedreht) {
  if (panelNr < MAX_PANELS) gedreht[panelNr] = istGedreht;
}

// Liefert den Anfang des LED-Speichers von Panel panelNr (ungueltige Nummer: Panel 0).
CRGB* PanelStreifen::getLeds(uint8_t panelNr) {
  if (panelNr >= MAX_PANELS) panelNr = 0;
  return &leds[panelNr * LEDS_PRO_PANEL];
}

// Liefert die Anzahl der Panels im Streifen.
uint8_t PanelStreifen::getAnzahlPanels() {
  return anzahlPanels;
}

// Rechnet eine Streifen-Koordinate in den Index im LED-Speicher um.
//
// 1. Panel bestimmen: x 0..31 -> Panel 0, x 32..63 -> Panel 1; lokales x = x % 32.
// 2. y umdrehen: LEDText zaehlt y von unten, die Verdrahtung von oben (yOben = 7 - y).
// 3. Gedrehtes Panel (180°): x und y spiegeln.
// 4. Zick-Zack wie LedPanel::xyZuIndex (Test T7): LED 0 links oben,
//    gerade Spalten laufen nach unten, ungerade nach oben.
uint16_t PanelStreifen::mXY(uint16_t x, uint16_t y) {
  uint8_t panelNr = x / PANEL_BREITE;
  int lokalX = x % PANEL_BREITE;
  int yOben = (PANEL_HOEHE - 1) - y;

  if (gedreht[panelNr]) {
    lokalX = (PANEL_BREITE - 1) - lokalX;
    yOben = (PANEL_HOEHE - 1) - yOben;
  }

  int spaltenStart = lokalX * PANEL_HOEHE;
  int index;
  if (lokalX % 2 == 0) {
    index = spaltenStart + yOben;
  } else {
    index = spaltenStart + (PANEL_HOEHE - 1 - yOben);
  }
  return panelNr * LEDS_PRO_PANEL + index;
}
