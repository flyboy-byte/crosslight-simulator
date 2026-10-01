#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "Client.h"
#include "WString.h"

class Stream;

class NetworkClient : public Client {
public:
  NetworkClient() {}
  explicit NetworkClient(int fd);
  ~NetworkClient() override {}
  virtual int connect(const char *host, uint16_t port) override;
  int connect(IPAddress ip, uint16_t port) override { return connect(ip.toString().c_str(), port); }
  virtual size_t write(const uint8_t *buf, size_t size) override;
  virtual size_t write(const char *str) {
    return write((const uint8_t *)str, strlen(str));
  }
  virtual size_t write(uint8_t c) override { return write(&c, 1); }
  virtual size_t write(Stream &stream);
  template <typename T> size_t write(T &streamLike) {
    uint8_t buffer[4096];
    size_t total = 0;
    while (streamLike.available() > 0) {
      const int count = streamLike.read(buffer, sizeof(buffer));
      if (count <= 0)
        break;
      const size_t written = write(buffer, static_cast<size_t>(count));
      total += written;
      if (written != static_cast<size_t>(count))
        break;
    }
    return total;
  }
  int available() override;
  int read() override;
  int read(uint8_t *buf, size_t size) override;
  int peek() override;
  void stop() override;
  virtual void clear() {}
  uint8_t connected() override;
  void setConnectionTimeout(uint32_t) {}
  operator bool() override { return connected(); }

private:
  struct Impl;
  std::shared_ptr<Impl> impl_;
};

class NetworkClientSecure : public NetworkClient {
public:
  void setHandshakeTimeout(uint32_t seconds) { (void)seconds; }
  void setInsecure() {}
};
