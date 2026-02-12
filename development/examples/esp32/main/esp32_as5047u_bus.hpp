/**
 * @file esp32_as5047u_bus.hpp
 * @brief ESP32 SPI transport implementation for AS5047U driver (header-only)
 *
 * This file provides the ESP32-specific implementation of the as5047u::SpiInterface
 * interface for communicating with AS5047U magnetic encoder over SPI.
 *
 * Header-only implementation — no separate .cpp file needed.
 *
 * @author N3b3x
 * @date 2025
 * @version 2.0.0
 */

#pragma once

#include "../../../inc/as5047u_spi_interface.hpp"
#include "esp32_as5047u_test_config.hpp"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include <cstdint>
#include <memory>

/**
 * @class Esp32As5047uBus
 * @brief ESP32 SPI transport implementation for AS5047U driver
 *
 * This class implements the as5047u::SpiInterface interface using ESP-IDF's SPI
 * driver with CRTP pattern. It supports configurable SPI pins, frequency, and
 * chip select.
 */
class Esp32As5047uBus : public as5047u::SpiInterface<Esp32As5047uBus> {
public:
  /**
   * @brief SPI configuration structure
   */
  struct SPIConfig {
    spi_host_device_t host = SPI2_HOST; ///< SPI host (SPI2_HOST for ESP32-S3)
    gpio_num_t miso_pin = GPIO_NUM_13;   ///< MISO pin (default GPIO13)
    gpio_num_t mosi_pin = GPIO_NUM_11;   ///< MOSI pin (default GPIO11)
    gpio_num_t sclk_pin = GPIO_NUM_12;   ///< SCLK pin (default GPIO12)
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
  explicit Esp32As5047uBus(const SPIConfig &config) : config_(config) {}

  /**
   * @brief Destructor - cleans up SPI resources
   */
  ~Esp32As5047uBus() { deinitialize(); }

  /**
   * @brief Perform a full-duplex SPI data transfer
   * @param tx Pointer to data to transmit (len bytes). If nullptr, zeros are
   * sent.
   * @param rx Pointer to buffer for received data (len bytes). If nullptr,
   * received data is ignored.
   * @param len Number of bytes to transfer.
   */
  void transfer(const uint8_t *tx, uint8_t *rx, std::size_t len) {
    if (!initialized_ || spi_device_ == nullptr) {
      ESP_LOGE(TAG, "SPI bus not initialized");
      return;
    }

    spi_transaction_t trans = {};
    trans.length = len * 8; // Length in bits
    trans.tx_buffer = tx;
    trans.rx_buffer = rx;

    esp_err_t ret = spi_device_transmit(spi_device_, &trans);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "SPI transfer failed: %s", esp_err_to_name(ret));
    }
  }

  /**
   * @brief Initialize the SPI bus (must be called before use)
   * @return true if successful, false otherwise
   */
  bool initialize() {
    if (initialized_) {
      ESP_LOGW(TAG, "SPI bus already initialized");
      return true;
    }

    if (!initializeSPI()) {
      ESP_LOGE(TAG, "Failed to initialize SPI bus");
      return false;
    }

    if (!addSPIDevice()) {
      ESP_LOGE(TAG, "Failed to add SPI device");
      deinitialize();
      return false;
    }

    initialized_ = true;
    ESP_LOGI(TAG, "SPI bus initialized successfully");
    return true;
  }

  /**
   * @brief Deinitialize the SPI bus
   */
  void deinitialize() {
    if (!initialized_) {
      return;
    }

    if (spi_device_ != nullptr) {
      spi_bus_remove_device(spi_device_);
      spi_device_ = nullptr;
    }

    spi_bus_free(config_.host);

    initialized_ = false;
    ESP_LOGI(TAG, "SPI bus deinitialized");
  }

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
  bool initializeSPI() {
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = config_.mosi_pin;
    buscfg.miso_io_num = config_.miso_pin;
    buscfg.sclk_io_num = config_.sclk_pin;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 64;

    esp_err_t ret = spi_bus_initialize(config_.host, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
      return false;
    }

    ESP_LOGI(TAG, "SPI bus configured: MISO=%d, MOSI=%d, SCLK=%d, Host=%d",
             config_.miso_pin, config_.mosi_pin, config_.sclk_pin, config_.host);

    return true;
  }

  /**
   * @brief Add SPI device to the bus
   * @return true if successful, false otherwise
   */
  bool addSPIDevice() {
    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = config_.frequency;
    devcfg.mode = config_.mode;
    devcfg.spics_io_num = config_.cs_pin;
    devcfg.queue_size = config_.queue_size;
    devcfg.cs_ena_pretrans = config_.cs_ena_pretrans;
    devcfg.cs_ena_posttrans = config_.cs_ena_posttrans;
    devcfg.flags = 0;

    esp_err_t ret = spi_bus_add_device(config_.host, &devcfg, &spi_device_);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "Failed to add SPI device: %s", esp_err_to_name(ret));
      return false;
    }

    ESP_LOGI(TAG, "SPI device added: CS=%d, Freq=%lu Hz, Mode=%d",
             config_.cs_pin, config_.frequency, config_.mode);

    return true;
  }
};

/**
 * @brief Factory function to create a configured Esp32As5047uBus instance
 *
 * Creates an Esp32As5047uBus using pin and parameter values from
 * AS5047U_TestConfig namespace (esp32_as5047u_test_config.hpp).
 *
 * @return std::unique_ptr<Esp32As5047uBus> Configured SPI bus interface
 */
inline auto CreateEsp32As5047uBus() noexcept -> std::unique_ptr<Esp32As5047uBus> {
  using namespace AS5047U_TestConfig;

  Esp32As5047uBus::SPIConfig config;

  // SPI pins from esp32_as5047u_test_config.hpp
  config.host = SPI2_HOST;
  config.miso_pin = static_cast<gpio_num_t>(SPIPins::MISO);
  config.mosi_pin = static_cast<gpio_num_t>(SPIPins::MOSI);
  config.sclk_pin = static_cast<gpio_num_t>(SPIPins::SCLK);
  config.cs_pin = static_cast<gpio_num_t>(SPIPins::CS);

  // SPI parameters from esp32_as5047u_test_config.hpp
  config.frequency = SPIParams::FREQUENCY;
  config.mode = SPIParams::MODE;
  config.queue_size = SPIParams::QUEUE_SIZE;
  config.cs_ena_pretrans = SPIParams::CS_ENA_PRETRANS;
  config.cs_ena_posttrans = SPIParams::CS_ENA_POSTTRANS;

  return std::make_unique<Esp32As5047uBus>(config);
}
