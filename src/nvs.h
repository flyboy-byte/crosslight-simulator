#pragma once

#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

using nvs_handle_t = uint32_t;

constexpr int NVS_READONLY = 0;
constexpr int NVS_READWRITE = 1;

// NVS "key not found": the desktop build has no persistent NVS, so blob reads
// report this and callers regenerate (e.g. DeviceSecret makes a fresh secret).
#ifndef ESP_ERR_NVS_NOT_FOUND
#define ESP_ERR_NVS_NOT_FOUND -0x4100
#endif

inline esp_err_t nvs_open(const char * /*name*/, int /*mode*/, nvs_handle_t *handle) {
  *handle = 1;
  return ESP_OK;
}

inline esp_err_t nvs_get_u8(nvs_handle_t /*handle*/, const char * /*key*/, uint8_t * /*value*/) { return ESP_FAIL; }
inline esp_err_t nvs_set_u8(nvs_handle_t /*handle*/, const char * /*key*/, uint8_t /*value*/) { return ESP_OK; }
// No persistent NVS on the desktop build: blob reads always miss (callers then
// regenerate), blob writes succeed but are not stored across runs.
inline esp_err_t nvs_get_blob(nvs_handle_t /*handle*/, const char * /*key*/, void * /*out*/,
                              size_t * /*length*/) { return ESP_ERR_NVS_NOT_FOUND; }
inline esp_err_t nvs_set_blob(nvs_handle_t /*handle*/, const char * /*key*/, const void * /*value*/,
                              size_t /*length*/) { return ESP_OK; }
inline esp_err_t nvs_commit(nvs_handle_t /*handle*/) { return ESP_OK; }
inline void nvs_close(nvs_handle_t /*handle*/) {}
