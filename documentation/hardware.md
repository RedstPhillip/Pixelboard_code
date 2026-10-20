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
| 16 | Data-Pin LED-Panel (**aktuell zum Testen**, nur ein Panel angeschlossen; nur noch Aufgabe 2 – Aufgabe 1 nutzt 25/26) | Ausgang; bei ESP32-WROVER-Modulen für PSRAM belegt, beim WROOM (esp32dev) frei |
| 25 | Data-Pin **oberes** LED-Panel (verkabelt, Aufgabe 1) | Ausgang (ADC2-Pin, als digitaler Ausgang unproblematisch) |
| 26 | Data-Pin **unteres** LED-Panel (verkabelt, Aufgabe 1) | Ausgang (ADC2-Pin, als digitaler Ausgang unproblematisch) |
| 13 (?) | DHT22 Data | **noch auszuprobieren** |

Ursprünglich geplant, aber **nicht angeschlossen** (nur ein Joystick vorhanden): Ext. Joystick X 32, Y 33, SW 27 – diese Pins sind frei.

## LED-Panels

- 2× 8×32 LEDs, Typ **WS2812** (Farbreihenfolge GRB), Anordnung **Vertical Zig-Zag**
- Jedes Panel an einem **eigenen** Data-Pin (nicht hintereinander gehängt)
- **Verdrahtung (Panel an GPIO 16, von vorne):** LED 0 sitzt **links oben**; Spalte 0 läuft nach unten (0..7),
  Spalte 1 nach oben (8..15), usw. (durch Test T7 ermittelt – die erste Annahme „links unten“ war falsch)
- Oberes Panel ist gegenüber dem unteren **um 180° gedreht** (Anschlüsse zeigen zueinander).
  Annahme im Code (Aufgabe 1): **unteres** Panel wie das getestete (LED 0 links oben, Anschluss oben),
  **oberes** gedreht (LED 0 rechts unten). Bestätigung durch Test T9b offen
- **Strom:** 256 LEDs × bis zu 60 mA ≈ 15 A bei vollem Weiß! Im Testprogramm daher Helligkeit 20/255 und
  Strombegrenzung per `FastLED.setMaxPowerInVoltsAndMilliamps(5, 500)`

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
