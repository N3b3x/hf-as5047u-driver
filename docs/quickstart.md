---
layout: default
title: "⚡ Quick Start"
description: "Get up and running with the AS5047U driver in minutes"
nav_order: 2
parent: "📚 Documentation"
permalink: /docs/quickstart/
---

# Quick Start

This guide will get you up and running with the AS5047U driver in just a few steps.

## Prerequisites

- [Driver installed](installation.md)
- [Hardware wired](hardware_setup.md)
- [SPI interface implemented](platform_integration.md)

## Minimal Example

Here's a complete working example:

```cpp
#include "as5047u.hpp"

// 1. Implement the SPI interface
class MySpi : public as5047u::SpiInterface<MySpi> {
public:
    void transfer(const uint8_t* tx, uint8_t* rx, size_t len) {
        // Your SPI transfer implementation
        // Assert CS, transfer data, deassert CS
    }
};

// 2. Create instances
MySpi spi;
as5047u::AS5047U encoder(spi, FrameFormat::SPI_24); // 24-bit frames with CRC

// 3. Read angle
uint16_t angle = encoder.GetAngle();             // compensated angle (0-16383)
float angle_deg = angle * 360.0f / 16384.0f;    // convert to degrees

// 4. Read velocity
int16_t vel = encoder.GetVelocity();             // velocity in LSB
float vel_dps = encoder.GetVelocityDegPerSec();  // velocity in deg/s

// 5. Diagnostics
uint8_t agc = encoder.GetAGC();                  // AGC value (0-255)
uint16_t mag = encoder.GetMagnitude();           // magnetic magnitude
uint16_t errors = encoder.GetErrorFlags();       // error flags
```

## Step-by-Step Explanation

### Step 1: Include the Header

```cpp
#include "as5047u.hpp"
```

This includes the main driver class and all necessary types.

### Step 2: Implement the SPI Interface

You need to implement the `SpiInterface` for your platform. See [Platform Integration](platform_integration.md) for detailed examples.

```cpp
class MySpi : public as5047u::SpiInterface<MySpi> {
public:
    void transfer(const uint8_t* tx, uint8_t* rx, size_t len) {
        // Assert chip select
        // Perform SPI transfer
        // Deassert chip select
    }
};
```

### Step 3: Create Driver Instance

```cpp
MySpi spi;
as5047u::AS5047U encoder(spi, FrameFormat::SPI_24);
```

The constructor takes:
- A reference to your SPI interface implementation
- The SPI frame format: `SPI_16`, `SPI_24` (with CRC), or `SPI_32` (with CRC)

### Step 4: Read Angle

```cpp
uint16_t angle = encoder.GetAngle();  // 14-bit angle (0-16383)
```

The angle is returned as a 14-bit value (0-16383), where 0 = 0° and 16383 = 360°.

### Step 5: Convert to Degrees

```cpp
float angle_deg = angle * 360.0f / 16384.0f;
```

### Step 6: Read Velocity

```cpp
int16_t vel = encoder.GetVelocity();              // LSB units
float vel_dps = encoder.GetVelocityDegPerSec();   // degrees/second
float vel_rpm = encoder.GetVelocityRPM();         // RPM
```

### Step 7: Check Diagnostics

```cpp
uint8_t agc = encoder.GetAGC();        // Automatic Gain Control (0-255)
uint16_t mag = encoder.GetMagnitude(); // Magnetic field magnitude
uint16_t errors = encoder.GetErrorFlags(); // Error flags
```

## Complete Example with Error Handling

```cpp
#include "as5047u.hpp"

class MySpi : public as5047u::SpiInterface<MySpi> {
    // ... SPI implementation
};

void app_main() {
    MySpi spi;
    as5047u::AS5047U encoder(spi, FrameFormat::SPI_24);
    
    while (true) {
        // Read angle with retry on CRC errors
        uint16_t angle = encoder.GetAngle(3); // 3 retries
        
        // Check for errors
        AS5047U_Error errors = encoder.GetStickyErrorFlags();
        if (errors != AS5047U_Error::None) {
            printf("Error detected: 0x%04X\n", static_cast<uint16_t>(errors));
        }
        
        // Read diagnostics
        uint8_t agc = encoder.GetAGC();
        uint16_t mag = encoder.GetMagnitude();
        
        printf("Angle: %u (%.2f°), AGC: %u, Mag: %u\n", 
               angle, angle * 360.0f / 16384.0f, agc, mag);
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

## Expected Output

When running this example, you should see:

```
Angle: 8192 (180.00°), AGC: 128, Mag: 8192
Angle: 8200 (180.18°), AGC: 128, Mag: 8192
Angle: 8208 (180.35°), AGC: 128, Mag: 8192
```

## Troubleshooting

If you encounter issues:

- **Compilation errors**: Check that you've implemented the `transfer()` method in your SPI interface
- **CRC errors**: Verify SPI connections and try increasing retry count
- **Zero readings**: Check magnet placement and field strength
- **See**: [Troubleshooting](troubleshooting.md) for common issues

## Next Steps

- Explore [Examples](examples.md) for more advanced usage
- Review the [API Reference](api_reference.md) for all available methods
- Check [Configuration](configuration.md) for customization options

---

**Navigation**
⬅️ [Installation](installation.md) | [Next: Hardware Setup ➡️](hardware_setup.md) | [Back to Index](index.md)

