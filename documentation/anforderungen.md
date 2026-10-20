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

**Status:** Beide Aufgaben für 1 und für **2 Panels** umgesetzt, jede mit eigenem Hauptprogramm:
Aufgabe 1 in `src/aufgabe1/main.cpp` (Klasse `Laufschrift`), Aufgabe 2 in `src/aufgabe2/main.cpp` (Klasse `Pixelboard`, 32×16).
Zusätzlich Snake (Aufgabe 3) in `src/aufgabe3/main.cpp` auf dem `Pixelboard`.

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
| 2026-10-13 | Aufteilung Aufgaben | Projekte von Philipp (Aufgabe 2) und Dominika (Aufgabe 1) zusammengeführt. Jede Aufgabe hat ein eigenes Hauptprogramm (`src/aufgabe1/`, `src/aufgabe2/`) und eine eigene PlatformIO-Umgebung (`env:aufgabe1`, `env:aufgabe2`, per `build_src_filter`); Klassen gemeinsam in `src/` |
| 2026-10-13 | Aufgabe 1 | Klasse `Laufschrift` nutzt **LEDMatrix** (Umrechnung) + **LEDText** (Laufschrift), unabhängig von `LedPanel`. Text „Zeit: X s“, grün, 50 ms pro Spalte; die Zahl zählt jede Sekunde **während** des Durchlaufs hoch (`aendereText()`), Text fest 12 Zeichen lang |
| 2026-10-20 | Aufgabe 1, 2 Panels | Text läuft **zuerst über das obere Panel und dann unten weiter** (Wunsch Dominika). Umsetzung: beide Panels als ein Streifen 64×8 (neue Klasse `PanelStreifen`, Unterklasse von `cLEDMatrixBase`), x 0..31 = unten, 32..63 = oben. Pins: oben **25**, unten **26** (verkabelt). Drehung per Konstante `OBEN_GEDREHT`/`UNTEN_GEDREHT` (ursprünglich oben gedreht angenommen; am 2026-10-20 nach Test T12–T13a korrigiert: oben nicht, unten gedreht). Umschalten 1/2 Panels mit `ANZAHL_PANELS` |
| 2026-10-20 | Aufgabe 3 (Snake) | Eigene Klasse `Snake` (`src/Snake.h`/`.cpp`) + Hauptprogramm `src/aufgabe3/main.cpp` → `env:aufgabe3`. Ursprünglich ein Panel 32×8 über `LedPanel`; **seit 2026-10-20 auf `Pixelboard` 32×16 (2 Panels, Pins 25/26) umgestellt**, Steuerung über `Joystick` (Schwellen 1500/2500 wegen Ruhelage ~1800–2000), Futter frisst/wächst, Tempo steigt ab 80 ms, Game Over bei Wand/Selbstkollision, Neustart per kurzem Tastendruck |
| 2026-10-20 | Aufgabe 2, 2 Panels | `LedPanel` bleibt **ein** Panel und bekommt die Option „180° gedreht“ (Konstruktor-Parameter). Neue Klasse `Pixelboard` enthält zwei `LedPanel` (Komposition) und bietet **eine** Fläche 32×16: (0,0) links oben, y 0..7 oben, y 8..15 unten. Pins oben 25, unten 26; Drehung per `OBEN_GEDREHT`/`UNTEN_GEDREHT` umstellbar. Laut Test T12/T12a ist **das untere Panel gedreht, das obere nicht** (oben `false`, unten `true`); Standard im Konstruktor entsprechend. Drehung wird gleich gerechnet wie in `PanelStreifen` (x und y spiegeln). Benutzung vereinfacht: `board.begin()` mit Standard-Pins 25/26, Helligkeit 20 und 500 mA Strombegrenzung in der Klasse |
| 2026-10-20 | Snake aufgeräumt | Richtungen als `enum Snake::Richtung`, Spielfeld fest = `Pixelboard` 32×16, `neustart()` statt `init()`. Joystick: gewertet wird die stärker ausgelenkte Achse (Totzone 1500–2500), damit eine ignorierte Umkehr nicht die andere Achse blockiert. Tastendrücke während des Spiels werden verworfen (kein ungewollter Neustart) |
| 2026-10-20 | Snake mit FreeRTOS | Hauptprogramm von Aufgabe 3 in **zwei Tasks** aufgeteilt: `tasterTask` (Priorität 2) liest Taster und Achsen alle 10 ms, `spielTask` (Priorität 1) bewegt und zeichnet. Übergabe nur über `volatile bool neustartAngefordert`; `Snake`/`Pixelboard` nur im Spiel-Task (kein Mutex). `loop()` wird mit `vTaskDelete(NULL)` beendet |
| 2026-10-13 | Upload-Port | `upload_port`/`monitor_port` nicht mehr fest (COM6), sondern automatische Erkennung – der Port ist auf jedem PC anders |

## Offene Fragen

Zurzeit keine. (Die früheren Fragen zu Wertebereich/Totzone des Joysticks sind durch die Snake-Steuerung erledigt.)
