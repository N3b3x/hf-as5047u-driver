# AS5047U ESP32-C6 Examples

This directory contains comprehensive examples demonstrating the AS5047U driver on ESP32-C6 platform.

## 🎯 Overview

The ESP32-C6 examples showcase real-world usage of the AS5047U magnetic encoder with:

- **Hardware-specific HAL** implementation for ESP32-C6
- **Multiple example applications** covering different use cases
- **Automated build system** with configurable app types
- **Comprehensive documentation** for each example
- **Production-ready code** with proper error handling

## 🔧 Hardware Requirements

### ESP32-C6 Development Board
- ESP32-C6-DevKitC-1 or compatible
- USB-C cable for programming and power

### AS5047U Connections

| AS5047U Pin | ESP32-C6 GPIO | Function |
|-------------|---------------|----------|
| MISO | GPIO2 | SPI Data In |
| MOSI | GPIO7 | SPI Data Out |
| SCLK | GPIO6 | SPI Clock |
| CS | GPIO10 | Chip Select |
| VDD | 3.3V | Logic Supply |
| GND | GND | Ground |

**Note:** The default SPI configuration uses mode 1 (CPOL=0, CPHA=1) at 4MHz.

## 🚀 Quick Start

### 1. Prerequisites

```bash
# Install ESP-IDF v5.5
curl -fsSL https://raw.githubusercontent.com/espressif/esp-idf/master/tools/install.sh | bash
source ~/esp/esp-idf/export.sh

# Verify installation
idf.py --version
```

### 2. Setup Repository

```bash
# Clone and setup
git clone --recursive https://github.com/n3b3x/hf-as5047u-driver.git
cd hf-as5047u-driver/examples/esp32

# Initialize build environment
./scripts/setup_repo.sh
```

### 3. Build and Flash

```bash
# Build driver integration test (default)
./scripts/build_app.sh driver_integration_test Release

# Flash to ESP32-C6
./scripts/flash_app.sh driver_integration_test Release

# Monitor output
idf.py monitor
```

## 📱 Available Examples

### 🟢 Test Suite

#### `driver_integration_test`
**Comprehensive Driver Integration Test Suite**
- Complete driver API validation (30+ tests)
- Multiple test sections covering all functionality
- Initialization, angle/velocity reading, diagnostics
- Frame format testing (16/24/32-bit)
- Configuration and error handling
- FreeRTOS task-based test execution
- GPIO14 progress indicator
- Automatic pass/fail tracking
- No actual hardware required (driver test only)

**Build:**
```bash
./scripts/build_app.sh driver_integration_test Release
```

### 🟡 Basic Examples

#### `basic_angle_reading`
**Basic Angle Reading Example**
- Simple angle reading loop
- Angle in LSB and degrees
- AGC and magnitude diagnostics
- Error flag monitoring
- Real hardware testing

**Build:**
```bash
./scripts/build_app.sh basic_angle_reading Release
```

#### `velocity_reading`
**Velocity Reading Example**
- Velocity reading in multiple units
- Degrees per second, radians per second, RPM
- Real hardware testing

**Build:**
```bash
./scripts/build_app.sh velocity_reading Release
```

#### `configuration_example`
**Configuration Example**
- ABI output configuration
- UVW commutation setup
- PWM output configuration
- Zero position and direction setting
- Real hardware testing

**Build:**
```bash
./scripts/build_app.sh configuration_example Release
```

#### `full_features`
**Comprehensive Example**
- All sensor features enabled
- Angle, velocity, diagnostics
- ABI, UVW, PWM configuration
- DAEC and filter settings
- OTP programming
- Real hardware testing

**Build:**
```bash
./scripts/build_app.sh full_features Release
```

## 🔨 Building Examples

### Using Build Scripts (Recommended)

```bash
# List available app types
./scripts/build_app.sh list

# Build specific app
./scripts/build_app.sh <app_type> <build_type>

# Examples:
./scripts/build_app.sh driver_integration_test Release
./scripts/build_app.sh basic_angle_reading Debug
```

### Using ESP-IDF Directly

