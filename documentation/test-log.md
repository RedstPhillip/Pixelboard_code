# Test-Log

Zurück zur [Übersicht](index.md).

Jede Programmänderung bekommt eine Testanweisung. Der Benutzer führt den Test durch, die KI trägt das Ergebnis ein.

| Nr. | Datum | Schritt / Änderung | Testanweisung | Erwartetes Ergebnis | Status / Ergebnis |
|---|---|---|---|---|---|
| T0 | 2026-10-13 | Umstellung auf PlatformIO | `pio run` (Kompilieren) | Build ohne Fehler | ✅ erledigt (KI, 2026-10-13): SUCCESS, RAM 6,6 %, Flash 21,0 % |
| T0b | 2026-10-13 | Upload auf ESP32 (COM6) | `pio run -t upload` | Upload SUCCESS | ✅ 2026-10-13: erster Versuch (KI) „Wrong boot mode“; mit BOOT-Taste durch Benutzer erfolgreich |
| T1 | 2026-10-13 | Schritt 1: Klasse `Joystick`, Achsen | In VS Code Upload (→) ausführen, dann Serial Monitor (🔌) öffnen. Joystick in alle Richtungen bewegen. | Ruhestellung ca. 1800–2000; Anschlag ca. 0 bzw. 4095; X und Y reagieren auf die richtige Bewegungsrichtung | ✅ 2026-10-13: vom Benutzer bestätigt |
| T2 | 2026-10-13 | Schritt 1: Klasse `Joystick`, Taster | Joystick-Taster kurz (< 1 s) und lang (≥ 1 s) drücken. | Kurz: genau eine Meldung „kurz gedrueckt“ beim Loslassen. Lang: genau eine Meldung „lang gedrueckt“ nach 1 s, beim Loslassen keine weitere Meldung | ✅ 2026-10-13: vom Benutzer bestätigt |
| T3 | 2026-10-13 | Aufräumen: `begin()`, Konstanten `static`, Ausgabe-Hilfsfunktionen | Neu hochladen, Serial Monitor öffnen, T1 und T2 wiederholen. | Verhalten identisch zu vorher; Ausgabezeile `Joystick X: … Y: …  \|  Extern X: … Y: …` | Build ✅; durch T4 ersetzt (Testprogramm auf einen Joystick reduziert) |
| T4 | 2026-10-13 | Nur ein Joystick (SW 14, X 34, Y 35) | Neu hochladen, T1 und T2 durchführen. | Ausgabe `X: … Y: …`; Tastermeldungen „Taster: kurz/lang gedrueckt“ | ✅ 2026-10-13: vom Benutzer bestätigt |
| T5 | 2026-10-13 | Aufteilung in `Joystick.h` / `Joystick.cpp` / `main.cpp` | Neu hochladen, Joystick bewegen und Taster drücken. | Verhalten identisch zu T4 | Build ✅ (KI); Hardwaretest offen |
| T6 | 2026-10-13 | Pixelboard: Panel-Testprogramm (1 Panel, GPIO 16) | Hochladen, Serial Monitor öffnen, Panel beobachten. Phasen laufen in Endlosschleife. | Phase 0: ganzes Panel rot → grün → blau (sonst Farbreihenfolge falsch). Phase 1: LEDs gehen einzeln an, erste LED rot – Weg zeigt Zig-Zag-Verlauf. Phase 2: blauer Rahmen, grüne Diagonale, roter Punkt = Ursprung (0,0). Phase 3: Laufschrift „LEDText OK“ läuft durch, lesbar (nicht gespiegelt/kopfüber) | ✅ 2026-10-13: vom Benutzer bestätigt („läuft wie Butter“) |
| T6a | 2026-10-13 | Upload Panel-Testprogramm | Upload | SUCCESS | ❌ „Packet content transfer stopped“ → vom Benutzer anders behoben, `upload_speed` nicht geändert |
| T7 | 2026-10-13 | Aufgabe 2: `LedPanel::setzePixel` (1 Panel, GPIO 16) | Hochladen, Panel so halten, dass die Laufschrift aus T6 lesbar war. | Phase 0: rot links oben, grün rechts oben, blau links unten, weiß rechts unten. Phase 1: oranger Punkt läuft in Zeile 0 von links nach rechts, dann Zeile 1 usw. bis rechts unten – ohne Sprünge | ❌ 2026-10-13: y gespiegelt (blau links oben, rot links unten, grün rechts unten) → LED 0 sitzt links oben, Formel korrigiert |
| T7a | 2026-10-13 | Korrektur `xyZuIndex` (y-Richtung) | wie T7 | wie T7 | Build ✅ (KI); Hardwaretest offen |
