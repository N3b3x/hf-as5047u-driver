# Configuration

This guide covers all configuration options available for the AS5047U driver.

## Compile-Time Configuration

### Kconfig Options

If your project uses Kconfig (e.g., ESP-IDF), the following options are available:

```
AS5047U Configuration
    AS5047U_FRAME_FORMAT (16/24/32-bit)
    AS5047U_CRC_RETRIES (default: 0)
```

### Configuration Header

You can also configure the driver by editing `inc/as5047u_config.hpp`:

```cpp
namespace AS5047U_CFG {
    // Default SPI frame format
    inline constexpr FrameFormat DEFAULT_FRAME_FORMAT = FrameFormat::SPI_24;
    
    // Number of CRC retries (0 = no retry)
    inline constexpr uint8_t CRC_RETRIES = 3;
}
```

## Runtime Configuration

### SPI Frame Format

The driver supports three SPI frame formats:

```cpp
// 16-bit frames (no CRC, highest throughput)
as5047u::AS5047U encoder(spi, FrameFormat::SPI_16);

// 24-bit frames (includes 8-bit CRC, recommended)
as5047u::AS5047U encoder(spi, FrameFormat::SPI_24);

// 32-bit frames (includes 8-bit CRC + 8-bit pad for daisy-chain)
as5047u::AS5047U encoder(spi, FrameFormat::SPI_32);
```

**Recommendation**: Use `SPI_24` for most applications - it provides CRC protection with good throughput.

### Change Frame Format at Runtime

```cpp
encoder.SetFrameFormat(FrameFormat::SPI_24);
```

## Sensor Configuration

### Zero Position

Set the zero reference position (soft offset):

```cpp
// Set zero position to current angle
uint16_t current_angle = encoder.GetAngle();
encoder.SetZeroPosition(current_angle);

// Or set a specific value (0-16383)
encoder.SetZeroPosition(8192);  // 180 degrees
```

### Rotation Direction

```cpp
// Clockwise rotation yields increasing angle (default)
encoder.SetDirection(true);

// Counter-clockwise rotation yields increasing angle
encoder.SetDirection(false);
```

### Dynamic Angle Error Compensation (DAEC)

DAEC provides low-latency angle correction for high-speed applications:

```cpp
// Enable DAEC (recommended for high-speed applications)
encoder.SetDynamicAngleCompensation(true);

// Disable DAEC (use raw angle)
encoder.SetDynamicAngleCompensation(false);
```

### Adaptive Filter (DFS™)

The Dynamic Filter System adaptively filters noise:

```cpp
// Enable adaptive filter
encoder.SetAdaptiveFilter(true);

// Set filter parameters (K_min, K_max)
// K_min: minimum filter strength (0-15)
// K_max: maximum filter strength (0-15)
encoder.SetFilterParameters(2, 3);
```

### Angle Output Source

Select which angle is used for outputs (ABI, UVW, PWM):

```cpp
// Use compensated angle (with DAEC)
encoder.SetAngleOutputSource(AS5047U_REG::SETTINGS2::AngleOutputSource::Compensated);

// Use raw angle (without DAEC)
encoder.SetAngleOutputSource(AS5047U_REG::SETTINGS2::AngleOutputSource::Raw);
```

## Output Configuration

### ABI (Incremental Encoder) Output

Configure incremental encoder outputs (A, B, I):

```cpp
// Set resolution (10-14 bits)
encoder.SetABIResolution(12);  // 12-bit = 4096 PPR

// Set index pulse length (1 or 3 LSB periods)
encoder.SetIndexPulseLength(1);

// Enable ABI output
encoder.ConfigureInterface(true, false, false);  // ABI only
```

### UVW (Commutation) Output

Configure 3-phase commutation outputs:

```cpp
// Set number of pole pairs (1-7)
encoder.SetUVWPolePairs(5);  // 5 pole pairs

// Enable UVW output
encoder.ConfigureInterface(false, true, false);  // UVW only
```

### PWM Output

Configure PWM-encoded angle output:

```cpp
// Enable PWM output
// PWM appears on W pin if ABI enabled, or I pin if UVW enabled
encoder.ConfigureInterface(true, false, true);  // ABI + PWM
```

### Combined Outputs

```cpp
// Enable both ABI and UVW (PWM not available in this mode)
encoder.ConfigureInterface(true, true, false);

// Enable ABI with PWM on W pin
encoder.ConfigureInterface(true, false, true);
```

## Advanced Configuration

### Hysteresis

Set incremental output hysteresis to reduce jitter:

```cpp
// Set hysteresis level
encoder.SetHysteresis(AS5047U_REG::SETTINGS3::Hysteresis::LSB_1);
```

### High-Temperature Mode

Enable 150°C operation mode:

```cpp
encoder.Set150CTemperatureMode(true);
```

### OTP Programming

Program current settings into OTP (One-Time Programmable) memory for permanent configuration:

```cpp
// Configure all settings first
encoder.SetZeroPosition(0);
encoder.SetDirection(true);
encoder.SetABIResolution(12);
// ... other settings

// Program to OTP (one-time operation, cannot be undone)
bool success = encoder.ProgramOTP();
if (success) {
    // Settings are now permanent
}
```

**Warning**: OTP programming is **irreversible**. Make sure all settings are correct before programming.

## CRC Retry Configuration

Configure automatic retry on CRC errors:

```cpp
// Read with automatic retry (3 attempts)
uint16_t angle = encoder.GetAngle(3);

// Or set default retries in config
// AS5047U_CFG::CRC_RETRIES = 3;
```

## Default Values

| Option | Default | Description |
|--------|---------|-------------|
| `DEFAULT_FRAME_FORMAT` | `SPI_16` | SPI frame format |
| `CRC_RETRIES` | `0` | Number of CRC retries |
| Zero Position | `0` | Zero reference angle |
| Direction | `true` (CW) | Rotation direction |
| DAEC | `enabled` | Dynamic angle compensation |
| Adaptive Filter | `enabled` | DFS adaptive filtering |
| ABI Resolution | `14-bit` | Incremental output resolution |
| UVW Pole Pairs | `1` | Commutation pole pairs |

## Recommended Settings

### For High-Speed Motor Control

```cpp
encoder.SetDynamicAngleCompensation(true);  // Enable DAEC
encoder.SetAdaptiveFilter(true);            // Enable DFS
encoder.SetFilterParameters(2, 3);         // Moderate filtering
encoder.SetABIResolution(12);              // 4096 PPR
encoder.ConfigureInterface(true, false, false);  // ABI output
```

### For Low-Noise Applications

```cpp
encoder.SetAdaptiveFilter(true);
encoder.SetFilterParameters(3, 5);          // Stronger filtering
encoder.SetHysteresis(AS5047U_REG::SETTINGS3::Hysteresis::LSB_2);
```

### For Basic Angle Reading

```cpp
// Use default settings, just read angle
uint16_t angle = encoder.GetAngle();
```

## Next Steps

- See [Examples](examples.md) for configuration examples
- Review [API Reference](api_reference.md) for all configuration methods

---

**Navigation**
⬅️ [Platform Integration](platform_integration.md) | [Next: API Reference ➡️](api_reference.md) | [Back to Index](index.md)

