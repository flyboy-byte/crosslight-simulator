#include "HalMemory.h"

#include "Arduino.h"

#include <new>

namespace {
HalMemory::HeapStats readMockHeap() {
  return {ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getMinFreeHeap(), ESP.getMaxAllocHeap()};
}
}  // namespace

// The simulator has no separate internal/PSRAM heaps; both report the same mock heap.
HalMemory::HeapStats HalMemory::getDefaultHeap() { return readMockHeap(); }

HalMemory::HeapStats HalMemory::getInternalHeap() { return readMockHeap(); }

HalMemory::HeapStats HalMemory::getPsramHeap() { return {0, 0, 0, 0}; }

void HalMemory::PsramDeleter::operator()(uint8_t* buffer) const { delete[] buffer; }

HalMemory::PsramBuffer HalMemory::allocatePsram(const size_t bytes) {
  return PsramBuffer(new (std::nothrow) uint8_t[bytes]);
}
