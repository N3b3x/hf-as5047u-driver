/**
 * @file BasicAngleReadingExample.cpp
 * @brief Basic angle reading example with diagnostics
 *
 * This example demonstrates:
 * - Basic initialization
 * - Angle reading in LSB and degrees
 * - AGC and magnitude diagnostics
 * - Error flag monitoring
 *
 * @author N3b3x
 * @date 2025
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>
#include <stdio.h>

#include "../../../inc/AS5047U.hpp"
#include "Esp32As5047uBus.hpp"

static const char* TAG = "AS5047U_Basic";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "as5047u::AS5047U Basic Angle Reading Example");
  ESP_LOGI(TAG, "===================================");

  // Configure SPI bus
  Esp32As5047uBus::SPIConfig config;
  config.miso_pin = GPIO_NUM_2;
  config.mosi_pin = GPIO_NUM_7;
  config.sclk_pin = GPIO_NUM_6;
  config.cs_pin = GPIO_NUM_10;
  config.frequency = 4000000;
  config.mode = 1;

  auto bus = std::make_unique<Esp32As5047uBus>(config);

  if (!bus->initialize()) {
    ESP_LOGE(TAG, "Failed to initialize SPI bus");
    return;
  }

  // Create encoder instance (24-bit frame format with CRC)
  as5047u::AS5047U encoder(*bus, FrameFormat::SPI_24);

  ESP_LOGI(TAG, "as5047u::AS5047U encoder initialized");
  ESP_LOGI(TAG, "Starting angle reading loop...");

  // Main reading loop
  while (true) {
    // Read angle
    uint16_t angle = encoder.GetAngle();
    float angle_deg = angle * 360.0f / 16384.0f;
    ESP_LOGI(TAG, "Angle: %u (%.2f°)", angle, angle_deg);

    // Read diagnostics
    uint8_t agc = encoder.GetAGC();
    uint16_t mag = encoder.GetMagnitude();
    ESP_LOGI(TAG, "AGC: %u, Magnitude: %u", agc, mag);

    // Check for errors
    AS5047U_Error errors = encoder.GetStickyErrorFlags();
    if (errors != AS5047U_Error::None) {
      ESP_LOGW(TAG, "Errors detected: 0x%04X", static_cast<uint16_t>(errors));
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
