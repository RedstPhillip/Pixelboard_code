# Coding-Guidelines

Zurück zur [Übersicht](index.md).

## Namenskonventionen

- **Klassen:** `PascalCase` – z. B. `Taster`, `Joystick`
- **Objekte / Variablen:** `camelCase` – z. B. `taster`, `joystickLinks`, `letzterZustand`
- **Methoden:** `camelCase` – z. B. `check()`, `tasterGedrueckt()`
- **Konstanten / Makros:** `UPPER_SNAKE_CASE` – z. B. `LED_PIN`
- Bezeichner auf **Deutsch**, passend zum bestehenden Code.

## Kommentare

Jede Methode erhält **über dem Methodenkopf einen Kommentar**, der ihre Funktionsweise beschreibt
(Zweck, Parameter, Rückgabewert):

```cpp
// Liest den Taster ein, entprellt das Signal und erkennt kurze bzw. lange Tastendrücke.
// Muss in jedem loop()-Durchlauf aufgerufen werden.
void check() { ... }
```

## Allgemein

- Nicht blockierend programmieren: `millis()` statt `delay()`.
- Größere Änderungen in kleine, einzeln testbare Schritte zerlegen
  (siehe [communication-guidelines.md](communication-guidelines.md)).
