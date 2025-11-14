/**
 * @file VelocityReadingExample.cpp
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

#include "../../../inc/AS5047U.hpp"
#include "Esp32As5047uBus.hpp"

static const char *TAG = "AS5047U_Velocity";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "AS5047U Velocity Reading Example");
  ESP_LOGI(TAG, "=================================");

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
  as5047u::Encoder encoder(*bus, FrameFormat::SPI_24);

  ESP_LOGI(TAG, "AS5047U encoder initialized");
  ESP_LOGI(TAG, "Starting velocity reading loop...");

  // Main reading loop
  while (true) {
    // Read velocity in different units
    int16_t velocity_lsb = encoder.getVelocity();
    double vel_deg = encoder.getVelocityDegPerSec();
    double vel_rad = encoder.getVelocityRadPerSec();
    double vel_rpm = encoder.getVelocityRPM();

    ESP_LOGI(TAG, "Velocity: %d LSB, %.2f deg/s, %.2f rad/s, %.2f RPM",
             velocity_lsb, vel_deg, vel_rad, vel_rpm);

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
