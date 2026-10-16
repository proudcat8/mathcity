# MATHCITY — The City That Thinks Using Mathematics

A model city designed with mathematics: buildings placed by coordinate
geometry, roads as a weighted graph, and an app that reroutes the city
when traffic changes or roads get blocked.

## Download the app

Get `MATHCITY.apk` from the file list above (or **Releases** on the right
once published) → copy to your phone → tap to install.

## Android app (native Kotlin + Jetpack Compose)

Polished Material3 app, 4 tabs:

- **Map** — Canvas city map. Tap a road to block/unblock (red) or cycle
  traffic Low 1 / Med 2 / High 3 (amber). Green = current best route.
  White dot = one-way road N1→N2. Chips switch home (R1/R2/R3) or Fire
  emergency (F→H). Selected road shows `cost = dist × traffic`.
- **Waste** — 4 collection points = 24 orders (4!). Brute-forces all
  permutations from depot W, shows 3 shortest.
- **Water** — shortage allocator. Sliders for total litres and H:S:R
  weights, minimums H≥400 / S≥150 enforced, % bars per group.
- **Test** — hospital placement by total residential distance
  (coordinate geometry) + 100-trial random road-failure resilience test.

Source: `android/` — logic in `app/src/main/java/com/mathcity/app/city/City.kt`.

### Build the APK

Needs JDK 17 (not 27+) and Android SDK platform-34 + build-tools 34.

```bash
export ANDROID_HOME=~/Android/Sdk ANDROID_SDK_ROOT=~/Android/Sdk JAVA_HOME=~/tools/jdk17 PATH=~/tools/jdk17/bin:$PATH
cd android
~/tools/gradle-8.10.2/bin/gradle assembleDebug
# out: app/build/outputs/apk/debug/app-debug.apk
```

First build downloads the Android Gradle Plugin + Compose deps (~500MB).

### Install

```bash
adb install -r MATHCITY.apk
```

Enable USB debugging on the phone first.

## Math inside (`android/.../city/City.kt`, mirrored in `core/`)

- Graph: 12 nodes (R1–R3, N1–N3, W, H, F, S, P1–P2), 14 edges.
- Distance: `√((x₂−x₁)² + (y₂−y₁)²)` from fixed coordinates.
- Road cost: `dist × traffic`, advanced `a·D + b·T + c·C`.
- Routing: hand-rolled Dijkstra (binary heap), respects blocks + one-way.
- Known weak point: R1 has a single road (E0 R1–N1); blocking it isolates R1.

## Desktop version (C++17 + Raylib)

Same city, desktop window. For classroom demo on laptop:

```bash
cmake -S . -B build && cmake --build build
./build/mathcity
```

Controls: click road = block, T+hover = traffic, E = emergency F→H,
N = normal, R = reset, W = waste order.

## Physical model (Arduino, decoupled)

`firmware/` runs standalone on a Nano/Uno — preprogrammed LED demo loop,
no USB link needed. App and model share the same 14-road graph but run
separately. Upgrade path: WS2812B strip, one segment per road on pin D6
(see `firmware/include/protocol.h`).

## Repo layout

```
MATHCITY.apk            # installable app (see Releases for downloads)
android/                # Kotlin + Compose app source
core/                   # C++ city math (graph, cost, Dijkstra, services)
app/                    # C++ Raylib desktop UI
firmware/               # Arduino standalone LED demo
decisions.md            # tech decisions + before/after snippets
flow.md                 # menu → code map + judge demo script
```
