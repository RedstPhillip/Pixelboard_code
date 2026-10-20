# Hardware

Zurück zur [Übersicht](index.md).

Diese Datei hält alle Hardware-Informationen (Board, Pinbelegung, Einschränkungen) aktuell.
Bei jeder Änderung an Verkabelung oder Pins wird sie von der KI selbständig aktualisiert.

## Board

- **Mikrocontroller:** ESP32 (Arduino-Framework)
- **ADC-Auflösung:** 12 Bit → Analogwerte **0–4095** (nicht 0–1023 wie beim Arduino Uno)
- **Logikpegel:** 3,3 V

## Pinbelegung

| ESP32-Pin | Anschluss | Pin-Typ / Hinweis |
|---|---|---|
| 14 | Joystick SW (Taster) | digitaler Eingang mit `INPUT_PULLUP` (ADC2-Pin, digital auch mit WLAN problemlos); wird beim Booten kurz HIGH gesetzt (als Eingang unkritisch) |
| 3,3 V | Joystick „5 V“ (VCC) | Versorgung bewusst mit 3,3 V, damit die Achsen-Spannung den ADC-Bereich (max. 3,3 V) nicht übersteigt |
| GND | Joystick GND | – |
| 34 | Joystick X | ADC1, nur Eingang, kein interner Pull-up/-down |
| 35 | Joystick Y | ADC1, nur Eingang, kein interner Pull-up/-down |
| 25 | Data-Pin **oberes** LED-Panel | Ausgang (ADC2-Pin, als digitaler Ausgang unproblematisch) |
| 26 | Data-Pin **unteres** LED-Panel | Ausgang (ADC2-Pin, als digitaler Ausgang unproblematisch) |
| 13 (?) | DHT22 Data | **noch auszuprobieren** |

Freie ADC1-Pins (z. B. für einen zweiten Joystick): 32, 33, 36 (SVP), 39 (SVN).

## LED-Panels

- 2× 8×32 LEDs, Typ **WS2812** (Farbreihenfolge GRB), Anordnung **Vertical Zig-Zag**
- Jedes Panel an einem **eigenen** Data-Pin (nicht hintereinander gehängt)
- **Verdrahtung eines nicht gedrehten Panels (von vorne):** LED 0 sitzt **links oben**; Spalte 0 läuft nach unten (0..7),
  Spalte 1 nach oben (8..15), usw. (Test T7)
- **Drehung (Tests T12–T13a, 2026-10-20):** Die Panels sind so montiert, dass die Anschlüsse zueinander zeigen.
  Elektrisch ist dabei das **obere Panel nicht gedreht** (LED 0 links oben) und das **untere um 180° gedreht**
  (LED 0 rechts unten) – umgekehrt zur ursprünglichen Annahme. Im Code: `Pixelboard(false, true)` (Standard),
  Aufgabe 1: `OBEN_GEDREHT = false`, `UNTEN_GEDREHT = true`.
- **Defekt behoben (2026-10-20):** Am oberen Panel blieben die letzten ~40 LEDs dunkel – Ursache war ein
  Hardwaredefekt (vom Benutzer behoben), nicht die Software.
- **Strom:** 256 LEDs × bis zu 60 mA ≈ 15 A bei vollem Weiß! Im Code daher Helligkeit 20/255 und
  Strombegrenzung auf 500 mA (in `Pixelboard::begin()` bzw. `src/aufgabe1/main.cpp`)

## Kritische ESP32-Pins

- **WLAN aktiv:** Der ADC2-Block (GPIO 25, 26, 27, 14, 12, 13, 4, 0, 2, 15) kann **nicht für Analogmessungen** verwendet werden,
  da das WLAN-Modul ADC2 nutzt. → Joystick-Achsen immer an **ADC1-Pins** (32–39) anschließen.
- **GPIO 6–11:** nicht verwendbar (intern für den Flash-Speicher benötigt).
- **GPIO 34–39:** nur Eingänge, **keine internen Pull-up/-down-Widerstände** → nicht für Taster mit `INPUT_PULLUP` geeignet.
- **GPIO 0, 1, 3, 5, 14, 15:** werden beim Booten kurz auf HIGH gesetzt.
- **GPIO 1 und 3:** nicht verwenden (TX/RX für den Programm-Upload).
- **GPIO 0, 2, 4, 5, 12, 15 (Strapping-Pins):** Zieht eine Last diese Pins beim Booten auf HIGH oder LOW,
  kann das zu unerwartetem Verhalten führen.

> **ACHTUNG:** Bei manchen ESP32-Boards mit 36 Pins ist der Pin zwischen „5 V“ und „SD3“/„GPIO 11“
> fälschlich mit „GND“ beschriftet. Richtig wäre **„CMD“** – dort **nicht** GND anschließen!
> Folge (2026-10-13 bei Dominika): Upload bricht mit „Failed to communicate with the flash chip“ ab.
> Ein echter GND ist z. B. der Pin zwischen GPIO 19 und GPIO 21.

## Prüfung der aktuellen Belegung

Die aktuelle Pinbelegung verletzt keine der obigen Regeln:
- Die Joystick-Achsen liegen auf ADC1 (34, 35) → funktionieren auch mit WLAN.
- Der Taster (14) liegt auf einem ADC2-Pin, wird aber nur digital gelesen → kein Problem mit WLAN.
- Der Taster liegt nicht auf GPIO 34–39 → `INPUT_PULLUP` funktioniert.
- GPIO 14 wird beim Booten kurz HIGH gesetzt – als Eingang unkritisch.
