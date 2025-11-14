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
# List all potential driver source files
set(DRIVER_SRCS
    "${SRC_ROOT}/AS5047U.cpp"
)

# Check which source files actually exist and add them
set(EXISTING_SRCS "")
foreach(src_file ${DRIVER_SRCS})
    if(EXISTS "${src_file}")
        list(APPEND EXISTING_SRCS "${src_file}")
        message(STATUS "Found source: ${src_file}")
    else()
        message(WARNING "Source file not found: ${src_file}")
    endif()
endforeach()

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
# Set C++ standard (use INTERFACE since this may be an INTERFACE library)
target_compile_features(${COMPONENT_LIB} INTERFACE cxx_std_17)

# Note: Compiler flags are typically set by the consuming component,
#       but we can set INTERFACE properties for INTERFACE libraries
#       For regular libraries, these would be PRIVATE, but since we have
#       no source files, ESP-IDF creates an INTERFACE library
