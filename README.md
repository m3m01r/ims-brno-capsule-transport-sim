# Brno Capsule Transport Simulation

Discrete-event simulation of an on-demand capsule transport system for Brno,
inspired by Hyperloop and the transport tubes from Futurama. Written in C++ with
[SIMLIB](https://www.fit.vut.cz/person/peringer/public/SIMLIB/).

The model replaces a one-directional segment of Brno tram lines 1 and 6
(Řečkovice / Královo Pole → Moravské náměstí) with single-occupancy capsules.
The goal is to check whether such a system can move people faster than trams
while keeping waiting times acceptable.

## How it works

Every stop has a single lift, a pool of capsules and a number of landing places.
Passengers arrive with an exponential distribution and pick a destination stop.
A passenger waits for the lift, enters a capsule, and departs once a landing
place at the destination is free. After drop-off, the capsule returns to its
original stop.

Travel time is computed from the distance and acceleration using triangular or
trapezoidal velocity profiles, with an optional jerk term for smoother
acceleration changes. Real distances and passenger intensities from Brno are
used as model inputs.

## Requirements

- `g++` with C++20
- SIMLIB (`-lsimlib`)
- `make`
- Python 3 with `matplotlib` and `numpy` (optional, for plots)

## Build and run

```sh
make
make run
```

`make run` starts the simulation with the configuration from the report
(`-a 7 -c 11 -l 15`).

### Command-line options

| Option | Description | Default |
| --- | --- | --- |
| `-a <m/s^2>` | Capsule acceleration | `2` |
| `-c <n>` | Number of capsules per stop | `2` |
| `-l <n>` | Number of landing places per stop | `5` |
| `-f` | Write statistics to `.out` files instead of stdout | `false` |

Example:

```sh
./capsules -a 7 -c 11 -l 15 -f
```

## Output

With `-f`, the program writes per-stop statistics to `.out` files:
passenger counts, waiting time before entering a capsule, waiting time before
departure, and journey times between stops. Without it, the same data is
printed to stdout.

The included `plot.py` script turns the exported data into charts:

```sh
python plot.py -c   # waiting time vs. number of capsules
python plot.py -l   # waiting time vs. number of landing places
python plot.py -a   # travel time vs. acceleration
python plot.py -j   # journey time from a selected stop
```

## Project structure

| File | Purpose |
| --- | --- |
| `main.cpp` | Entry point, argument parsing, statistics output |
| `tram_stop.*` | `TramStop` class and Brno stop data |
| `passenger.*` | Passenger process and passenger generator |
| `capsule.*` | Capsule return process |
| `plot.py` | Plotting of simulation results |
| `Makefile` | Build and run targets |

## Authors

- Danil Tasmassys (`xtasmad00`)
- Albert Tikaiev (`xtikaia00`)

Faculty of Information Technology, Brno University of Technology.
