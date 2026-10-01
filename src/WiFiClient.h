#pragma once
// On ESP32, WiFiClient.h is its own header (WiFiClient is a TCP client backed
// by the WiFi stack). The simulator already aliases WiFiClient to
// NetworkClient in WiFi.h; this file just gives it the standalone include
// path that upstream's SecureHttpClient.h expects.
#include "WiFi.h"
