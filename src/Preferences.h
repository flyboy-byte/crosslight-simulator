#pragma once
// Minimal stand-in for ESP32 Arduino's Preferences (NVS key-value storage).
// In-process only (not persisted to disk) -- good enough for exercising logic
// that reads/writes a value within one simulator run; does not need to
// survive a relaunch the way real NVS does.
#include <cstdint>
#include <map>
#include <string>

class Preferences {
public:
  bool begin(const char *name, bool readOnly = false) {
    _ns = name ? name : "";
    _readOnly = readOnly;
    return true;
  }
  void end() { _ns.clear(); }

  int64_t getLong64(const char *key, int64_t defaultValue = 0) const {
    const auto it = store().find(_ns + "/" + key);
    return it == store().end() ? defaultValue : it->second;
  }
  bool putLong64(const char *key, int64_t value) {
    if (_readOnly) return false;
    store()[_ns + "/" + key] = value;
    return true;
  }

private:
  static std::map<std::string, int64_t> &store() {
    static std::map<std::string, int64_t> s;
    return s;
  }
  std::string _ns;
  bool _readOnly = false;
};
