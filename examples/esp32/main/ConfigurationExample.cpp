/**
 * @file ConfigurationExample.cpp
 * @brief Configuration example with ABI, UVW, and PWM setup
 *
 * This example demonstrates:
 * - ABI output configuration
 * - UVW commutation setup
 * - PWM output configuration
 * - Zero position and direction setting
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

static const char *TAG = "AS5047U_Config";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "as5047u::AS5047U Configuration Example");
  ESP_LOGI(TAG, "==============================");

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

  // Create encoder instance
  as5047u::AS5047U encoder(*bus, FrameFormat::SPI_24);

  ESP_LOGI(TAG, "as5047u::AS5047U encoder initialized");

  // Configure ABI output (12-bit resolution)
  encoder.setABIResolution(12);
  ESP_LOGI(TAG, "ABI output configured: 12-bit resolution");

  // Configure UVW commutation (5 pole pairs)
  encoder.setUVWPolePairs(5);
  ESP_LOGI(TAG, "UVW commutation configured: 5 pole pairs");

  // Configure interface (ABI enabled, UVW disabled, PWM disabled)
  encoder.configureInterface(true, false, false);
  ESP_LOGI(TAG, "Interface configured: ABI enabled");

  // Set zero position (example: 0)
  encoder.setZeroPosition(0);
  ESP_LOGI(TAG, "Zero position set: 0");

  // Set direction (clockwise)
  encoder.setDirection(true);
  ESP_LOGI(TAG, "Direction set: clockwise");

  ESP_LOGI(TAG, "Configuration complete. Starting angle reading...");

  // Main reading loop
  while (true) {
    uint16_t angle = encoder.getAngle();
    double angle_deg = angle * 360.0 / 16384.0;
    ESP_LOGI(TAG, "Angle: %u (%.2f°)", angle, angle_deg);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
