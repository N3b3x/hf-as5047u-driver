# Installation

This guide covers how to obtain and integrate the AS5047U driver library into your project.

## Prerequisites

Before installing the driver, ensure you have:

- **C++20 Compiler**: GCC 10+, Clang 12+, or MSVC 2019+
- **Build System**: Make, CMake, or ESP-IDF (depending on your platform)
- **Platform SDK**: ESP-IDF, STM32 HAL, Arduino, or your platform's SPI driver

## Obtaining the Source

### Option 1: Git Clone

```bash
git clone https://github.com/n3b3x/hf-as5047u-driver.git
cd hf-as5047u-driver
```

### Option 2: Copy Files

Copy the following files into your project:

```
inc/
  ├── as5047u.hpp
  ├── as5047u_spi_interface.hpp
  ├── AS5047U_REGISTERS.hpp
  ├── AS5047U_types.hpp
  └── AS5047U_config.hpp
src/
  └── AS5047U.cpp
```

## Integration Methods

### Using CMake

Add the driver as a subdirectory in your `CMakeLists.txt`:

```cmake
add_subdirectory(external/hf-as5047u-driver)
target_link_libraries(your_target PRIVATE hf_as5047u)
target_include_directories(your_target PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/hf-as5047u-driver/inc
)
```

### Using ESP-IDF Component

The driver can be used as an ESP-IDF component. Add it to your `components` directory:

```cmake
# In your main CMakeLists.txt
idf_component_register(
    SRCS "your_code.cpp"
    INCLUDE_DIRS "."
    REQUIRES hf_as5047u
)
```

### Manual Integration

1. Copy the driver files to your project
2. Add the `inc/` directory to your include path
3. Include the header:
   ```cpp
   #include "as5047u.hpp"
   ```
4. Compile with C++20 support:
   ```bash
   g++ -std=c++20 -I inc/ your_code.cpp src/AS5047U.cpp
   ```

## Running Unit Tests

The library includes unit tests. To run them:

```bash
cd tests
g++ -std=c++20 -I ../inc ../src/AS5047U.cpp test_as5047u.cpp -o test
./test
```

Expected output:
```
All tests passed.
```

## Verification

To verify the installation:

1. Include the header in a test file:
   ```cpp
   #include "as5047u.hpp"
   ```

2. Compile a simple test:
   ```bash
   g++ -std=c++20 -I inc/ -c src/AS5047U.cpp -o test.o
   ```

3. If compilation succeeds, the library is properly installed.

## Next Steps

- Follow the [Quick Start](quickstart.md) guide to create your first application
- Review [Hardware Setup](hardware_setup.md) for wiring instructions
- Check [Platform Integration](platform_integration.md) to implement the SPI interface

---

**Navigation**
⬅️ [Back to Index](index.md) | [Next: Quick Start ➡️](quickstart.md)

