#pragma once
class BatteryMonitor {
public:
  BatteryMonitor(int pin, int factor = -1) {}
  void begin() {}
  int getVoltage() { return 4200; }
  int getPercentage() { return 100; }
  // Added upstream (X3 BQ27220 fuel gauge fix, PR #3730): writes design
  // capacity into the real fuel gauge chip. No such chip on the host, so this
  // is a no-op success.
  static bool loadDesignCapacity() { return true; }
};
