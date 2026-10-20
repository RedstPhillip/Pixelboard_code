# TO-DO-Liste / Projektfortschritt

Zurück zur [Übersicht](index.md).

## Erledigt

- [x] 2026-10-13 – `AGENTS.md` und Ordner `documentation/` mit Grunddateien angelegt
- [x] 2026-10-13 – `AGENTS.md` als Startdatei erweitert: Beschreibung aller MD-Dateien und Pflicht zur selbständigen Aktualisierung
- [x] 2026-10-13 – [hardware.md](hardware.md) angelegt: ESP32-Pinbelegung, kritische Pins; Board-/Pin-Frage geklärt
- [x] 2026-10-13 – Schritt 1: Klasse `Taster` zu `Joystick` umgebaut (jetzt `src/main.cpp`): Taster + Rohwerte X/Y,
      Testprogramm mit beiden Joysticks über Serial Monitor (Test T1, T2 in [test-log.md](test-log.md))
- [x] 2026-10-13 – Auf VS Code + PlatformIO umgestellt (`platformio.ini`, `src/main.cpp`), Build erfolgreich (T0),
      Anleitung in [platformio.md](platformio.md)
- [x] 2026-10-13 – Aufgeräumt: leere Ordner `include/`, `lib/`, `test/` entfernt, `Taster.ino` nach `documentation/alt/`
      verschoben; Upload-Port COM6 in `platformio.ini` eingetragen; Schritt-für-Schritt-Anleitung in [platformio.md](platformio.md)
- [x] 2026-10-13 – Code aufgeräumt (ohne neue Funktionen): Pin-Konfiguration aus dem Konstruktor in `begin()`,
      Zeitkonstanten `static const` + `UPPER_SNAKE_CASE`, doppelte Ausgaben in Hilfsfunktionen; Build ✅
- [x] 2026-10-13 – Nur noch ein Joystick (SW 14, X 34, Y 35): externer Joystick aus Testprogramm entfernt,
      Hilfsfunktionen dadurch überflüssig und wieder entfernt; Build ✅
- [x] 2026-10-13 – Tests T1, T2, T4 bestanden: Joystick-Testprogramm läuft auf dem ESP32
- [x] 2026-10-13 – Klasse in `src/Joystick.h` + `src/Joystick.cpp` aufgeteilt, `main.cpp` enthält nur noch das Testprogramm; Build ✅ (T5)
- [x] 2026-10-13 – Pixelboard: Libraries FastLED (3.7.8), LEDMatrix, LEDText eingebunden; `main.cpp` = Testprogramm
      für ein Panel an GPIO 16 (Farbtest, Pixel-Lauf, LEDMatrix, LEDText); Build ✅ (T6)
- [x] 2026-10-13 – Aufgeräumt: Panel-Logik in Klasse `LedPanel` (`.h`/`.cpp`) ausgelagert, `main.cpp` nur Testprogramm
- [x] 2026-10-13 – Aufgabe 2 (1 Panel): `LedPanel::setzePixel(x, y, farbe)` mit eigener Zig-Zag-Umrechnung; Build ✅ (T7)
- [x] 2026-10-13 – Alten Code (`documentation/alt/`) gelöscht; Projekt als Git-Repository auf GitHub veröffentlicht
      (public: github.com/RedstPhillip/Pixelboard_code)
- [x] 2026-10-13 – Aufgabe 1 (1 Panel): Klasse `Laufschrift` (LEDText/LEDMatrix), Laufschrift „Zeit: X s“, Zahl zählt
      während des Durchlaufs hoch (von Dominika, in eigenem Projekt entwickelt und getestet)
- [x] 2026-10-13 – Projekte zusammengeführt: `src/aufgabe1/main.cpp` (Laufschrift) und `src/aufgabe2/main.cpp` (x/y),
      je eigene PlatformIO-Umgebung; Upload-Port automatisch statt COM6; Build beider Umgebungen ✅ (T8)
- [x] 2026-10-20 – Aufgabe 1 für **2 Panels**: neue Klasse `PanelStreifen` (2 Panels als 64×8-Streifen, 180°-Drehung),
      `Laufschrift` umgebaut (Panelanzahl wählbar); Text läuft oben, dann unten weiter; Build beider Umgebungen ✅ (T9a, T9b)
- [x] 2026-10-20 – Aufgabe 3: Snake-Spiel hinzugefügt (`src/aufgabe3/main.cpp`, Klasse `Snake.h`/`.cpp`, `env:aufgabe3`)
- [x] 2026-10-20 – Aufgabe 2 für 2 Panels: `LedPanel` um 180°-Drehung erweitert, neue Klasse `Pixelboard` (Fläche 32×16,
      zwei `LedPanel`), Testprogramm auf GPIO 25/26 umgestellt; Build beider Umgebungen ✅ (T12). T7a/T8a (1 Panel an GPIO 16) entfallen
- [x] 2026-10-20 – Defekt oberes Panel (letzte ~40 LEDs) als Hardwarefehler gefunden und behoben (T13); Aufgabe 2 auf 2 Panels läuft (T12b)
- [x] 2026-10-20 – `Pixelboard` aufgeräumt: `begin()` ohne Parameter (Pins 25/26), Helligkeit + Strombegrenzung in der Klasse,
      `setHelligkeit()`; Testprogramm ohne Diagnose-Phase; Build aufgabe1/2/3 ✅ (T14)
- [x] 2026-10-20 – Snake auf `Pixelboard` umgestellt (Spielfeld 32×16 über beide Panels, Pins 25/26 statt GPIO 16); Build ✅ (T15)
- [x] 2026-10-20 – Joystick-Problem bei Snake war ein Kabelfehler (VCC); Snake läuft (T15a)
- [x] 2026-10-20 – Aufgeräumt: Snake-Klasse (Richtungen als `enum`, Konstanten, Methodenkommentare, ungenutzte Methoden
      entfernt, keine Umkehr bei schnellen Eingaben), Snake-Hauptprogramm ohne Diagnose-Ausgabe und ohne `delay()`,
      Joystick-Auswertung nach stärker ausgelenkter Achse; Aufgabe 1 Drehung korrigiert (oben nicht, unten gedreht);
      veraltete Doku (GPIO 16, externer Joystick, Joystick-Fragen) entfernt; Build aufgabe1/2/3 ✅ (T16)

## Offen

### Pixelboard
- [ ] Test T16 durchführen (Snake nach dem Aufräumen) und T9b (Aufgabe 1 auf 2 Panels, Drehung korrigiert)
- [ ] Uhrzeit (Zeitserver), Wetter (Temperatur), DHT22 + Google-Tabelle
- [ ] Optional: Tetris, zweiter Joystick

### Sonstiges
- [ ] DHT22 an GPIO 13 ausprobieren (siehe [hardware.md](hardware.md))
