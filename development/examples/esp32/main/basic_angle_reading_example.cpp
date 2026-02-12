/**
 * @file basic_angle_reading_example.cpp
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

#include "../../../inc/as5047u.hpp"
#include "esp32_as5047u_bus.hpp"
#include "esp32_as5047u_test_config.hpp"

// Forward declaration of a detailed diagnostics routine that
// exercises the low-level register API and verifies that
// configuration writes can be read back correctly.
static void run_diagnostics(as5047u::AS5047U<Esp32As5047uBus>& encoder);

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

  // Create encoder instance
  // Use 16-bit frame format (no CRC) for maximum compatibility.
  as5047u::AS5047U encoder(*bus, FrameFormat::SPI_16);

  ESP_LOGI(TAG, "as5047u::AS5047U encoder initialized");

  // Give the sensor some time after power-on (datasheet tpon ~10ms).
  vTaskDelay(pdMS_TO_TICKS(20));

  // Run a one-shot diagnostics and configuration verification pass.
  run_diagnostics(encoder);

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

// Detailed diagnostics: read core registers, toggle a few configuration bits,
// and read them back so we can verify that SPI writes are actually sticking
// and that we are not just seeing a stuck-low MISO line.
static void run_diagnostics(as5047u::AS5047U<Esp32As5047uBus>& encoder) {
  ESP_LOGI(TAG, "Running AS5047U diagnostics and configuration verification...");

  // 1) Dump comprehensive status using the driver's helper.
  encoder.DumpStatus();

  // 2) Read core status/diagnostic registers directly.
  auto errfl = encoder.ReadReg<AS5047U_REG::ERRFL>();
  ESP_LOGI(TAG, "ERRFL raw: 0x%04X", errfl.value);

  auto dia = encoder.ReadReg<AS5047U_REG::DIA>();
  ESP_LOGI(TAG,
           "DIA: 0x%04X (VDD_mode=%u LoopsFinished=%u OffComp_finished=%u AGC_finished=%u "
           "CORDIC_overflow=%u MagHalf_flag=%u)",
           dia.value, dia.bits.VDD_mode, dia.bits.LoopsFinished, dia.bits.OffComp_finished,
           dia.bits.AGC_finished, dia.bits.CORDIC_overflow_flag, dia.bits.MagHalf_flag);

  auto agc_reg = encoder.ReadReg<AS5047U_REG::AGC>();
  ESP_LOGI(TAG, "AGC register raw: 0x%04X (AGC_value=%u)", agc_reg.value, agc_reg.bits.AGC_value);

  auto mag_reg = encoder.ReadReg<AS5047U_REG::MAG>();
  ESP_LOGI(TAG, "MAG register raw: 0x%04X (MAG_value=%u)", mag_reg.value, mag_reg.bits.MAG_value);

  auto angle_unc = encoder.ReadReg<AS5047U_REG::ANGLEUNC>();
  auto angle_com = encoder.ReadReg<AS5047U_REG::ANGLECOM>();
  ESP_LOGI(TAG, "ANGLEUNC raw: 0x%04X (value=%u)", angle_unc.value, angle_unc.bits.ANGLEUNC_value);
  ESP_LOGI(TAG, "ANGLECOM raw: 0x%04X (value=%u)", angle_com.value, angle_com.bits.ANGLECOM_value);

  // 3) Verify that non-volatile configuration registers can be written and read back.
  //    We toggle a few bits and immediately restore the original value.

  // SETTINGS2: test NOISESET and PWMon bits.
  auto s2_orig = encoder.ReadReg<AS5047U_REG::SETTINGS2>();
  ESP_LOGI(TAG, "SETTINGS2 original: 0x%04X", s2_orig.value);

  auto s2_test = s2_orig;
  s2_test.bits.NOISESET ^= 1u;
  s2_test.bits.PWMon ^= 1u;
  bool s2_write_ok = encoder.WriteReg(s2_test);
  auto s2_after = encoder.ReadReg<AS5047U_REG::SETTINGS2>();
  ESP_LOGI(TAG,
           "SETTINGS2 test write ok=%d, written=0x%04X, read-back=0x%04X "
           "(NOISESET=%u PWMon=%u)",
           s2_write_ok ? 1 : 0, s2_test.value, s2_after.value, s2_after.bits.NOISESET,
           s2_after.bits.PWMon);

  // Restore original SETTINGS2.
  if (!encoder.WriteReg(s2_orig)) {
    ESP_LOGW(TAG, "Failed to restore original SETTINGS2");
  }

  // DISABLE: toggle ABI_off and UVW_off.
  auto dis_orig = encoder.ReadReg<AS5047U_REG::DISABLE>();
  ESP_LOGI(TAG, "DISABLE original: 0x%04X", dis_orig.value);

  auto dis_test = dis_orig;
  dis_test.bits.ABI_off ^= 1u;
  dis_test.bits.UVW_off ^= 1u;
  bool dis_write_ok = encoder.WriteReg(dis_test);
  auto dis_after = encoder.ReadReg<AS5047U_REG::DISABLE>();
  ESP_LOGI(TAG,
           "DISABLE test write ok=%d, written=0x%04X, read-back=0x%04X "
           "(ABI_off=%u UVW_off=%u)",
           dis_write_ok ? 1 : 0, dis_test.value, dis_after.value, dis_after.bits.ABI_off,
           dis_after.bits.UVW_off);

  // Restore original DISABLE.
  if (!encoder.WriteReg(dis_orig)) {
    ESP_LOGW(TAG, "Failed to restore original DISABLE");
  }

  // SETTINGS3: read back ABIRES and HYS to confirm they are not stuck at zero.
  auto s3 = encoder.ReadReg<AS5047U_REG::SETTINGS3>();
  ESP_LOGI(TAG, "SETTINGS3: 0x%04X (UVWPP=%u HYS=%u ABIRES=%u)", s3.value, s3.bits.UVWPP,
           s3.bits.HYS, s3.bits.ABIRES);

  // Finally, read sticky error flags after the above transactions.
  auto sticky = encoder.GetStickyErrorFlags();
  ESP_LOGI(TAG, "Sticky error flags after diagnostics: 0x%04X",
           static_cast<uint16_t>(sticky));

  ESP_LOGI(TAG, "Diagnostics completed.");
}
