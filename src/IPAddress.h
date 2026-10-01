#pragma once
// On ESP32 this is its own header; split out of WiFi.h so Client.h (which
// NetworkClient.h needs, which WiFi.h needs for the WiFiClient alias) can use
// IPAddress without creating a WiFi.h <-> NetworkClient.h <-> Client.h cycle.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "WString.h"

class IPAddress {
  uint8_t bytes[4] = {0, 0, 0, 0};

public:
  IPAddress() {}
  IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    bytes[0] = a;
    bytes[1] = b;
    bytes[2] = c;
    bytes[3] = d;
  }
  String toString() const {
    char buf[16];
    snprintf(buf, sizeof(buf), "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2],
             bytes[3]);
    return String(buf);
  }
  uint8_t operator[](int i) const { return bytes[i % 4]; }
  uint8_t &operator[](int i) { return bytes[i % 4]; }
  bool operator==(const IPAddress &o) const {
    return memcmp(bytes, o.bytes, sizeof(bytes)) == 0;
  }
  bool operator!=(const IPAddress &o) const { return !(*this == o); }
};
