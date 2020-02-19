
/*
Copyright (c) 2010 Donatien Garnier (donatiengar [at] gmail [dot] com)
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "TMP102.h"

#define TEMP_REG_ADDR 0x00
#define CONFIG_REG_ADDR 0x01
#define TLOW_REG_ADDR 0x02
#define THIGH_REG_ADDR 0x03

TMP102::TMP102(I2C& i2c, int addr7bit) : _i2c(i2c)
{
  _addr = addr7bit << 1;
  _config[0] = 0x60; // Default value of first config byte
  _config[1] = 0xA0; // Default value of second config byte
}

TMP102::~TMP102()
{
}

float TMP102::read()
{
  char buf[2];
  int16_t rawBits;
  
  buf[0] = TEMP_REG_ADDR;     // Set pointer to Temperature Register
  _i2c.write(_addr, buf, 1);
  _i2c.read(_addr, buf, 2);   // Read two bytes from register

  rawBits = (buf[0] << 8) | buf[1]; // Combine both bytes
  // Detect whether in normal temperature mode or extended temperature mode. Bit 0 flags this mode.
  if (buf[1] & 1) { // Extended Temperature Mode flag set
    rawBits = rawBits >> 3;
  } else {
    rawBits = rawBits >> 4;
  }
     
  return (float) ((float)rawBits * 0.0625);
}

void TMP102::config_ext_temp_mode(bool enable) {
  char buf[3];
  buf[0] = CONFIG_REG_ADDR;
  buf[1] = _config[0];
  if (enable) { // 13-bit mode, so technically up to 256 deg C but PCB may fail well before then
    _config[1] |= (1 << 5); // Turn on bit 5
  } else { // 12-bit mode, so max temp of 127.9375 deg C
    _config[1] &= ~(1 << 5); // Turn off bit 5
  }
  buf[2] = _config[1];
  _i2c.write(_addr, buf, 3);
}

void TMP102::config_conversion_rate(TMP102_ConversionRate_e newRate) {
  char buf[3];
  buf[0] = CONFIG_REG_ADDR;
  buf[1] = _config[0];
  switch (newRate) {
    case CONVERT_RATE_QUARTER_HZ:
      _config[1] &= ~(1 << 7); // Turn off bit 7
      _config[1] &= ~(1 << 6); // Turn off bit 6
      break;
    case CONVERT_RATE_1_HZ:
      _config[1] &= ~(1 << 7); // Turn off bit 7
      _config[1] |= (1 << 6);  // Turn on bit 6
      break;
    case CONVERT_RATE_4_HZ:
      _config[1] |= (1 << 7); // Turn on bit 7
      _config[1] &= ~(1 << 6);  // Turn off bit 6
      break;
    case CONVERT_RATE_8_HZ:
      _config[1] |= (1 << 7); // Turn on bit 7
      _config[1] |= (1 << 6); // Turn on bit 6
      break;
    default:
      // No change to parameters if invalid code received
      break;
  }
  buf[2] = _config[1];
  _i2c.write(_addr, buf, 3);
}
