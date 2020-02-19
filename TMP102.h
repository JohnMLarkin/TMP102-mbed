/*****************************************************************************
 * TMP102 Temperature Sensor Class
 * 
 * Developed by John M. Larkin, Whitworth University
 * 
 * MIT License
 * Copyright (c) 2020 John Larkin
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ****************************************************************************/

#ifndef TMP102_H
#define TMP102_H

#include <mbed.h>

enum TMP102_ConversionRate_e {
  CONVERT_RATE_QUARTER_HZ,
  CONVERT_RATE_1_HZ,
  CONVERT_RATE_4_HZ,
  CONVERT_RATE_8_HZ
};

class TMP102
{
public:
  // Creates an instance of the class.
  // Connect module using a shared I2C object
  //
  // The 7-bit I2C address is selected using pin ADD0.
  //   ADD0 to ground = 0x48
  //   ADD0 to VCC = 0x49
  //   ADD0 to SDA = 0x4A
  //   ADD0 to SCL = 0x4B
  //
  // On the SparkFun breakout board, ADD0 comes connected to ground via a jumper on the board so the default address is 0x48.
  // If you want a different address, this jumper must be cut and ADD0 connected appropriately.  
  TMP102(I2C& i2c, int addr7bit = 0x48);
  
  ~TMP102();
  
  // Reads the current temperature, autodetecting 12-bit or 13-bit temperature mode
  float read();

  // Set temperature range to extended (up to 256 deg C) mode rather than standard (up to 128 deg C)
  void config_ext_temp_mode(bool enable);

  // Set rate of temperature conversions if in continuous conversion mode
  void config_conversion_rate(TMP102_ConversionRate_e newRate);
  
private:
  I2C& _i2c;
  int _addr; // store address in 8-bit form
  char _config[2]; // configuration bytes
};

#endif
