# AGENTS.md – Startdatei für KI-Agenten

**Diese Datei wird von der KI als Erstes gelesen**, um einen Überblick über das Projekt zu erhalten.
Danach liest die KI die für die aktuelle Aufgabe relevanten Dateien im Ordner `documentation/`.

## Projekt in einem Satz

ESP32-Projekt (C++, Arduino-Framework): Die Klasse `Taster` aus dem Vorjahr wurde zur Klasse `Joystick`
umgebaut (Taster + 2 Potentiometer).
**Aktuelles Projekt: Pixelboard** (2 LED-Panels 8x32 + Joystick, siehe [anforderungen.md](documentation/anforderungen.md)).
Entwicklung mit **VS Code + PlatformIO** (Kompilieren: `pio run`). Klassen je als `.h` + `.cpp` in `src/`
(`Joystick`, `LedPanel`), Testprogramm in `src/main.cpp`.

## Was in den anderen Markdown-Dateien gespeichert wird

| Datei | Inhalt |
|---|---|
| [documentation/index.md](documentation/index.md) | Übersicht über die Dokumentation, Projektstruktur, aktueller Stand in Kurzform |
| [documentation/anforderungen.md](documentation/anforderungen.md) | Projektanforderungen, Ausgangslage (bestehender Code), offene Fragen und getroffene Entscheidungen |
| [documentation/hardware.md](documentation/hardware.md) | Hardware: Board (ESP32), Pinbelegung, kritische Pins und Einschränkungen |
| [documentation/platformio.md](documentation/platformio.md) | Entwicklungsumgebung VS Code + PlatformIO: Projektaufbau, `platformio.ini`, Bedienung, typische Probleme |
| [documentation/coding-guidelines.md](documentation/coding-guidelines.md) | Regeln für den Code: Namenskonventionen (`PascalCase` für Klassen, `camelCase` für Objekte), Kommentar über jedem Methodenkopf |
| [documentation/communication-guidelines.md](documentation/communication-guidelines.md) | Wie die KI mit dem Benutzer kommuniziert: sachlich, mit Alternativen samt Vor-/Nachteilen, nachfragen statt Annahmen treffen, kleine Schritte |
| [documentation/todo.md](documentation/todo.md) | TO-DO-Liste: erledigte und offene Aufgaben, dokumentiert den Projektfortschritt |
| [documentation/uml.md](documentation/uml.md) | UML-Klassendiagramme aller erstellten Klassen (Attribute, Methoden, Beziehungen) |
| [documentation/test-log.md](documentation/test-log.md) | Log-Datei mit Testanweisungen zu jeder Änderung und deren Erledigung/Ergebnis |

## Pflicht: Dokumentation selbständig aktuell halten

Die KI hält **alle Markdown-Dateien von sich aus aktuell, ohne dass der Benutzer dazu auffordert.**
Nach jeder Änderung bzw. jedem abgeschlossenen Arbeitsschritt prüft die KI, welche Dateien betroffen sind,
und aktualisiert sie im selben Schritt:

- **Code geändert** → [uml.md](documentation/uml.md) anpassen (neue/geänderte Klassen, Attribute, Methoden).
- **Aufgabe erledigt oder neue Aufgabe erkannt** → [todo.md](documentation/todo.md) aktualisieren (mit Datum).
- **Neuer Programmschritt** → Testanweisung in [test-log.md](documentation/test-log.md) eintragen;
  Testergebnisse des Benutzers dort nachtragen.
- **Frage geklärt oder Entscheidung getroffen** → in [anforderungen.md](documentation/anforderungen.md) festhalten.
- **Hardware, Verkabelung oder Pins geändert** → [hardware.md](documentation/hardware.md) aktualisieren
  und die neue Belegung gegen die Liste der kritischen ESP32-Pins prüfen.
- **`platformio.ini` oder Projektstruktur geändert** → [platformio.md](documentation/platformio.md) und
  [index.md](documentation/index.md) aktualisieren.
- **Neue Regel vom Benutzer** → in die passenden Guidelines übernehmen.
- **Neue Datei angelegt** → in dieser Tabelle und in [index.md](documentation/index.md) eintragen.

Ziel: Ein neuer Agent oder Chat kennt allein durch diese Dateien den vollständigen Projektstand.

## Weitere Arbeitsregeln (Kurzfassung)

- [Coding-Guidelines](documentation/coding-guidelines.md) und
  [Communication-Guidelines](documentation/communication-guidelines.md) einhalten.
- Bei fehlenden Informationen **nachfragen statt Annahmen zu treffen**.
- Größere Änderungen in kleine, einzeln testbare Schritte zerlegen.
- Sprache: Deutsch.