```bash
# Set app type and build type
idf.py build -DAPP_TYPE=driver_integration_test -DBUILD_TYPE=Release

# Flash
idf.py flash

# Monitor
idf.py monitor
```

## 📊 Test Framework

All test examples use a standardized test framework with:

- **Test result tracking** - Automatic pass/fail counting
- **Execution timing** - Performance measurement
- **FreeRTOS task execution** - Isolated test environments
- **GPIO14 progress indicator** - Visual test progression
- **Standardized logging** - Consistent output format

### Test Framework Features

- `RUN_TEST(test_func)` - Run test inline
- `RUN_TEST_IN_TASK(name, func, stack_size, priority)` - Run test in FreeRTOS task
- `RUN_TEST_SECTION_IF_ENABLED(enabled, section_name, ...)` - Conditional test sections
- `g_test_results` - Global test results tracking

## 🔌 Hardware Configuration

### SPI Configuration

The default SPI configuration can be modified in the example files:

```cpp
Esp32As5047uBus::SPIConfig config;
config.miso_pin = GPIO_NUM_2;      // MISO pin
config.mosi_pin = GPIO_NUM_7;      // MOSI pin
config.sclk_pin = GPIO_NUM_6;      // SCLK pin
config.cs_pin = GPIO_NUM_10;       // CS pin
config.frequency = 4000000;        // SPI frequency (4MHz)
config.mode = 1;                   // SPI mode (1: CPOL=0, CPHA=1)
```

### Pin Configuration

| Function | Default GPIO | Notes |
|----------|-------------|-------|
| SPI MISO | GPIO2 | Can be changed in code |
| SPI MOSI | GPIO7 | Can be changed in code |
| SPI SCLK | GPIO6 | Can be changed in code |
| SPI CS | GPIO10 | Can be changed in code |
| Test Progress | GPIO14 | Used by test framework |

### Frame Formats

The AS5047U supports three SPI frame formats:

- **SPI_16** - 16-bit frames (no CRC, high-throughput mode)
- **SPI_24** - 24-bit frames (includes 8-bit CRC for reliability) - **Recommended**
- **SPI_32** - 32-bit frames (includes 8-bit CRC and 8-bit pad for daisy-chain)

## 📚 Documentation

For more detailed documentation, see:

- [Main Driver README](../../README.md)
- [Setup Guide](../../docs/setup.md)
- [Usage Guide](../../docs/usage.md)
- [Examples Guide](../../docs/examples.md)
- [Building Guide](../../docs/building.md)

## 🐛 Troubleshooting

### Build Issues

**Error: "APP_TYPE not defined"**
- Use the build scripts: `./scripts/build_app.sh <app_type> <build_type>`
- Or set manually: `idf.py build -DAPP_TYPE=driver_integration_test -DBUILD_TYPE=Release`

**Error: "Failed to read valid app types from app_config.yml"**
- Ensure the scripts submodule is initialized: `git submodule update --init --recursive`
- Check that `scripts/get_app_info.py` exists

### Runtime Issues

**SPI Communication Errors**
- Check wiring connections (MISO, MOSI, SCLK, CS, GND, VDD)
- Verify SPI mode (default is mode 1: CPOL=0, CPHA=1)
- Check SPI frequency (default is 4MHz)
- Verify chip select is properly configured

**No Angle Data**
- Ensure encoder is powered (3.3V)
- Check magnetic field is present (magnitude should be > 0)
- Verify SPI communication (check error flags)
- Ensure proper SPI frame format (24-bit recommended)

**CRC Errors**
- Use 24-bit or 32-bit frame format (includes CRC)
- Check SPI signal integrity
- Verify SPI timing and frequency

**Initialization Failures**
- Check SPI bus initialization
- Verify chip select pin configuration
- Ensure proper power supply (3.3V)
- Check for SPI bus conflicts

## 📝 License

See the main driver [LICENSE](../../LICENSE) file.

## 🤝 Contributing

See the main driver [Contributing Guide](../../docs/README.md).

---

**For questions or issues, please open an issue on GitHub.**

