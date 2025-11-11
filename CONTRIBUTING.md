---
layout: default
title: "🤝 Contributing"
description: "Guidelines and information for contributing to the HardFOC AS5047U Driver"
nav_order: 5
parent: "🔧 HardFOC AS5047U Driver"
permalink: /CONTRIBUTING/
---

# 🤝 Contributing to HardFOC AS5047U Driver

Thank you for your interest in contributing to the HardFOC AS5047U Driver!
This document provides guidelines and information for contributors.

## 📋 **Code Standards**

### 🎯 **Coding Style and Best Practices**

**All contributors must follow the official HardFOC Coding Standards:**

📚 **[HardFOC Embedded Systems Coding Standards](https://github.com/hardfoc/org-discussions)**

The coding standards document provides comprehensive guidelines for:
- **Naming Conventions**: Functions, variables, constants, classes, and more
- **Code Organization**: File structure, include order, class organization
- **Error Handling**: Patterns for embedded systems
- **Memory Management**: Best practices for resource-constrained systems
- **Thread Safety**: Guidelines for multi-threaded code
- **Documentation Standards**: Doxygen and inline comments
- **Embedded Systems Best Practices**: Fixed-width types, volatile usage, and more

### 🎯 **Quick Reference for AS5047U Driver**

- **C++20 Standard Compliance** - All code must be compatible with C++20
- **Consistent Naming** - Follow the established naming conventions:
  - Classes: `PascalCase` (e.g., `AS5047U`, `spiBus`)
  - Public Functions: `PascalCase` (e.g., `GetAngle`, `SetZeroPosition`)
  - Private Functions: `camelCase` (e.g., `checkInitialized`, `transferFrame`)
  - Member Variables: `snake_case` with trailing underscore (e.g., `bus_`, `frame_format_`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_ANGLE_VALUE`, `DEFAULT_FRAME_FORMAT`)
  - Local Variables: `snake_case` (e.g., `current_angle`, `error_flags`)
  - Parameters: `snake_case` (e.g., `channel`, `angle_value`)

### 🏗️ **Architecture Guidelines**

- **Hardware Abstraction** - Use hardware-agnostic `spiBus` interface
- **Error Handling** - All functions use appropriate error handling patterns
- **Safety** - Use `noexcept` where appropriate for safety-critical code
- **Dependencies** - Keep dependencies minimal (freestanding where possible)
- **Frame Formats** - Support 16-bit, 24-bit, and 32-bit SPI frame formats

## 🧪 **Testing**

### 🔧 **Unit Tests and Hardware Validation Requirements**

- **Unit Tests** - Write comprehensive unit tests for all new functionality
- **Hardware Testing** - Test on actual AS5047U hardware with ESP32
- **Integration Tests** - Verify compatibility with existing HardFOC systems
- **Performance Tests** - Ensure real-time performance requirements are met
- **Safety Tests** - Validate safety features and error handling
- **Frame Format Tests** - Test all supported SPI frame formats (16/24/32-bit)

## 📖 **Documentation**

### 📚 **Documentation Standards and Updates**

- **API Documentation** - Update documentation for all public interfaces
- **User Guides** - Create or update guides for new features
- **Example Code** - Provide working examples for encoder applications
- **Architecture Documentation** - Document design decisions and patterns
- **Doxygen Comments** - All public APIs must have Doxygen documentation

## 🐛 **Bug Reports**

### 🔍 **How to Report Bugs Effectively**

When reporting bugs, please include:

1. **Hardware Information**: AS5047U board, ESP32 version, encoder configuration
2. **Environment Details**: ESP-IDF version, compiler version, operating system
3. **Reproduction Steps**: Minimal code example, configuration settings
4. **Hardware Configuration**: Connected peripherals, pin assignments, SPI settings
5. **Debugging Information**: Error messages, log output, stack traces
6. **Frame Format**: Which SPI frame format was used (16/24/32-bit)

## ✨ **Feature Requests**

### 🚀 **Proposing New Features and Enhancements**

When proposing new features:

1. **Use Case** - Describe the specific encoder use case
2. **Technical Specification** - Provide detailed technical requirements
3. **API Design** - Propose the interface design following established patterns
4. **Implementation Plan** - Outline the implementation approach
5. **Testing Strategy** - Describe how the feature will be tested

## 🔄 **Development Workflow**

### 📋 **Step-by-Step Development Process**

1. **Fork the Repository**
2. **Create a Feature Branch**
3. **Implement Your Changes** following the [Coding Standards](https://github.com/hardfoc/org-discussions)
4. **Write Tests** for your changes
5. **Document Your Changes** with examples
6. **Submit a Pull Request**

## 📋 **Code Quality Standards**

- **C++20 Compliance** - Code compiles without warnings
- **HardFOC Compatibility** - Tested on HardFOC boards
- **Error Handling** - All error conditions handled appropriately
- **Documentation** - All public APIs documented with Doxygen
- **Tests** - Adequate test coverage provided
- **Performance** - Real-time requirements met
- **Coding Standards** - Follows [HardFOC Coding Standards](https://github.com/hardfoc/org-discussions)

## 🔗 **Resources**

- **[HardFOC Coding Standards](https://github.com/hardfoc/org-discussions)** - Complete coding standards document
- **[AS5047U Datasheet](../datasheet/)** - Hardware datasheet
- **[Documentation](../docs/)** - Driver documentation
- **[Examples](../examples/)** - Example code and usage patterns

---

## 🚀 Thank You for Contributing to HardFOC

Your contributions help make HardFOC motor controller boards more accessible and powerful for everyone.

