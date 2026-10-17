# Decisions - MATHCITY

Tech scope only: code + electronics + app. No cardboard/poster.
Workdir: `/home/proudcat/Downloads/mathcity/`
OS: Arch Linux. Toolchain: JDK 17, Gradle, Android SDK platform-34, Compose.

## D1: C++ desktop -> Kotlin + Jetpack Compose Android app

Why: the judge uses a phone. Kotlin + Compose builds a native APK, works offline, has real buttons/sliders, no browser or server. C++ Raylib desktop was kept only as a fallback demo on a laptop; it is not the app language.

Before (rejected - desktop):
```cpp
// app/main.cpp
InitWindow(800, 600, "MATHCITY");
```

After (chosen - Android):
```kotlin
// android/app/.../MainActivity.kt
setContent { MathcityTheme { AppScreen() } }
```

## D2: App and physical model run separately

Why: no fragile USB serial link on demo day. The Android app computes and shows routes on screen. The Arduino firmware runs standalone preprogrammed LED patterns on the model. Same graph logic in both; no runtime connection required.

Before (rejected - linked):
```cpp
serial_send("B E2\n"); // app talks to Arduino live
```

After (chosen - decoupled):
```kotlin
blocked = blocked + edgeId // app only, drawn on Compose canvas
```

## D3: Hand-rolled Dijkstra instead of graph library

Why: the project must show the math (graphs, distance x traffic, blocked edges). A library hides the algorithm.

Before (rejected):
```kotlin
// library hides everything
graph.shortestPath(start, end)
```

After (chosen):
```kotlin
// android/app/.../city/City.kt
fun dijkstra(start: Int, end: Int, ...): RouteResult { ... }
```

## D4: Arduino IDE -> PlatformIO + FastLED + WS2812B

Why: PlatformIO = CLI, reproducible `platformio.ini`, versioned. WS2812B strip = 1 data pin (D6) for 12-14 roads vs 14 pins for discrete LEDs.

Before (rejected):
```cpp
digitalWrite(13, HIGH); // one pin per road, runs out
```

After (chosen):
```cpp
// firmware/src/main.cpp
FastLED.show(); // GREEN=route, YELLOW=congested, RED=blocked
```

## D5: Layout of repo

```
MATHCITY.apk            # installable app
android/                # Kotlin + Compose app source
firmware/               # Arduino standalone LED demo (optional)
decisions.md            # tech decisions + before/after snippets
flow.md                 # app flow + judge demo script
README.md               # features + install + languages
```
