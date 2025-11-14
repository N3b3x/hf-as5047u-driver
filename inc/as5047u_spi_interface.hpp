/**
 * @file as5047u_spi_interface.hpp
 * @brief CRTP-based SPI bus interface for AS5047U driver
 *
 * This header defines the hardware-agnostic SPI bus interface using the
 * Curiously Recurring Template Pattern (CRTP) for compile-time polymorphism.
 */

#pragma once
#include <cstddef>
#include <cstdint>

namespace as5047u {

/**
 * @brief CRTP-based template interface for SPI bus operations
 *
 * This template class provides a hardware-agnostic interface for SPI
 * communication using the CRTP pattern. Platform-specific implementations
 * should inherit from this template with themselves as the template parameter.
 *
 * Benefits of CRTP:
 * - Compile-time polymorphism (no virtual function overhead)
 * - Static dispatch instead of dynamic dispatch
 * - Better optimization opportunities for the compiler
 *
 * Example usage:
 * @code
 * class MySPI : public as5047u::SpiInterface<MySPI> {
 * public:
 *   void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) {
 *     // Platform-specific SPI implementation
 *   }
 * };
 * @endcode
 *
 * @tparam Derived The derived class type (CRTP pattern)
 */
template <typename Derived> class SpiInterface {
public:
  /**
   * @brief Perform a full-duplex SPI data transfer.
   *
   * Sends `len` bytes from `tx` and simultaneously receives `len` bytes into
   * `rx`. Implementations should assert the device's chip select for the
   * duration of the transfer.
   *
   * @param tx Pointer to data to transmit (len bytes). If nullptr, zeros can be
   * sent.
   * @param rx Pointer to buffer for received data (len bytes). If nullptr,
   * received data can be ignored.
   * @param len Number of bytes to transfer.
   */
  void transfer(const uint8_t *tx, uint8_t *rx, std::size_t len) {
    return static_cast<Derived *>(this)->transfer(tx, rx, len);
  }

protected:
  /**
   * @brief Protected constructor to prevent direct instantiation
   */
  SpiInterface() = default;

  // Prevent copying
  SpiInterface(const SpiInterface &) = delete;
  SpiInterface &operator=(const SpiInterface &) = delete;

  // Allow moving
  SpiInterface(SpiInterface &&) = default;
  SpiInterface &operator=(SpiInterface &&) = default;

  /**
   * @brief Protected destructor
   * @note Derived classes can have public destructors
   */
  ~SpiInterface() = default;
};

} // namespace as5047u
