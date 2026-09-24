#pragma once

// Host stub for ESP-IDF's capability-aware heap API.
//
// The firmware asks these directly (TtfEpdFont, SdCardFontSystem) to decide
// whether a font can live in PSRAM. The desktop has no PSRAM, so SPIRAM
// queries report zero -- matching HalMemory::getPsramHeap() -- and the
// firmware takes its "no PSRAM" branch, which is the truthful answer here.
// Internal-RAM queries report the mock heap so those checks still behave.

#include <cstddef>
#include <cstdlib>

#include "Arduino.h"

#define MALLOC_CAP_DEFAULT (1 << 0)
#define MALLOC_CAP_8BIT (1 << 2)
#define MALLOC_CAP_INTERNAL (1 << 11)
#define MALLOC_CAP_SPIRAM (1 << 10)

inline size_t heap_caps_get_free_size(const uint32_t caps) {
  if (caps & MALLOC_CAP_SPIRAM) return 0;
  return ESP.getFreeHeap();
}

inline size_t heap_caps_get_largest_free_block(const uint32_t caps) {
  if (caps & MALLOC_CAP_SPIRAM) return 0;
  return ESP.getMaxAllocHeap();
}

inline size_t heap_caps_get_minimum_free_size(const uint32_t caps) {
  if (caps & MALLOC_CAP_SPIRAM) return 0;
  return ESP.getMinFreeHeap();
}

inline size_t heap_caps_get_total_size(const uint32_t caps) {
  if (caps & MALLOC_CAP_SPIRAM) return 0;
  return ESP.getHeapSize();
}

// Capabilities carry no meaning on the host: a SPIRAM-only request must fail
// the way it would on a board without PSRAM, and everything else is malloc.
inline void* heap_caps_malloc(const size_t bytes, const uint32_t caps) {
  if ((caps & MALLOC_CAP_SPIRAM) && !(caps & MALLOC_CAP_INTERNAL)) return nullptr;
  return malloc(bytes);
}

inline void heap_caps_free(void* ptr) { free(ptr); }
