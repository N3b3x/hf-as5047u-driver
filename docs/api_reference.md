# API Reference

Complete reference documentation for all public methods and types in the AS5047U driver.

## Source Code

- **Main Header**: [`inc/as5047u.hpp`](../inc/as5047u.hpp)
- **SPI Interface**: [`inc/as5047u_spi_interface.hpp`](../inc/as5047u_spi_interface.hpp)
- **Implementation**: [`src/AS5047U.cpp`](../src/AS5047U.cpp)
- **Registers**: [`inc/AS5047U_REGISTERS.hpp`](../inc/AS5047U_REGISTERS.hpp)

## Core Classes

### `AS5047U<SpiType>`

Main driver class for interfacing with the AS5047U magnetic encoder.

**Template Parameters:**
- `SpiType`: Type implementing `as5047u::SpiInterface<SpiType>`

**Constructor:**
```cpp
explicit AS5047U(SpiType& bus, FrameFormat format = AS5047U_CFG::DEFAULT_FRAME_FORMAT) noexcept;
```

**Location**: [`inc/as5047u.hpp#L91`](../inc/as5047u.hpp#L91)

**Parameters:**
- `bus`: Reference to SPI interface implementation
- `format`: SPI frame format (SPI_16, SPI_24, or SPI_32)

**Example:**
```cpp
MySpi spi;
as5047u::AS5047U encoder(spi, FrameFormat::SPI_24);
```

## Methods

### Frame Format

#### `SetFrameFormat()`

Set the SPI frame format at runtime.

**Signature:**
```cpp
void SetFrameFormat(FrameFormat format) noexcept;
```

**Location**: [`src/AS5047U.cpp#L16`](../src/AS5047U.cpp#L16)

**Parameters:**
- `format`: Frame format (SPI_16, SPI_24, or SPI_32)

**Example:**
```cpp
encoder.SetFrameFormat(FrameFormat::SPI_24);
```

---

### Angle Reading

#### `GetAngle()`

Read the 14-bit absolute angle with dynamic compensation (DAEC active).

**Signature:**
```cpp
[[nodiscard]] uint16_t GetAngle(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L33`](../src/AS5047U.cpp#L33)

**Parameters:**
- `retries`: Number of retries on CRC/framing error (default: 0)

**Returns:**
- 14-bit angle value (0-16383), where 0 = 0° and 16383 = 360°

**Example:**
```cpp
uint16_t angle = encoder.GetAngle();
float angle_deg = angle * 360.0f / 16384.0f;
```

**See also:**
- `GetRawAngle()` - Read angle without DAEC

---

#### `GetRawAngle()`

Read the 14-bit absolute angle without dynamic compensation (raw angle).

**Signature:**
```cpp
[[nodiscard]] uint16_t GetRawAngle(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L48`](../src/AS5047U.cpp#L48)

**Parameters:**
- `retries`: Number of retries on CRC/framing error (default: 0)

**Returns:**
- 14-bit raw angle value (0-16383)

**Example:**
```cpp
uint16_t raw_angle = encoder.GetRawAngle();
```

---

### Velocity Reading

#### `GetVelocity()`

Read the current rotational velocity (signed 14-bit).

**Signature:**
```cpp
[[nodiscard]] int16_t GetVelocity(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L63`](../src/AS5047U.cpp#L63)

**Parameters:**
- `retries`: Number of retries on CRC/framing error (default: 0)

**Returns:**
- Signed 14-bit velocity in LSB units

**Example:**
```cpp
int16_t vel = encoder.GetVelocity();
```

**See also:**
- `GetVelocityDegPerSec()` - Velocity in degrees/second
- `GetVelocityRPM()` - Velocity in RPM

---

#### `GetVelocityDegPerSec()`

Get rotational velocity in degrees per second.

**Signature:**
```cpp
[[nodiscard]] float GetVelocityDegPerSec(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L79`](../src/AS5047U.cpp#L79)

**Returns:**
- Velocity in degrees/second

**Example:**
```cpp
float vel_dps = encoder.GetVelocityDegPerSec();
```

---

#### `GetVelocityRPM()`

Get rotational velocity in revolutions per minute.

**Signature:**
```cpp
[[nodiscard]] float GetVelocityRPM(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L89`](../src/AS5047U.cpp#L89)

**Returns:**
- Velocity in RPM

**Example:**
```cpp
float vel_rpm = encoder.GetVelocityRPM();
```

---

### Diagnostics

#### `GetAGC()`

Read the current Automatic Gain Control (AGC) value.

**Signature:**
```cpp
[[nodiscard]] uint8_t GetAGC(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L94`](../src/AS5047U.cpp#L94)

**Returns:**
- AGC value (0-255), where 0 = minimum gain, 255 = maximum gain

**Example:**
```cpp
uint8_t agc = encoder.GetAGC();
if (agc == 0 || agc == 255) {
    // AGC at limit - check magnet placement
}
```

---

#### `GetMagnitude()`

Read the current magnetic field magnitude.

**Signature:**
```cpp
[[nodiscard]] uint16_t GetMagnitude(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L109`](../src/AS5047U.cpp#L109)

**Returns:**
- Magnetic field magnitude (0-16383)

**Example:**
```cpp
uint16_t mag = encoder.GetMagnitude();
```

---

#### `GetErrorFlags()`

Read and clear error/status flags.

**Signature:**
```cpp
[[nodiscard]] uint16_t GetErrorFlags(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L124`](../src/AS5047U.cpp#L124)

**Returns:**
- 16-bit error flag register. All flags clear after read.

**Error Flags:**
- `AS5047U_Error::AgcWarning` - AGC at limit
- `AS5047U_Error::MagHalf` - Magnetic field half of regulated value
- `AS5047U_Error::CrcError` - CRC error during SPI communication
- `AS5047U_Error::FramingError` - SPI framing error
- See `AS5047U_Error` enum for all flags

**Example:**
```cpp
uint16_t errors = encoder.GetErrorFlags();
if (errors & static_cast<uint16_t>(AS5047U_Error::CrcError)) {
    // Handle CRC error
}
```

---

#### `GetStickyErrorFlags()`

Read sticky error flags (not cleared on read).

**Signature:**
```cpp
[[nodiscard]] AS5047U_Error GetStickyErrorFlags() const;
```

**Location**: [`inc/as5047u.hpp#L[line]`](../inc/as5047u.hpp)

**Returns:**
- `AS5047U_Error` enum with sticky error flags

**Example:**
```cpp
AS5047U_Error errors = encoder.GetStickyErrorFlags();
```

---

#### `DumpStatus()`

Print formatted status and diagnostics using printf.

**Signature:**
```cpp
void DumpStatus() const;
```

**Location**: [`src/AS5047U.cpp#L597`](../src/AS5047U.cpp#L597)

**Example:**
```cpp
encoder.DumpStatus();
```

---

### Configuration

#### `SetZeroPosition()`

Set a new zero reference position (soft offset).

**Signature:**
```cpp
bool SetZeroPosition(uint16_t angle_lsb, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L162`](../src/AS5047U.cpp#L162)

**Parameters:**
- `angle_lsb`: 14-bit angle value that should be treated as 0°
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

**Example:**
```cpp
uint16_t current_angle = encoder.GetAngle();
encoder.SetZeroPosition(current_angle);
```

---

#### `GetZeroPosition()`

Get the currently configured zero position offset.

**Signature:**
```cpp
[[nodiscard]] uint16_t GetZeroPosition(uint8_t retries = AS5047U_CFG::CRC_RETRIES) const;
```

**Location**: [`src/AS5047U.cpp#L135`](../src/AS5047U.cpp#L135)

**Returns:**
- Current zero position in LSB (0-16383)

---

#### `SetDirection()`

Define the rotation direction for increasing angle.

**Signature:**
```cpp
bool SetDirection(bool clockwise, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`inc/as5047u.hpp#L214`](../inc/as5047u.hpp#L214)

**Parameters:**
- `clockwise`: If true, clockwise rotation yields increasing angle
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

---

#### `SetABIResolution()`

Set the ABI (incremental encoder) resolution.

**Signature:**
```cpp
bool SetABIResolution(uint8_t resolution_bits, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L171`](../src/AS5047U.cpp#L171)

**Parameters:**
- `resolution_bits`: Resolution in bits (10-14 bits)
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

**Example:**
```cpp
encoder.SetABIResolution(12);  // 12-bit = 4096 PPR
```

---

#### `SetUVWPolePairs()`

Set the number of pole pairs for UVW commutation outputs.

**Signature:**
```cpp
bool SetUVWPolePairs(uint8_t pairs, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L179`](../src/AS5047U.cpp#L179)

**Parameters:**
- `pairs`: Number of pole pairs (1-7)
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

---

#### `ConfigureInterface()`

Configure interface outputs (ABI, UVW) and PWM output.

**Signature:**
```cpp
bool ConfigureInterface(bool abi, bool uvw, bool pwm, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L205`](../src/AS5047U.cpp#L205)

**Parameters:**
- `abi`: Enable ABI (A, B, I) outputs
- `uvw`: Enable UVW commutation outputs
- `pwm`: Enable PWM output
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

**Example:**
```cpp
encoder.ConfigureInterface(true, false, false);  // ABI only
```

---

#### `SetDynamicAngleCompensation()`

Enable/disable Dynamic Angle Error Compensation (DAEC).

**Signature:**
```cpp
bool SetDynamicAngleCompensation(bool enable, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L225`](../src/AS5047U.cpp#L225)

**Parameters:**
- `enable`: Enable DAEC if true
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

---

#### `SetAdaptiveFilter()`

Enable/disable the adaptive filter (Dynamic Filter System).

**Signature:**
```cpp
bool SetAdaptiveFilter(bool enable, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L232`](../src/AS5047U.cpp#L232)

**Parameters:**
- `enable`: Enable adaptive filter if true
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

---

#### `SetFilterParameters()`

Set adaptive filter parameters (K_min and K_max).

**Signature:**
```cpp
bool SetFilterParameters(uint8_t k_min, uint8_t k_max, uint8_t retries = AS5047U_CFG::CRC_RETRIES);
```

**Location**: [`src/AS5047U.cpp#L239`](../src/AS5047U.cpp#L239)

**Parameters:**
- `k_min`: Minimum filter strength (0-15)
- `k_max`: Maximum filter strength (0-15)
- `retries`: Number of retries on CRC/framing error

**Returns:**
- `true` if register write succeeded

---

#### `ProgramOTP()`

Permanently program current settings into OTP memory.

**Signature:**
```cpp
bool ProgramOTP();
```

**Location**: [`src/AS5047U.cpp#L257`](../src/AS5047U.cpp#L257)

**Returns:**
- `true` if programming and verification succeeded

**Warning**: OTP can be programmed only once. Ensure proper supply voltage and desired configuration before use.

**Example:**
```cpp
// Configure all settings first
encoder.SetZeroPosition(0);
encoder.SetDirection(true);
// ... other settings

// Program to OTP
bool success = encoder.ProgramOTP();
```

---

## Types

### `FrameFormat`

SPI frame format enumeration.

```cpp
enum class FrameFormat : uint8_t {
    SPI_16,  // 16-bit frames (no CRC)
    SPI_24,  // 24-bit frames (with CRC)
    SPI_32   // 32-bit frames (with CRC + pad)
};
```

### `AS5047U_Error`

Error flag enumeration.

```cpp
enum class AS5047U_Error : uint16_t {
    None = 0,
    AgcWarning = 1 << 0,
    MagHalf = 1 << 1,
    CrcError = 1 << 6,
    FramingError = 1 << 4,
    // ... other flags
};
```

## Error Handling

The driver uses return values and error flags for error handling:

- **Configuration methods** return `bool` (true = success, false = failure)
- **Reading methods** return values directly; check error flags separately
- **CRC errors** can be retried by passing `retries` parameter
- **Error flags** are available via `GetErrorFlags()` or `GetStickyErrorFlags()`

### Error Codes

| Code | Name | Description |
|------|------|-------------|
| `AgcWarning` | AGC Warning | AGC reached minimum (0) or maximum (255) value |
| `MagHalf` | Magnitude Half | Magnetic field is half of regulated value |
| `CrcError` | CRC Error | CRC error during SPI communication |
| `FramingError` | Framing Error | SPI framing error |
| `CommandError` | Command Error | Invalid SPI command received |

---

**Navigation**
⬅️ [Configuration](configuration.md) | [Next: Examples ➡️](examples.md) | [Back to Index](index.md)

