#pragma once
// Decoupled mode: no live serial required.
// Arduino runs standalone demo loop. App runs separately with same graph.
// If USB link added later, use these lines: R <path> / B <edge> / T <edge>=<1..3> / E 1
#define PROTO_BAUD 115200
#define LED_PIN 6
#define NUM_ROADS 14
