#pragma once
// Decoupled mode: physical model runs standalone on Arduino.
// This is a stub so the app builds without hardware. Live serial added later as stretch.
inline int serial_open(const char* /*dev*/) { return -1; }
inline void serial_send(int /*fd*/, const char* /*line*/) {}
inline void serial_close(int /*fd*/) {}
