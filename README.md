# MandelbrotGL

Ein Standalone-Programm, das die Mandelbrot-Menge mit OpenGL 3.3 rendert.

Kompiliert und getestet mit Windows und Linux Ubuntu (lässt sich dort
per Doppelklick öffnen).

![Hauptbildschirm](https://raw.githubusercontent.com/MH321Productions/MandelbrotGL/main/Main.png)

## Bedienung

Das Programm lässt sich mit den Pfeiltasten und WASD steuern, um den
Bildausschnitt zu verschieben. Mit dem Scrollrad kann man (bis zu einem
maximalen Punkt) zoomen. Mit F11 wird der Volbilldmodus aktiviert/deaktiviert.

## UI

Das Fenster verfügt über ein Interface, mit welchem man Änderungen am Bild
vornehmen kann:

- Colormap: Mit den RadioButtons lassen sich die Farben des gerenderten Bildes
(anhand der von der GPU berechneten Grauwerte) festlegen
- Invertieren: Damit werden die von der GPU berechneten Grauwerte invertiert,
bevor sie mit der Colormap verrechnet werden

## Aktionen

Mit den Knöpfen auf der rechten Seite des Interfaces lassen sich folgende
Aktionen ausführen:

- Speichern: Öffnet einen Systemdialog, um das gerenderte Bild zu speichern
(die Größe entspricht der aktuellen Fenstergröße, das UI wird nicht mit
abgespeichert)
- Reset: Setzt die Konfigurationswerte und den Bildausschnitt auf die
Anfangswerte (wie im oberen Bild zu sehen) zurück, jedoch nicht die
Fenstergröße
- Beenden: Beendet das Programm (duh!)

## Credits

Die Bilder der Widgets des UI und der Font, mit dem die FPS gerendert werden,
stehen unter einer
[CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)-Lizenz
und sind bei [Kenney](https://www.kenney.nl/) zu finden.
