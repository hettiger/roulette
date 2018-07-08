# Roulette

Willkommen zur Roulette Strategie Simulation.

## Grundsätzlicher Ablauf

- Es wird immer auf Rot oder Schwarz gesetzt.
- Du startest mit einem von dir festgelegten Grundeinsatz.
- Verlierst du, verdoppelst du deinen Einsatz.
- Gewinnst du, beginnst du wieder mit deinem Grundeinsatz.

Du hast in dieser Simulation außerdem die Möglichkeit, eine Maximalanzahl für Fehlversuche festzulegen.
Das heißt, wenn du z. B. nach 5 Verdopplungen des Einsatzes noch nicht gewonnen hast,
fängst du wieder mit deinem Grundeinsatz an. So riskierst du in einer Runde nicht zuviel...

> __Achtung:__ 
> Roulette ist ein Glücksspiel und es gibt keine sinnvolle Strategie um langfristig gegen die Bank zu gewinnen.

### Installation

```bash
cd ~/Roulette
cmake .
make
./Roulette -h
```

### Folgende Argumente können an das Programm übergeben werden:

```txt
-d
Debugging Information ausgeben.

-v
Das Programm gibt mehr Informationen aus.

-vv
Das Programm gibt sämtliche Informationen aus.

-t=8
Anzahl der Threads. (Default = 8)

-n=1
Anzahl der Durchläufe. (Default = 1)

-r=200.00
Startbankroll (Default = 200.00)

-b=10.00
Grundeinsatz (Default = 10.00)

-f=0
Faktor für dynamischen Einsatz (Einsatz = Bankroll × Faktor, Ohne Dynamik = 0, Default = 0)

-s=2
Min. Einsatz (Default = 2.00)

-e=12000
Max. Einsatz (Default = 12000.00)

-l=5
Maximal erlaubte Fehlversuche (Default = 5, Ohne Limit = 0)
```
