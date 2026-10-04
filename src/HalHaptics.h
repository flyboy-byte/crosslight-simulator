#pragma once

#include <cstdint>

// Simulator stub for the haptic HAL. The desktop build has no vibration motor,
// so every entry point is a no-op. Mirrors the public API of the firmware's
// lib/hal/HalHaptics.h (which is itself all no-ops under CROSSPOINT_EMULATED).
class HalHaptics {
 public:
  static void longPress(bool /*enabled*/, uint8_t /*intensity*/) {}
  static void feedback(bool /*enabled*/, bool /*pressed*/, uint8_t /*intensity*/) {}
};
