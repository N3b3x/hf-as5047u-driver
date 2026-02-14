/**
 * @file as5047u_types.hpp
 * @brief Type definitions for the AS5047U driver
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */
#pragma once
#include <cstdint>

/**
 * @brief Supported SPI frame formats for AS5047U communication.
 */
enum class FrameFormat : uint8_t {
  SPI_16, /**< 16-bit frames (no CRC, high-throughput mode) */
  SPI_24, /**< 24-bit frames (includes 8-bit CRC for reliability) */
  SPI_32  /**< 32-bit frames (includes 8-bit CRC and 8-bit pad for daisy-chain)
           */
};

/**
 * @brief Presets for the adaptive velocity/angle filter (Dynamic Filter System).
 *
 * Maps to datasheet Figure 17 "Angular Velocity Measurement Filter Parameters".
 * Lower K = less noise at standstill, higher latency; higher K = more noise,
 * faster response. Use SetFilterPreset() for one-call configuration.
 */
enum class FilterPreset : uint8_t {
  /** Minimum noise at standstill (~5.8 °/s RMS). Best for low-speed or static use. */
  LowNoise = 0,
  /** Balanced noise vs response (~8–20 °/s). Good default for most applications. */
  Balanced,
  /** Maximum bandwidth, highest standstill noise (~245 °/s). Use when speed matters. */
  HighBandwidth
};
