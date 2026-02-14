/**
 * @file esp32_as5047u_test_config.hpp
 * @brief Hardware configuration for AS5047U driver on ESP32-C6
 * 
 * This file contains the actual hardware configuration that is used by the HAL
 * and example applications. Modify these values to match your hardware setup.
 * 
 * @author N3b3x
 * @date 2025
 * @version 1.0.0
 */

#pragma once

#include <cstdint>

//==============================================================================
// COMPILE-TIME CONFIGURATION FLAGS
//==============================================================================

/**
 * @brief Enable detailed SPI transaction logging
 * 
 * @details
 * When enabled (set to 1), the Esp32As5047uSpiBus will log detailed
 * information about each SPI transaction including:
 * - TX/RX frame bytes
 * - Register read/write details
 * - Parity and error flag checks
 * 
 * When disabled (set to 0), only basic error logging is performed.
 * 
 * Default: 1 (enabled) for integration test - Set to 0 to reduce log noise
 */
#ifndef ESP32_AS5047U_ENABLE_DETAILED_SPI_LOGGING
#define ESP32_AS5047U_ENABLE_DETAILED_SPI_LOGGING 1
#endif

namespace AS5047U_TestConfig {

/**
 * @brief SPI Configuration for ESP32-C6
 * 
 * These pins are used for SPI communication with the AS5047U.
 * Ensure your hardware matches these pin assignments or modify accordingly.
 */
struct SPIPins {
    static constexpr uint8_t MISO = 13;          ///< GPIO13 - SPI MISO (Master In Slave Out)
    static constexpr uint8_t MOSI = 11;          ///< GPIO11 - SPI MOSI (Master Out Slave In)
    static constexpr uint8_t SCLK = 12;          ///< GPIO12 - SPI Clock (SCLK)
    static constexpr uint8_t CS   = 10;          ///< GPIO10 - Chip Select (active low)
};

/**
 * @brief Control GPIO Pins for AS5047U
 * 
 * Optional pins for additional encoder functionality.
 * Set to -1 if not connected/configured.
 */
struct ControlPins {
    static constexpr int8_t INDEX_PIN = -1;     ///< Index pulse output (U pin, once per revolution)
    static constexpr int8_t PWM_PIN = -1;       ///< PWM output pin (W pin)
    static constexpr int8_t ABI_A = -1;         ///< ABI incremental output A
    static constexpr int8_t ABI_B = -1;         ///< ABI incremental output B
};

/**
 * @brief SPI Communication Parameters
 * 
 * The AS5047U supports SPI frequencies up to 10MHz.
 * 
 * SPI Mode: CPOL=0, CPHA=1 (Mode 1)
 * Data format: 16-bit frames (default) or 24-bit frames (SPI_24 mode)
 * 
 * AS5047U SPI timing (per datasheet):
 * - Maximum SPI clock: 10 MHz
 * - CS setup time (tCSS): 350ns min before first SCLK edge
 * - CS hold time (tCSH): 50ns min after last SCLK edge
 * - CS high time between frames (tCSmin): 350ns min
 * 
 * cs_ena_pretrans and cs_ena_posttrans are in clock cycles.
 * At 4MHz: 1 cycle = 250ns, so 2 cycles = 500ns (above 350ns minimum)
 */
struct SPIParams {
    static constexpr uint32_t FREQUENCY = 1000000;    ///< 4MHz SPI frequency (conservative default)
    static constexpr uint8_t MODE = 1;                ///< SPI Mode 1 (CPOL=0, CPHA=1)
    static constexpr uint8_t QUEUE_SIZE = 1;          ///< Transaction queue size
    static constexpr uint8_t CS_ENA_PRETRANS = 1;     ///< CS asserted N clock cycles before transaction
    static constexpr uint8_t CS_ENA_POSTTRANS = 1;   ///< CS held N clock cycles after transaction
    /// SPI host: 2 = SPI2_HOST, 3 = SPI3_HOST. Try 3 if SCLK does not toggle on your board.
    static constexpr uint8_t SPI_HOST_ID = 2;
};

/**
 * @brief Encoder Resolution and Specifications
 * 
 * AS5047U is a 14-bit on-axis magnetic rotary position sensor.
 */
struct EncoderSpecs {
    static constexpr uint16_t RESOLUTION_BITS = 14;          ///< Angular resolution in bits
    static constexpr uint16_t COUNTS_PER_REV = 16384;        ///< 2^14 counts per revolution
    static constexpr float DEGREES_PER_COUNT = 360.0f / 16384.0f; ///< Degrees per LSB (~0.022°)
    static constexpr float MAX_ROTATION_SPEED_RPM = 28000.0f; ///< Maximum rotation speed (RPM)
};

/**
 * @brief Supply Voltage Specifications (volts)
 * 
 * VDD: Power supply for AS5047U
 * VDD3V3: Regulated output (if used)
 */
struct SupplyVoltage {
    static constexpr float VDD_MIN = 3.0f;     ///< Minimum VDD voltage (V)
    static constexpr float VDD_NOM = 3.3f;     ///< Nominal VDD voltage (V)
    static constexpr float VDD_MAX = 3.6f;     ///< Maximum VDD voltage (V)
    static constexpr float VDD5V_MIN = 4.5f;   ///< Minimum 5V supply voltage (V)
    static constexpr float VDD5V_NOM = 5.0f;   ///< Nominal 5V supply voltage (V)
    static constexpr float VDD5V_MAX = 5.5f;   ///< Maximum 5V supply voltage (V)
};

/**
 * @brief Temperature Specifications (celsius)
 * 
 * Operating temperature range from AS5047U datasheet.
 */
struct Temperature {
    static constexpr int16_t OPERATING_MIN = -40;    ///< Minimum operating temperature (°C)
    static constexpr int16_t OPERATING_MAX = 150;    ///< Maximum operating temperature (°C)
    static constexpr int16_t WARNING_THRESHOLD = 130; ///< Temperature warning threshold (°C)
};

/**
 * @brief Timing Parameters
 * 
 * Timing requirements from the AS5047U datasheet.
 */
struct Timing {
    static constexpr uint16_t POWER_ON_DELAY_MS = 10;       ///< Power-on initialization delay (ms)
    static constexpr uint16_t READOUT_LATENCY_US = 100;     ///< Read latency for angle data (μs)
    static constexpr uint16_t INTER_FRAME_US = 1;           ///< Minimum time between SPI frames (μs)
};

/**
 * @brief Diagnostic Thresholds
 * 
 * Thresholds for magnetic field diagnostics and error detection.
 */
struct Diagnostics {
    static constexpr uint16_t POLL_INTERVAL_MS = 100;      ///< Diagnostic polling interval (ms)
    static constexpr uint8_t MAX_RETRY_COUNT = 3;          ///< Maximum communication retries
    static constexpr uint8_t CORDIC_OVERFLOW_LIMIT = 5;    ///< CORDIC overflow error threshold
};

/**
 * @brief Test Configuration
 * 
 * Default parameters for testing.
 */
struct TestConfig {
    static constexpr uint16_t ANGLE_READ_COUNT = 100;       ///< Number of angle reads per test
    static constexpr uint16_t VELOCITY_SAMPLE_MS = 10;      ///< Velocity sampling interval (ms)
    static constexpr uint16_t TEST_DURATION_MS = 5000;      ///< Test duration (ms)
    static constexpr float ANGLE_TOLERANCE_DEG = 0.5f;      ///< Angle reading tolerance (degrees)
};

/**
 * @brief Application-specific Configuration
 * 
 * Configuration values that can be adjusted per application.
 */
struct AppConfig {
    // Logging
    static constexpr bool ENABLE_DEBUG_LOGGING = true;     ///< Enable detailed debug logs
    static constexpr bool ENABLE_SPI_LOGGING = false;      ///< Enable SPI transaction logs
    
