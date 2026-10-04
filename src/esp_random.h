#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>
inline uint32_t esp_random() { return static_cast<uint32_t>(rand()); }
// Fill a buffer with (non-cryptographic) random bytes. Good enough for the
// desktop build, where DeviceSecret/BookKey just need a per-run value.
inline void esp_fill_random(void *buf, size_t len) {
  auto *p = static_cast<uint8_t *>(buf);
  for (size_t i = 0; i < len; ++i) p[i] = static_cast<uint8_t>(rand());
}
