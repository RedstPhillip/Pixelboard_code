# Projektanforderungen

Zurück zur [Übersicht](index.md).

## Projekt Pixelboard (aktuell)

### Hardware
- ESP32, Joystick, **2× LED-Array 8×32** (WS2812, Anordnung **Vertical Zig-Zag**) → Pins in [hardware.md](hardware.md)
- Die Panels werden **nicht** hintereinander gehängt: jedes Panel an einen eigenen Digitalausgang (separat ansteuerbar)
- **Schwierigkeit:** Das obere Panel ist gegenüber dem unteren um **180° gedreht** (die Anschlüsse zeigen zueinander)

### Libraries
- **FastLED** (Adafruit NeoPixel funktioniert nicht zuverlässig mit dem ESP32)
- **LEDMatrix** (GitHub AaronLiddiment)
- **LEDText** (GitHub AaronLiddiment)

### Mindest-Funktionalität
- Uhrzeit von Zeitserver abfragen und anzeigen
- Wetterdaten (mindestens Temperatur) von einem Wetterserver anzeigen
- Snake-Spiel
- Raumtemperatur und Luftfeuchtigkeit vom DHT22 anzeigen und in einer Google-Tabelle speichern

### Darüber hinaus (optional)
- Tetris, extra Joystick, …

### Aufgaben (jeweils **zuerst für 1 Panel, dann für 2**)
1. Laufschrift mit LEDText, die eine Variable enthält (z. B. Sekundenzähler)
2. Für Spiele: Methode, die einzelne LEDs über ein **x/y-Koordinatensystem** ansteuert

**Status:** Aufgabe 2 für 1 Panel umgesetzt (`LedPanel::setzePixel`), Aufgabe 1 noch nicht begonnen.

## Erstes Mini-Projekt: Joystick-Klasse (abgeschlossen)

Die Klasse `Taster` aus dem letzten Schuljahr (siehe [uml.md](uml.md))
wird zu einer Klasse für die Ansteuerung eines **Joysticks** erweitert.

Ein Joystick besteht aus:
- **1 Taster** (Druck auf den Stick) – digital
- **2 Potentiometern** (X-Achse, Y-Achse) – analog

## Ausgangslage: bestehende Klasse `Taster`

- Entprellung (50 ms) per `millis()`, nicht blockierend
- Erkennung von kurzem Druck (< 1000 ms) und langem Druck (≥ 1000 ms)
- `INPUT_PULLUP`, d. h. gedrückt = `LOW`
- Methoden: `check()`, `tasterGedrueckt()`, `tasterWurdeKurzGedrueckt()`, `tasterWurdeLangeGedrueckt()`

## Entscheidungen

| Datum | Thema | Entscheidung |
|---|---|---|
| 2026-10-13 | Hardware | ESP32, **ein** Joystick: SW 14, X 34, Y 35 → [hardware.md](hardware.md). Achsenwerte 0–4095 |
| 2026-10-13 | Klassenaufbau | Klasse `Taster` wird zu `Joystick` **umgebaut** (Tasterlogik + Achsen in einer Klasse) – keine Vererbung, keine Komposition |
| 2026-10-13 | Projekt/Werkzeug | VS Code + PlatformIO → [platformio.md](platformio.md) |
| 2026-10-13 | Dateistruktur | Klasse in `src/Joystick.h` (Deklaration) + `src/Joystick.cpp` (Implementierung), Testprogramm in `src/main.cpp`. Alles in `src/` statt `include/`, damit es nur einen Code-Ordner gibt |
| 2026-10-13 | Programm | Schlankes Testprogramm: Werte im Serial Monitor, kein RemoteXY/NeoPixel. Alter Code vom Vorjahr gelöscht (2026-10-13, vor Veröffentlichung auf GitHub) |
| 2026-10-13 | Umfang | Joystick: **keine neuen Features**, nur Rohwerte + Taster. Klasse `Joystick` bleibt unverändert |
| 2026-10-13 | Koordinatensystem | **(0,0) = links oben**, x nach rechts (0..31), y nach unten (0..7) – wie bei Bildschirmen, praktisch für Tetris/Snake |
| 2026-10-13 | x/y-Umrechnung | Eigene Methode in Klasse `LedPanel` (nicht LEDMatrix), da die Umrechnung die Aufgabe ist. Pin als Template-Parameter (`begin<16>()`), weil FastLED ihn beim Kompilieren braucht |
| 2026-10-13 | Pixelboard | Vorerst nur `main.cpp` bearbeiten: Testprogramm für **ein** Panel an GPIO 16. FastLED fest auf **3.7.8** (3.10.x kompiliert nicht) |

## Offene Fragen (erst bei Bedarf klären)

1. **Wertebereich der Achsen:** Rohwerte 0–4095 beibehalten, auf -100…+100 umrechnen oder Richtungen
   (links/rechts/oben/unten/Mitte)?
2. **Totzone / Kalibrierung:** Mittelstellung beim Start kalibrieren? Totzone um die Mitte?
