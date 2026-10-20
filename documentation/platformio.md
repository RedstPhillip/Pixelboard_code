# Entwicklungsumgebung: VS Code + PlatformIO

Zurück zur [Übersicht](index.md).

## Projektaufbau

```
Taster/                  ← diesen Ordner in VS Code öffnen (Datei → Ordner öffnen)
├── platformio.ini       Projektkonfiguration (Board, Framework, Baudrate)
├── src/aufgabe1/main.cpp  Hauptprogramm Aufgabe 1 (Laufschrift) – nur in env:aufgabe1
├── src/aufgabe2/main.cpp  Hauptprogramm Aufgabe 2 (x/y-Koordinaten) – nur in env:aufgabe2
├── src/aufgabe3/main.cpp  Hauptprogramm Aufgabe 3 (Snake) – nur in env:aufgabe3
├── src/*.h / src/*.cpp    Klassen (Joystick, LedPanel, Pixelboard, Laufschrift, PanelStreifen, Snake), werden von den Umgebungen mitkompiliert
└── .pio/                Build-Ausgabe, wird automatisch erzeugt – nicht bearbeiten
```

`platformio.ini`:

| Einstellung | Wert | Bedeutung |
|---|---|---|
| `platform` | `espressif32` | ESP32-Plattform |
| `board` | `esp32dev` | generisches ESP32-DevKit (passt für die meisten 30/36-Pin-Boards) |
| `framework` | `arduino` | Arduino-Befehle (`digitalRead`, `millis` …) verfügbar |
| `monitor_speed` | `115200` | Baudrate des Serial Monitors, muss zu `Serial.begin()` passen |
| `upload_port` / `monitor_port` | auskommentiert | Port wird automatisch erkannt (CP210x/CH340). Nicht fest eintragen: der Port ist auf jedem PC anders (Philipp COM6, Dominika COM3 – dort ist COM6 Bluetooth) |
| `[env]` | gemeinsamer Teil | Einstellungen und Bibliotheken für alle Aufgaben |
| `[env:aufgabe1]` / `[env:aufgabe2]` / `[env:aufgabe3]` | `build_src_filter` | Jede Umgebung kompiliert alle Klassen in `src/`, aber nur **ihr** Hauptprogramm (andere Aufgaben-Ordner werden ausgeschlossen – sonst gäbe es zwei `setup()`/`loop()`) |
| `default_envs` | `aufgabe1` | Umgebung, die ohne Auswahl gebaut/hochgeladen wird |

**Aufgabe auswählen:** In VS Code unten in der blauen Statusleiste auf **`env:…`** klicken und `env:aufgabe1` oder
`env:aufgabe2` wählen – ✓ und → beziehen sich dann nur auf diese Aufgabe. Im Terminal: `pio run -e aufgabe2 -t upload`.
Achtung: Steht dort „Default“ und `default_envs` fehlt, würden **alle** Umgebungen nacheinander hochgeladen (die letzte gewinnt).

Bibliotheken werden über `lib_deps` in `platformio.ini` eingetragen. PlatformIO lädt sie beim nächsten
Build automatisch nach `.pio/libdeps/` herunter (nicht von Hand bearbeiten). Aktuell eingetragen:

| Eintrag | Bedeutung |
|---|---|
| `fastled/FastLED@3.7.8` | FastLED aus der PlatformIO-Registry, **feste Version** (3.10.x kompiliert nicht mit dem ESP32-Compiler von PlatformIO) |
| `https://github.com/AaronLiddiment/LEDMatrix` | direkt von GitHub (nicht in der Registry) |
| `https://github.com/AaronLiddiment/LEDText` | direkt von GitHub (nicht in der Registry) |

## Schritt für Schritt: Programm auf den ESP32 bringen

1. VS Code → *Datei → Ordner öffnen* → Ordner `Taster` (der mit `platformio.ini`).
2. ESP32 per USB anschließen (Port wird automatisch erkannt) und unten die Aufgabe (`env:aufgabe1`/`env:aufgabe2`/`env:aufgabe3`) wählen.
3. Unten links auf **→ (Upload)** klicken.
4. Sobald im Terminal `Connecting.....` erscheint: **BOOT-Taste am ESP32 gedrückt halten**, bis
   `Writing at ...` erscheint, dann loslassen. (Ohne das kommt „Wrong boot mode detected“.)
5. Nach `SUCCESS` einmal die **EN-/RST-Taste** drücken (Neustart).
6. Unten links auf **🔌 (Serial Monitor)** klicken → die Joystick-Werte laufen durch.
   Vor dem nächsten Upload den Monitor schließen (Papierkorb-Symbol im Terminal).

## Bedienung in VS Code

Die Knöpfe sind in der **blauen Statusleiste unten links**:

| Symbol | Funktion |
|---|---|
| ✓ (Haken) | **Build** – Kompilieren |
| → (Pfeil) | **Upload** – Kompilieren und auf den ESP32 laden |
| 🔌 (Stecker) | **Serial Monitor** – Ausgaben von `Serial.print` anzeigen |
| 🗑 (Mülleimer) | **Clean** – Build-Ordner löschen |

Alternativ im Terminal:

```bash
pio run                 # Kompilieren
pio run -t upload       # Hochladen
pio device monitor      # Serial Monitor (Beenden: Strg+C)
```

## Typische Probleme

- **Upload schlägt fehl („Failed to connect … Wrong boot mode“):** Während „Connecting….“ angezeigt wird,
  die **BOOT-Taste** am ESP32 gedrückt halten, bis der Upload startet.
- **Kein COM-Port gefunden:** USB-Treiber fehlt (meist CP210x oder CH340, je nach Chip am Board),
  oder das USB-Kabel ist nur ein Ladekabel ohne Datenleitungen.
- **„Packet content transfer stopped“:** Verbindung bricht während des Uploads ab. Mögliche Ursachen: Serial Monitor
  offen, schlechtes USB-Kabel, LED-Panel zieht während des Uploads zu viel Strom. Notfalls `upload_speed = 115200`
  in `platformio.ini` (wurde am 2026-10-13 vom Benutzer anders gelöst, daher nicht eingetragen).
- **„Failed to communicate with the flash chip“, danach „Packet content transfer stopped“:** Der ESP32 erreicht seinen
  Flash nicht – Verdrahtungsproblem. Bei Dominika (2026-10-13) hing GND am falsch beschrifteten Pin zwischen 5 V und SD3
  (in Wahrheit **CMD**, siehe [hardware.md](hardware.md)); mit GND am Pin zwischen GPIO 19 und 21 behoben.
- **Upload geht nicht, obwohl der Port stimmt:** Serial Monitor vorher schließen (blockiert den Port).
- **Unleserliche Zeichen im Monitor:** `monitor_speed` passt nicht zu `Serial.begin()`.
- **Google Drive:** Der Ordner `.pio/` enthält viele Build-Dateien, die Google Drive unnötig synchronisiert.
