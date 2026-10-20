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
│   ├── Joystick.h       Klasse Joystick – Deklaration (Attribute, Methodenköpfe), aktuell ungenutzt
│   ├── Joystick.cpp     Klasse Joystick – Implementierung der Methoden
│   ├── LedPanel.h       Klasse LedPanel – Deklaration (ein Panel 32x8, x/y-Koordinaten)
│   ├── LedPanel.cpp     Klasse LedPanel – Implementierung (Umrechnung x/y → LED-Index)
│   ├── Laufschrift.h    Klasse Laufschrift – Deklaration (Text über 1 oder 2 Panels, LEDText)
│   ├── Laufschrift.cpp  Klasse Laufschrift – Implementierung
│   ├── PanelStreifen.h  Klasse PanelStreifen – Deklaration (2 Panels als ein 64×8-Streifen, 180°-Drehung)
│   ├── PanelStreifen.cpp Klasse PanelStreifen – Implementierung (Umrechnung x/y → LED in Panel 0/1)
│   ├── aufgabe1/
│   │   └── main.cpp     Aufgabe 1: Laufschrift mit Sekundenzähler (1 oder 2 Panels, oben 25 / unten 26) → env:aufgabe1
│   └── aufgabe2/
│       └── main.cpp     Aufgabe 2: Testprogramm x/y-Koordinaten (1 Panel an GPIO 16) → env:aufgabe2
└── documentation/       Diese Dokumentation
```

## Aktueller Stand

Siehe [todo.md](todo.md). Kurzfassung: Joystick-Mini-Projekt abgeschlossen. Neues **Projekt Pixelboard**:
Libraries installiert und getestet. Beide Aufgaben für **ein** Panel umgesetzt, jede mit eigenem Hauptprogramm:
Aufgabe 1 (Laufschrift, Klasse `Laufschrift`; Version für **2 Panels** umgesetzt – oben, dann unten weiter; Tests T9a/T9b offen) und Aufgabe 2 (x/y-Methode, Klasse `LedPanel`, Test T7a offen).
Danach: beide Aufgaben für 2 Panels (oberes 180° gedreht).
