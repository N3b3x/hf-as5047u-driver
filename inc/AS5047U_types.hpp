/**
 * @file as5047u_types.hpp
 * @brief Type definitions for the AS5047U driver.
 *
 * This header contains type definitions that are shared between as5047u.hpp
 * and as5047u_config.hpp to avoid circular dependencies.
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
