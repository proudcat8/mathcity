# Decisions - MATHCITY

Tech scope only: code + electronics + app. No cardboard/poster.
Workdir: `/home/proudcat/Downloads/mathcity/`
OS: Arch Linux. Toolchain: `base-devel cmake cargo? no - g++, platformio`.

## D1: Python -> C++17 (core + app)

Why: no runtime, single binary, deterministic Dijkstra, direct `/dev/ttyUSB0` access, shares structs with Arduino firmware. Python + Streamlit needs browser/server/pip, fragile for live judge demo.

Before (Python - rejected):
```python
import heapq
def dijkstra(graph, start):
    dist = {start: 0}
    pq = [(0, start)]
```

After (C++17 - chosen):
```cpp
// core/dijkstra.h
#pragma once
#include <vector>
std::vector<int> dijkstra(int start, int end);
```

## D2: Streamlit/Web UI -> Raylib Desktop UI (C++)

Why: offline, no browser, 60fps 2D map, click edge to block. One `./mathcity` binary.

Before (rejected):
```python
import streamlit as st
st.slider("traffic", 1, 3)
```

After (chosen):
```cpp
// app/main.cpp
// TODO: InitWindow(800, 600, "MATHCITY");
// TODO: if (IsMouseButtonPressed(0)) ToggleBlock(edge);
```

## D3: pyserial -> raw termios serial (C++)

Why: zero dep, works on Arch with `g++` only, same language as rest.

Before (rejected):
```python
import serial
ser = serial.Serial('/dev/ttyUSB0', 115200)
ser.write(b'ROUTE:N1,N2\n')
```

After (chosen):
```cpp
// app/serial.h
#pragma once
int serial_open(const char* dev); // TODO: termios 115200
void serial_send(int fd, const char* line);
```

Protocol (locked): `R N1,N2,H\n`, `T E2=3\n`, `B E2\n`, `E 1\n`

## D4: Arduino IDE -> PlatformIO + FastLED + WS2812B

Why: PlatformIO = CLI, reproducible `platformio.ini`, versioned. WS2812B strip = 1 data pin (D6) for 12-14 roads vs 14 pins for discrete LEDs.

Before (rejected):
```cpp
digitalWrite(13, HIGH); // one pin per road, runs out
```

After (chosen):
```cpp
// firmware/src/main.cpp
// TODO: FastLED.show(); // GREEN=route, YELLOW=congest, RED=blocked
```

## D5: networkx -> hand-rolled Dijkstra + adjacency list

Why: project must SHOW math (graphs, distance x traffic, `a*D+b*T+c*C`). Library hides it.

Before (rejected):
```python
import networkx as nx
nx.shortest_path(G, "F", "H")
```

After (chosen):
```cpp
// core/dijkstra.cpp
// TODO: adjacency list + binary-heap, skip blocked, respect one_way
```

## D6: Layout of repo

```
core/graph.h/.cpp       // NODES(x,y), EDGES(u,v,dist,time,one_way)
core/cost.h/.cpp        // dist*traffic, a*D+b*T+c*C
core/dijkstra.h/.cpp    // routing
core/services.h/.cpp    // waste perm, water ratio, resilience
app/main.cpp            // Raylib UI + menu
app/serial.h/.cpp       // termios link
firmware/src/main.cpp   // LED display slave
firmware/include/protocol.h // shared protocol strings
firmware/platformio.ini // env: nanoatmega328
CMakeLists.txt          // root build
```
