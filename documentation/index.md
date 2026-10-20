# Projektdokumentation – Übersicht

Zentrale Wissensbasis des Projekts (Kontext-Engineering). Die Dateien sind untereinander verlinkt
(Prinzip „WikiLLM“), damit ein neuer Agent/Chat ohne Vorwissen sofort den Projektstand kennt.

| Datei | Inhalt |
|---|---|
| [anforderungen.md](anforderungen.md) | Projektanforderungen (Mini-Projekt Joystick) und offene Fragen |
| [hardware.md](hardware.md) | Board (ESP32), Pinbelegung, kritische Pins |
| [platformio.md](platformio.md) | VS Code + PlatformIO: Aufbau, Bedienung, Probleme |
| [coding-guidelines.md](coding-guidelines.md) | Namenskonventionen, Kommentarregeln |
| [communication-guidelines.md](communication-guidelines.md) | Wie die KI kommuniziert und arbeitet |
| [todo.md](todo.md) | TO-DO-Liste / Projektfortschritt |
| [uml.md](uml.md) | UML-Klassendiagramme der erstellten Klassen |
| [test-log.md](test-log.md) | Testanweisungen und deren Erledigung |

## Projektstruktur

```
Taster/
├── AGENTS.md            Einstiegspunkt für KI-Agenten
├── platformio.ini       PlatformIO-Konfiguration (ESP32, siehe platformio.md)
├── src/
│   ├── Joystick.h       Klasse Joystick – Deklaration (Attribute, Methodenköpfe)
│   ├── Joystick.cpp     Klasse Joystick – Implementierung der Methoden
│   ├── LedPanel.h       Klasse LedPanel – Deklaration (ein Panel 32x8, x/y-Koordinaten, optional 180° gedreht)
│   ├── LedPanel.cpp     Klasse LedPanel – Implementierung (Umrechnung x/y → LED-Index)
│   ├── Pixelboard.h     Klasse Pixelboard – Deklaration (2 Panels als Fläche 32x16)
│   ├── Pixelboard.cpp   Klasse Pixelboard – Implementierung
│   ├── Laufschrift.h    Klasse Laufschrift – Deklaration (Text über 1 oder 2 Panels, LEDText)
│   ├── Laufschrift.cpp  Klasse Laufschrift – Implementierung
│   ├── PanelStreifen.h  Klasse PanelStreifen – Deklaration (2 Panels als ein 64×8-Streifen, 180°-Drehung)
│   ├── PanelStreifen.cpp Klasse PanelStreifen – Implementierung (Umrechnung x/y → LED in Panel 0/1)
│   ├── Snake.h          Klasse Snake – Deklaration (Snake-Spiel auf dem Pixelboard)
│   ├── Snake.cpp        Klasse Snake – Implementierung
│   ├── aufgabe1/
│   │   └── main.cpp     Aufgabe 1: Laufschrift mit Sekundenzähler (1 oder 2 Panels, oben 25 / unten 26) → env:aufgabe1
│   ├── aufgabe2/
│   │   └── main.cpp     Aufgabe 2: Testprogramm x/y-Koordinaten (Pixelboard, 2 Panels) → env:aufgabe2
│   └── aufgabe3/
│       └── main.cpp     Aufgabe 3: Snake-Spiel mit Joystick (Pixelboard 32x16) → env:aufgabe3
└── documentation/       Diese Dokumentation
```

## Aktueller Stand

Siehe [todo.md](todo.md). Kurzfassung: Joystick-Mini-Projekt abgeschlossen. **Projekt Pixelboard** (2 Panels, 32×16):
Aufgabe 1 (Laufschrift über beide Panels), Aufgabe 2 (x/y-Koordinaten, Klasse `Pixelboard`) und Aufgabe 3 (Snake mit
Joystick) laufen. Als Nächstes: Uhrzeit, Wetter, DHT22 + Google-Tabelle.