    // Performance
    static constexpr bool ENABLE_PERFORMANCE_MONITORING = true;  ///< Enable performance metrics
    static constexpr uint16_t STATS_REPORT_INTERVAL_MS = 10000;  ///< Statistics reporting interval
    
    // Error handling
    static constexpr bool ENABLE_AUTO_RECOVERY = true;     ///< Enable automatic error recovery
    static constexpr uint8_t MAX_ERROR_COUNT = 10;         ///< Maximum errors before failsafe
};

} // namespace AS5047U_TestConfig

/**
 * @brief Hardware configuration validation
 * 
 * Compile-time checks to ensure configuration is valid.
 */
static_assert(AS5047U_TestConfig::SPIParams::FREQUENCY <= 10000000,
              "SPI frequency exceeds AS5047U maximum of 10MHz");

static_assert(AS5047U_TestConfig::SPIParams::MODE == 1,
              "AS5047U requires SPI Mode 1 (CPOL=0, CPHA=1)");

static_assert(AS5047U_TestConfig::EncoderSpecs::RESOLUTION_BITS == 14,
              "AS5047U is a 14-bit encoder");

static_assert(AS5047U_TestConfig::EncoderSpecs::COUNTS_PER_REV == (1 << 14),
              "Counts per revolution must match 2^RESOLUTION_BITS");

/**
 * @brief Helper macro for compile-time GPIO pin validation
 */
#define AS5047U_VALIDATE_GPIO(pin) \
    static_assert((pin) >= 0 && (pin) < 30, "Invalid GPIO pin number for ESP32-C6")
