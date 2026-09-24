#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

class HalMemory {
 public:
  struct HeapStats {
    size_t freeBytes;
    size_t totalBytes;
    size_t minFreeBytes;
    size_t largestBlockBytes;
  };

  // Mirrors the firmware's PSRAM allocator (lib/hal/HalMemory.h). The desktop
  // has no PSRAM, so this is a plain heap allocation -- callers only need the
  // buffer to exist and to free correctly.
  struct PsramDeleter {
    void operator()(uint8_t* buffer) const;
  };
  using PsramBuffer = std::unique_ptr<uint8_t[], PsramDeleter>;
  static PsramBuffer allocatePsram(size_t bytes);

  static HeapStats getDefaultHeap();
  static HeapStats getInternalHeap();
  static HeapStats getPsramHeap();
};
