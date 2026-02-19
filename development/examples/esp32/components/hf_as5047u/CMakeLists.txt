###############################################################################
# HF AS5047U Driver - ESP-IDF Component Wrapper
###############################################################################
# Purpose:
#   Adapt the generic, hardware-agnostic driver target contract to ESP-IDF's
#   component model using idf_component_register().
#
# Key Rule:
#   Do not duplicate source/include/dependency definitions here.
#   Always consume the shared build settings from driver root.
#
# Note:
#   cmake_minimum_required() is intentionally omitted here — ESP-IDF's
#   project.cmake already sets it and adding another can cause policy conflicts.
###############################################################################

# =============================================================================
# Shared Build Contract Import
# =============================================================================
set(HF_AS5047U_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../.." CACHE PATH
    "Path to root of hf-as5047u-driver (auto-detected from component location)")

# Normalise and validate the resolved path
get_filename_component(HF_AS5047U_ROOT "${HF_AS5047U_ROOT}" ABSOLUTE)
if(NOT EXISTS "${HF_AS5047U_ROOT}/cmake/hf_as5047u_build_settings.cmake")
    message(FATAL_ERROR
        "Cannot find AS5047U driver root.\n"
        "  Resolved path: ${HF_AS5047U_ROOT}\n"
        "  Expected file: ${HF_AS5047U_ROOT}/cmake/hf_as5047u_build_settings.cmake\n"
        "  Fix: set HF_AS5047U_ROOT to the correct driver checkout path.")
endif()

include("${HF_AS5047U_ROOT}/cmake/hf_as5047u_build_settings.cmake")

# =============================================================================
# ESP-IDF Component Registration
# =============================================================================
idf_component_register(
    SRCS ${HF_AS5047U_SOURCE_FILES}
    INCLUDE_DIRS ${HF_AS5047U_PUBLIC_INCLUDE_DIRS}
    REQUIRES ${HF_AS5047U_IDF_REQUIRES}
)

# =============================================================================
# C++ Standard Contract
# =============================================================================
if(HF_AS5047U_SOURCE_FILES)
    target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_20)
else()
    target_compile_features(${COMPONENT_LIB} INTERFACE cxx_std_20)
endif()
