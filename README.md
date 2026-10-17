# MATHCITY

MATHCITY is a small model city where mathematics does the planning instead of guesswork. Buildings sit at coordinates, roads form a weighted graph, and the app recalculates the best route whenever traffic changes or a road gets blocked.

## What the app does

Map tab
Draws the city with houses, hospital, fire station, school, water tank and waste points. Tap any road to block it or change its traffic level. The app uses Dijkstra's algorithm to show the new cheapest route instantly. A short road with heavy traffic can cost more than a longer clear road, so the shortest path is not always the best one.

Waste tab
Four waste collection points have 24 possible visit orders. The app checks every order and shows the three shortest collection routes.

Water tab
When water is limited, the app splits it between the hospital, school and residential areas using ratios and percentages, with minimum amounts enforced for each area.

Test tab
Shows where to place a hospital by comparing total distance from all residential areas. Also runs a test that knocks out one random road at a time one hundred times and reports how often the city stays connected.

## Install

Download MATHCITY.apk from this repository, copy it to your phone, and install it.

## Languages used

- Kotlin - app logic and interface (Jetpack Compose)
- XML - Android manifest and resources

## What is inside

- android/ - Kotlin app with a Compose interface
- firmware/ - Arduino code for the physical model LED demo (optional companion)

## Ideas demonstrated

Coordinate geometry, distance formula, weighted graphs, dynamic weights, Dijkstra shortest path, permutations, ratios, percentages, inequalities and probability.
