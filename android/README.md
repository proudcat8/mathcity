# MATHCITY — Android App

Native Kotlin + Jetpack Compose (Material3) port of the MATHCITY city model.
The city is a weighted graph: intersections are nodes, roads are edges,
each road has a cost. The app finds the cheapest route live as traffic
changes or roads get blocked.

## Screens

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

## Math inside (`app/src/main/java/com/mathcity/app/city/City.kt`)

- Graph: 12 nodes (R1–R3, N1–N3, W, H, F, S, P1–P2), 14 edges.
- Distance: `√((x₂−x₁)² + (y₂−y₁)²)` from fixed coordinates.
- Road cost: `dist × traffic`, advanced `a·D + b·T + c·C`.
- Routing: hand-rolled Dijkstra (binary heap), respects blocks + one-way.
- Known weak point: R1 has a single road (E0 R1–N1); blocking it isolates R1.

## Build

Needs JDK 17 (not 27+) and Android SDK platform-34 + build-tools 34.

```bash
export ANDROID_HOME=~/Android/Sdk ANDROID_SDK_ROOT=~/Android/Sdk JAVA_HOME=~/tools/jdk17 PATH=~/tools/jdk17/bin:$PATH
cd android
gradle assembleDebug
# out: app/build/outputs/apk/debug/app-debug.apk
```

First build downloads the Android Gradle Plugin + Compose deps (~500MB).

## Install

```bash
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

Or copy `android/MATHCITY.apk` (stable copy of the debug build) to the
phone and tap it. Enable USB debugging for `adb`.
