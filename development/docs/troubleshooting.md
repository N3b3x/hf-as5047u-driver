# Troubleshooting

This guide helps you diagnose and resolve common issues when using the AS5047U driver.

## Common Error Messages

### Error: CRC Errors

**Symptoms:**
- Frequent CRC errors when reading angle
- `GetErrorFlags()` returns `CrcError` flag
- Unreliable angle readings

**Causes:**
- SPI signal integrity issues (noise, reflections)
- Incorrect SPI mode or timing
- Loose connections
- SPI speed too high
- Power supply noise

**Solutions:**
1. **Reduce SPI Speed**: Try lowering SPI frequency (e.g., 1-2 MHz instead of 10 MHz)
2. **Check SPI Mode**: Ensure SPI Mode 1 (CPOL=0, CPHA=1) or Mode 3 (CPOL=1, CPHA=1)
3. **Enable Retries**: Use retry parameter: `encoder.GetAngle(3)` for automatic retry
4. **Verify Connections**: Check all SPI connections are secure
5. **Add Decoupling**: Ensure proper decoupling capacitors (100nF + 10µF) near VDD
6. **Check Ground**: Verify good ground connection between MCU and sensor
7. **Use 24-bit Frames**: Switch to `FrameFormat::SPI_24` for CRC protection

---

### Error: Framing Errors

**Symptoms:**
- `GetErrorFlags()` returns `FramingError` flag
- Communication failures

**Causes:**
- Incorrect SPI frame format
- CS timing issues
- SPI transaction length mismatch

**Solutions:**
1. **Check Frame Format**: Ensure frame format matches sensor configuration
2. **Verify CS Timing**: Check chip select assertion/deassertion timing
3. **Review SPI Implementation**: Ensure `transfer()` method handles CS correctly

---

### Error: AGC Warning

**Symptoms:**
- `GetErrorFlags()` returns `AgcWarning` flag
- AGC value at 0 or 255
- Unreliable readings

**Causes:**
- Magnet too close or too far from sensor
- Weak magnetic field
- Stray magnetic fields interfering

**Solutions:**
1. **Check Air Gap**: Adjust magnet distance (0.5-3.0 mm recommended, 1.0 mm typical)
2. **Verify Magnet**: Ensure diametrically magnetized magnet (6-12 mm diameter)
3. **Check Field Strength**: Magnetic field should be 30-100 mT at sensor surface
4. **Remove Interference**: Keep magnet away from ferromagnetic materials and motors
5. **Check Alignment**: Ensure magnet is centered above sensor die

---

### Error: Magnitude Half Warning

**Symptoms:**
- `GetErrorFlags()` returns `MagHalf` flag
- Magnetic field magnitude is half of regulated value

**Causes:**
- Magnet too far from sensor
- Weak magnetic field
- Incorrect magnet type

**Solutions:**
1. **Reduce Air Gap**: Move magnet closer to sensor (within 0.5-3.0 mm range)
2. **Check Magnet Strength**: Verify magnet provides adequate field (30-100 mT)
3. **Verify Magnet Type**: Ensure diametrically magnetized (2-pole) magnet

---

## Hardware Issues

### Device Not Responding

**Symptoms:**
- No response from sensor
- All readings return 0 or invalid values
- SPI communication fails

**Checklist:**
- [ ] Verify power supply voltage is 2.7V - 3.6V (3.3V typical)
- [ ] Check all connections are secure (VDD, GND, CLK, MISO, MOSI, CS)
- [ ] Verify SPI pin mapping (AS5047U uses reversed MISO/MOSI naming)
- [ ] Check CS line is properly controlled (active low)
- [ ] Verify SPI mode is correct (Mode 1 or Mode 3)
- [ ] Use oscilloscope/logic analyzer to verify bus activity
- [ ] Check for short circuits or incorrect wiring

---

### Communication Errors

**Symptoms:**
- Timeout errors
- CRC/checksum errors
- Inconsistent readings

**Solutions:**
- **Check Bus Speed**: Ensure SPI speed is within specifications (up to 10 MHz, 4 MHz recommended)
- **Verify Signal Integrity**: Check for noise, reflections, or signal degradation
- **Ensure Proper Termination**: Verify proper bus termination
- **Check for Loose Connections**: Inspect all connections
- **Reduce SPI Speed**: Try lower frequency if issues persist
- **Add Pull-ups**: Ensure proper pull-up resistors if needed

---

### Incorrect Angle Readings

**Symptoms:**
- Angle values don't change when rotating
- Angle values jump erratically
- Angle values are always zero

**Causes:**
- Magnet not properly positioned
- Incorrect magnet type
- Stray magnetic fields
- Sensor not initialized

