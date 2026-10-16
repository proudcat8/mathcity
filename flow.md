# Flow - MATHCITY

Binary: `./mathcity` (from `app/main.cpp`). Firmware: Arduino slave, only displays.

## Startup

```
main() -> graph_load() [core/graph.cpp] -> serial_open("/dev/ttyUSB0") [app/serial.cpp] -> UI loop [app/main.cpp]
```

No graph = no run. All coords/edges come from `core/graph.h`.

## Menu -> Code Map

Raylib window + terminal keys. Each option calls one module:

1. Option 1 - Show base map
   -> `graph_load()` -> draw nodes/edges
   -> files: `core/graph.cpp` + `app/main.cpp`

2. Option 2 - Normal route A->B (e.g. R1->H)
   -> `dijkstra(start, end)` with `traffic=1`, `blocked={}`
   -> files: `core/dijkstra.cpp` + `core/cost.cpp` (`cost = dist * traffic`)

3. Option 3 - Set traffic (1=Low, 2=Med, 3=High)
   -> `cost_update(edge, traffic)` -> `dijkstra()` again
   -> short+congested loses to long+free
   -> files: `core/cost.cpp` -> `core/dijkstra.cpp`

4. Option 4 - Block road (e.g. E2)
   -> `blocked_add(E2)` -> `dijkstra()` skipping E2
   -> send `B E2\n` via `serial_send()`
   -> files: `core/dijkstra.cpp` -> `app/serial.cpp` -> `firmware/src/main.cpp` (RED LED)

5. Option 5 - Emergency F->H
   -> `dijkstra(F, H)` + `serial_send("E 1")` -> buzzer + GREEN route flash
   -> if blocked, auto-recalc alternate
   -> files: `core/dijkstra.cpp` -> `app/serial.cpp`

6. Option 6 - Waste collection (P1-P4)
   -> `waste_best()` : 24 perms via `next_permutation`, sum dist matrix, return top 3
   -> files: `core/services.cpp`

7. Option 7 - Water distribution (H:S:R)
   -> `water_alloc(total, ratio 5:2:3, mins H>=400)` -> liters + % + deficit
   -> files: `core/services.cpp`

8. Option 8 - Resilience test (probability)
   -> `resilience(100)` : random 1-edge fail x100, run Dijkstra, report success% + avg extra cost
   -> files: `core/services.cpp` -> `core/dijkstra.cpp`

9. Option 0 - Quit
   -> `serial_close()` -> `CloseWindow()`

## Data Flow (per recalc)

```
UI input (block/traffic) -> cost.cpp builds weights -> dijkstra.cpp returns path+total
-> main.cpp draws path -> serial.cpp sends "R N1,N2,H" -> firmware parses protocol.h -> LEDs update
```

## Judge Demo Script

```
Judge: "Block N3-N5" -> Opt 4 -> new path computed -> model shows RED old, GREEN new
Judge: "Heavy traffic E1" -> Opt 3 (E1=3) -> Opt 2 rerun -> longer road wins
Judge: "Fire!" -> Opt 5 -> fastest F->H + buzzer
```
