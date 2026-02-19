/**
 * @file velocity_reading_example.cpp
 * @brief Velocity reading example with unit conversions
 *
 * This example demonstrates:
 * - Velocity reading in multiple units
 * - Degrees per second, radians per second, RPM
 *
 * @author N3b3x
 * @date 2025
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>
#include <stdio.h>

#include "../../../inc/as5047u.hpp"
#include "esp32_as5047u_bus.hpp"
#include "esp32_as5047u_test_config.hpp"

static const char* TAG = "AS5047U_Velocity";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "as5047u::AS5047U Velocity Reading Example");
  ESP_LOGI(TAG, "=================================");
  ESP_LOGI(TAG, "Driver version: %s", as5047u::GetDriverVersion());

  // Create SPI bus using centralized test config
  auto bus = CreateEsp32As5047uSpiBus();

  if (!bus) {
    ESP_LOGE(TAG, "Failed to create SPI bus");
    return;
  }

  if (!bus->initialize()) {
    ESP_LOGE(TAG, "Failed to initialize SPI bus");
    return;
  }

  // Create encoder instance
  as5047u::AS5047U encoder(*bus, FrameFormat::SPI_24);

  ESP_LOGI(TAG, "as5047u::AS5047U encoder initialized");
  ESP_LOGI(TAG, "Starting velocity reading loop...");

  // Main reading loop
  while (true) {
    // Read velocity in different units
    float velocity_lsb = encoder.GetVelocity(as5047u::VelocityUnit::Lsb);
    float vel_deg = encoder.GetVelocity(as5047u::VelocityUnit::DegPerSec);
    float vel_rad = encoder.GetVelocity(as5047u::VelocityUnit::RadPerSec);
    float vel_rpm = encoder.GetVelocity(as5047u::VelocityUnit::Rpm);

    ESP_LOGI(TAG, "Velocity: %.0f LSB, %.2f deg/s, %.2f rad/s, %.2f RPM", velocity_lsb, vel_deg,
             vel_rad, vel_rpm);

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
