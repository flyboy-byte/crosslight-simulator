#pragma once
// Arduino's abstract network client base (what WiFiClient, WiFiClientSecure,
// and freeink's SecureClient all derive from). Not previously needed here;
// added when the plugin system (upstream) started deriving a TLS client from
// it directly instead of going through NetworkClient.
#include "IPAddress.h"
#include "Stream.h"

class Client : public Stream {
public:
  virtual ~Client() = default;
  virtual int connect(IPAddress ip, uint16_t port) = 0;
  virtual int connect(const char *host, uint16_t port) = 0;
  // The two-arg buffer form hides Stream's no-arg read() unless re-exposed.
  using Stream::read;
  virtual int read(uint8_t *buf, size_t size) = 0;
  virtual void stop() = 0;
  virtual uint8_t connected() = 0;
  virtual operator bool() = 0;
};
