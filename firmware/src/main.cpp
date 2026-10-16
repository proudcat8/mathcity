#include <Arduino.h>
#include "protocol.h"

// Decoupled demo: cycles 3 preprogrammed scenarios on builtin LED.
// Upgrade: replace with FastLED WS2812B strip, one segment per road (14 roads, pin 6).
// Scenarios mirror app: 0=normal R1->H green, 1=congest yellow, 2=blocked alternate red.

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // scenario 0: normal, slow blink
    for (int i = 0; i < 3; i++) { digitalWrite(LED_BUILTIN, HIGH); delay(400); digitalWrite(LED_BUILTIN, LOW); delay(400); }
    delay(1000);
    // scenario 1: congested, fast blink
    for (int i = 0; i < 6; i++) { digitalWrite(LED_BUILTIN, HIGH); delay(150); digitalWrite(LED_BUILTIN, LOW); delay(150); }
    delay(1000);
}
