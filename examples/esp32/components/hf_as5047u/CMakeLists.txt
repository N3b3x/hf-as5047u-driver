# =============================================================================
# AS5047U Driver Component - CMakeLists.txt
# =============================================================================
# This component provides the AS5047U driver library
# =============================================================================

cmake_minimum_required(VERSION 3.16)

# =============================================================================
# Path Configuration
# =============================================================================
# Set the source root path relative to this component directory
set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src")
set(INC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../inc")

message(STATUS "AS5047U Driver - ESP32 Component Configuration")
message(STATUS "Source Root: ${SRC_ROOT}")

# =============================================================================
# Source File Discovery and Validation
# =============================================================================
# Driver is header-only: implementation is in src/as5047u.ipp (included by inc/as5047u.hpp).
# No separate .cpp to compile; template is instantiated by the main application.
set(DRIVER_SRCS "")

# Optional: if a non-header implementation file is added later, discover it here
if(EXISTS "${SRC_ROOT}/as5047u.cpp")
    list(APPEND DRIVER_SRCS "${SRC_ROOT}/as5047u.cpp")
    message(STATUS "Found driver source: ${SRC_ROOT}/as5047u.cpp")
endif()
set(EXISTING_SRCS ${DRIVER_SRCS})

# =============================================================================
# Include Directories Configuration
# =============================================================================
# Define include directories for the driver component
set(DRIVER_INCLUDE_DIRS
    "${INC_ROOT}"                       # Main driver headers
)

# =============================================================================
# Component Dependencies
# =============================================================================
# Define required ESP-IDF components for the driver
set(DRIVER_REQUIRES
    driver              # ESP-IDF driver framework
    freertos            # FreeRTOS support
)

# =============================================================================
# Component Registration
# =============================================================================
# Register the driver component with ESP-IDF build system
idf_component_register(
    SRCS ${EXISTING_SRCS}                 # Source files (validated above)
    INCLUDE_DIRS ${DRIVER_INCLUDE_DIRS}   # Include directories defined above
    REQUIRES ${DRIVER_REQUIRES}           # Component dependencies defined above
)

# =============================================================================
# Compiler Configuration
# =============================================================================
# Header-only component becomes an INTERFACE library when SRCS is empty.
# Use INTERFACE so that targets linking this component get C++17.
target_compile_features(${COMPONENT_LIB} INTERFACE cxx_std_17)
