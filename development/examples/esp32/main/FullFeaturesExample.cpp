/**
 * @file FullFeaturesExample.cpp
 * @brief Comprehensive example demonstrating all as5047u::AS5047U features
 *
 * This example demonstrates:
 * - All sensor features enabled
 * - Angle, velocity, diagnostics
 * - ABI, UVW, PWM configuration
 * - DAEC and filter settings
 * - OTP programming
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

static const char *TAG = "AS5047U_Full";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "as5047u::AS5047U Full Features Example");
  ESP_LOGI(TAG, "=============================");

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

  // Configure advanced features
  encoder.setDynamicAngleCompensation(true);
  ESP_LOGI(TAG, "DAEC enabled");

  encoder.setAdaptiveFilter(true);
  ESP_LOGI(TAG, "Adaptive filter enabled");

  encoder.setFilterParameters(2, 3);
  ESP_LOGI(TAG, "Filter parameters set: K_min=2, K_max=3");

  // Configure outputs
  encoder.setABIResolution(12);
  encoder.setUVWPolePairs(5);
  encoder.configureInterface(true, true, false);
  ESP_LOGI(TAG, "Outputs configured: ABI (12-bit), UVW (5 pole pairs)");

  ESP_LOGI(TAG, "Starting comprehensive reading loop...");

  // Main reading loop
  while (true) {
    // Read angle
    uint16_t angle = encoder.getAngle();
    uint16_t raw_angle = encoder.getRawAngle();
    double angle_deg = angle * 360.0 / 16384.0;
    ESP_LOGI(TAG, "Angle: %u (%.2f°), Raw: %u", angle, angle_deg, raw_angle);

    // Read velocity
    double vel_deg = encoder.getVelocityDegPerSec();
    double vel_rpm = encoder.getVelocityRPM();
    ESP_LOGI(TAG, "Velocity: %.2f deg/s, %.2f RPM", vel_deg, vel_rpm);

    // Read diagnostics
    uint8_t agc = encoder.getAGC();
    uint16_t mag = encoder.getMagnitude();
    ESP_LOGI(TAG, "AGC: %u, Magnitude: %u", agc, mag);

    // Check for errors
    AS5047U_Error errors = encoder.getStickyErrorFlags();
    if (errors != AS5047U_Error::None) {
      ESP_LOGW(TAG, "Errors: 0x%04X", static_cast<uint16_t>(errors));
    }

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
