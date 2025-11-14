/**
 * @file Esp32As5047uBus.hpp
 * @brief ESP32 SPI transport implementation for AS5047U driver
 *
 * This file provides the ESP32-specific implementation of the AS5047U::spiBus
 * interface for communicating with AS5047U magnetic encoder over SPI.
 *
 * @author N3b3x
 * @date 2025
 * @version 1.0.0
 */

#pragma once

#include "../../../inc/AS5047U_Bus.hpp"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include <cstdint>
#include <memory>

/**
 * @class Esp32As5047uBus
 * @brief ESP32 SPI transport implementation for AS5047U driver
 *
 * This class implements the as5047u::spiBus interface using ESP-IDF's SPI
 * driver with CRTP pattern. It supports configurable SPI pins, frequency, and
 * chip select.
 */
class Esp32As5047uBus : public as5047u::spiBus<Esp32As5047uBus> {
public:
  /**
   * @brief SPI configuration structure
   */
  struct SPIConfig {
    spi_host_device_t host = SPI2_HOST; ///< SPI host (SPI2_HOST for ESP32-C6)
    gpio_num_t miso_pin = GPIO_NUM_2;   ///< MISO pin (default GPIO2)
    gpio_num_t mosi_pin = GPIO_NUM_7;   ///< MOSI pin (default GPIO7)
    gpio_num_t sclk_pin = GPIO_NUM_6;   ///< SCLK pin (default GPIO6)
    gpio_num_t cs_pin = GPIO_NUM_10;    ///< CS pin (default GPIO10)
    uint32_t frequency = 4000000;       ///< SPI frequency in Hz (default 4MHz)
    uint8_t mode = 1;       ///< SPI mode (default 1: CPOL=0, CPHA=1)
    uint8_t queue_size = 1; ///< Transaction queue size
    uint8_t cs_ena_pretrans =
        1; ///< CS asserted N clock cycles before transaction
    uint8_t cs_ena_posttrans = 1; ///< CS held N clock cycles after transaction
  };

  /**
   * @brief Constructor with default SPI configuration
   */
  Esp32As5047uBus() : Esp32As5047uBus(SPIConfig{}) {}

  /**
   * @brief Constructor with custom SPI configuration
   * @param config SPI configuration parameters
   */
  explicit Esp32As5047uBus(const SPIConfig &config);

  /**
   * @brief Destructor - cleans up SPI resources
   */
  ~Esp32As5047uBus();

  /**
   * @brief Perform a full-duplex SPI data transfer
   * @param tx Pointer to data to transmit (len bytes). If nullptr, zeros are
   * sent.
   * @param rx Pointer to buffer for received data (len bytes). If nullptr,
   * received data is ignored.
   * @param len Number of bytes to transfer.
   */
  void transfer(const uint8_t *tx, uint8_t *rx, std::size_t len);

  /**
   * @brief Initialize the SPI bus (must be called before use)
   * @return true if successful, false otherwise
   */
  bool initialize();

  /**
   * @brief Deinitialize the SPI bus
   */
  void deinitialize();

  /**
   * @brief Get the current SPI configuration
   * @return Current SPI configuration
   */
  const SPIConfig &getConfig() const noexcept { return config_; }

  /**
   * @brief Check if SPI bus is initialized
   * @return true if initialized, false otherwise
   */
  bool isInitialized() const noexcept { return initialized_; }

private:
  SPIConfig config_;                         ///< SPI configuration
  spi_device_handle_t spi_device_ = nullptr; ///< SPI device handle
  bool initialized_ = false;                 ///< Initialization state
  static constexpr const char *TAG = "Esp32As5047uBus"; ///< Logging tag

  /**
   * @brief Initialize SPI bus
   * @return true if successful, false otherwise
   */
  bool initializeSPI();

  /**
   * @brief Add SPI device to the bus
   * @return true if successful, false otherwise
   */
  bool addSPIDevice();
};
