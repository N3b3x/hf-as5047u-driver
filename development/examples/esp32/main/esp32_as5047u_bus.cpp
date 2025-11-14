/**
 * @file esp32_as5047u_bus.cpp
 * @brief ESP32 SPI transport implementation for as5047u::AS5047U driver
 */

#include "esp32_as5047u_bus.hpp"
#include "esp_err.h"

Esp32As5047uBus::Esp32As5047uBus(const SPIConfig& config) : config_(config) {}

Esp32As5047uBus::~Esp32As5047uBus() {
  deinitialize();
}

bool Esp32As5047uBus::initialize() {
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

void Esp32As5047uBus::deinitialize() {
  if (!initialized_) {
    return;
  }

  // Remove SPI device
  if (spi_device_ != nullptr) {
    spi_bus_remove_device(spi_device_);
    spi_device_ = nullptr;
  }

  // Free SPI bus
  spi_bus_free(config_.host);

  initialized_ = false;
  ESP_LOGI(TAG, "SPI bus deinitialized");
}

void Esp32As5047uBus::transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) {
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

bool Esp32As5047uBus::initializeSPI() {
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

  ESP_LOGI(TAG, "SPI bus configured: MISO=%d, MOSI=%d, SCLK=%d, Host=%d", config_.miso_pin,
           config_.mosi_pin, config_.sclk_pin, config_.host);

  return true;
}

bool Esp32As5047uBus::addSPIDevice() {
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

  ESP_LOGI(TAG, "SPI device added: CS=%d, Freq=%lu Hz, Mode=%d", config_.cs_pin, config_.frequency,
           config_.mode);

  return true;
}