**Solutions:**
1. **Check Magnet Placement**: Verify magnet is centered and at correct distance
2. **Verify Magnet Type**: Ensure diametrically magnetized (2-pole) magnet
3. **Check for Interference**: Remove sources of stray magnetic fields
4. **Verify Initialization**: Ensure SPI bus is properly initialized
5. **Check Error Flags**: Read error flags to diagnose specific issues

---

## Software Issues

### Compilation Errors

**Error: "No matching function"**

**Solution:**
- Ensure you've implemented the `transfer()` method in your SPI interface
- Check method signature matches: `void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len)`
- Verify you're inheriting from `as5047u::SpiInterface<YourClass>`

**Error: "Undefined reference"**

**Solution:**
- Verify you're linking the driver source file (`as5047u.cpp`)
- Check include paths are correct
- Ensure C++20 standard is enabled: `-std=c++20`

**Error: "Template instantiation failed"**

**Solution:**
- Verify your SPI interface class is properly templated: `class MySPI : public as5047u::SpiInterface<MySPI>`
- Check that all required methods are implemented

---

### Runtime Errors

**Initialization Fails**

**Checklist:**
- [ ] SPI bus interface is properly initialized
- [ ] Hardware connections are correct
- [ ] SPI configuration matches sensor requirements
- [ ] Device is powered and ready
- [ ] CS pin is correctly configured

**Unexpected Behavior**

**Checklist:**
- [ ] Verify configuration matches your use case
- [ ] Check for timing issues (add delays if needed)
- [ ] Review error handling code
- [ ] Verify frame format is correct
- [ ] Check error flags for diagnostic information

---

## Debugging Tips

### Enable Debug Output

Add debug prints to your SPI implementation:

```cpp
void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) {
    printf("SPI Transfer: len=%zu\n", len);
    // ... your SPI code
}
```

### Use DumpStatus()

Print comprehensive sensor status:

```cpp
encoder.DumpStatus();
```

This prints angle, velocity, AGC, magnitude, and error flags.

### Check Error Flags

Always check error flags after operations:

```cpp
uint16_t angle = encoder.GetAngle();
AS5047U_Error errors = encoder.GetStickyErrorFlags();
if (errors != AS5047U_Error::None) {
    printf("Errors: 0x%04X\n", static_cast<uint16_t>(errors));
}
```

### Use a Logic Analyzer

For bus communication issues, a logic analyzer can help:
- Verify correct protocol timing
- Check for signal integrity issues
- Validate data being sent/received
- Debug CS timing issues

### Verify SPI Pin Mapping

**Important**: AS5047U uses reversed SPI pin naming:
- MCU MOSI → AS5047U MISO (data from MCU to sensor)
- MCU MISO → AS5047U MOSI (data from sensor to MCU)

Double-check your wiring matches this!

---

## FAQ

### Q: What SPI frame format should I use?

**A:** For most applications, use `FrameFormat::SPI_24` - it provides CRC protection with good throughput. Use `SPI_16` for maximum speed (no CRC), or `SPI_32` for daisy-chain applications.

### Q: How do I know if my magnet is positioned correctly?

**A:** Check the AGC value - it should be between 10-245 (not at limits 0 or 255). Also check magnitude - it should be in a reasonable range (typically 5000-15000). Use `DumpStatus()` to see all diagnostics.

### Q: Can I use multiple AS5047U sensors on the same SPI bus?

**A:** Yes, use `FrameFormat::SPI_32` with different pad bytes for each sensor. Each sensor needs its own CS line.

### Q: Why are my angle readings noisy?

**A:** Enable adaptive filtering: `encoder.SetAdaptiveFilter(true)` and adjust filter parameters. Also check magnet placement and ensure proper decoupling capacitors.

### Q: How do I calibrate the zero position?

**A:** Read the current angle when at your desired zero position, then call `encoder.SetZeroPosition(current_angle)`. See [Example 7](examples.md#example-7-zero-position-calibration) for details.

### Q: What happens if I program OTP incorrectly?

**A:** OTP programming is **irreversible**. If you program incorrect settings, you cannot change them. Always verify settings before programming OTP.

---

## Getting More Help

If you're still experiencing issues:

1. Check the [API Reference](api_reference.md) for method details
2. Review [Examples](examples.md) for working code
3. Search existing issues on GitHub
4. Open a new issue with:
   - Description of the problem
   - Steps to reproduce
   - Hardware setup details (MCU, wiring, magnet type)
   - Error messages/logs
   - Output from `DumpStatus()`

---

**Navigation**
⬅️ [Examples](examples.md) | [Back to Index](index.md)

