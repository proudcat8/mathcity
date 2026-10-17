# Flow - MATHCITY

Android app: `MATHCITY.apk`. Four tabs: Map, Waste, Water, Test.
Physical model runs standalone on Arduino; no live link required.

## Startup

```
App opens on Map tab -> draws city canvas with 12 nodes + 14 roads
-> shows current best route R1 -> H in green
-> user interacts via tabs and taps
```

## Tab -> Code Map

1. Map tab - `android/app/.../MainActivity.kt:MapScreen()`
   - Tap a road -> block/unblock it (red) OR cycle traffic Low/Med/High (amber)
   - Chips switch origin R1/R2/R3 or Fire emergency F->H
   - Dijkstra recalculates instantly; green line moves to new best route
   - Selected road panel shows `cost = distance x traffic`

2. Waste tab - `WasteScreen()`
   - Four collection points = 24 orders via permutation
   - Shows top 3 shortest routes starting/ending at water tank W

3. Water tab - `WaterScreen()`
   - Sliders for total water and H:S:R ratio weights
   - Minimums enforced: hospital >= 400 L, school >= 150 L
   - Shows litres and % per area with progress bars

4. Test tab - `TestScreen()`
   - Hospital placement: compares total residential distance for 3 candidate spots
   - Road failure test: knocks out 1 random road x 100, reruns Dijkstra R1->H,
     reports % connected and average detour

## Data Flow (per recalc on Map tab)

```
Tap road -> blocked/traffic state updates
-> dijkstra(start, end, traffic, blocked) returns path + total cost
-> Compose canvas repaints: green = best route, red = blocked, amber = congested
-> status banner shows route nodes and cost
```

## Judge Demo Script

```
Judge: "Block this road" -> tap road on Map tab -> it turns red -> new green route appears
Judge: "Heavy traffic here" -> tap road while in Traffic mode -> amber -> longer clear road wins
Judge: "Fire emergency!" -> tap Fire chip -> route switches to F -> H in red emergency colour
Judge: "What about waste?" -> switch to Waste tab -> shows best of 24 orders
Judge: "Water shortage?" -> switch to Water tab -> sliders split litres by ratio
Judge: "Is the city reliable?" -> switch to Test tab -> Run 100 trials -> % connected
```

## Files used per feature

- Map: `MainActivity.kt` (`MapScreen`), `City.kt` (`dijkstra`)
- Waste: `MainActivity.kt` (`WasteScreen`), `City.kt` (`wasteBest`)
- Water: `MainActivity.kt` (`WaterScreen`), `City.kt` (`waterAlloc`)
- Test: `MainActivity.kt` (`TestScreen`), `City.kt` (`resilience`)
- Theme: `Theme.kt`
