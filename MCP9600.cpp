/* mbed MCP9600 Library, for the MCP9600 Thermocouple EMF to temperature Converter
 * Copyright (c) 2018, Yoshiteru Kita, ULVAC-PHI, INC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
 #include "MCP9600.h"
 #include "mbed.h"
 
// Constructor with I2C instance and I2C Device Address of MCP9600
MCP9600::MCP9600(I2C *i2c, char addr) : i2cMcp9600(i2c) {
    Mcp9600Addr = addr; 
}

// Destructor
MCP9600::~MCP9600() {
}

// Set I2C Device Address of MCP9600
void MCP9600::setAddress(char addr) {
    Mcp9600Addr = addr;     
}

// Get I2C Device Address of this instance
char MCP9600::getAddress(void) {
    return (Mcp9600Addr);
}

// Read Temperature from Temperature Register
// tempRegister  0:TH / 1:Tdelta / 2: TC
// Return value: temperature in float value (degree C)
float MCP9600::readTempRegister( char tempRegister )
{
    char    data[2];
    float   float_val;

    i2cMcp9600->write( Mcp9600Addr, &tempRegister, 1, 1);
    
    // Read temp (data[0] for MSB, data[1] for LSB)
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 1 );
    i2cMcp9600->read( Mcp9600Addr, &data[1], 1, 0 );
    
    // Convert to real value
    if ((data[0] & 0x80) == 0x80) {     // TH < 0
        data[0] = data[0] & 0x7F;       // Clear Sign
        float_val = 2032.0f - (data[0] * 16.0f);
        float_val = float_val + (15.9375f -  data[1] / 16.0f);
        float_val = float_val * (-1.0f);
    }
    else {                              // TH >= 0
        float_val = (data[0] * 16.0 + data[1] / 16.0);
    }
    return(float_val);
}


long MCP9600::readADCRegister(void)
{
    char    data[3];
    char    adcRegister;
    long    long_val;

    adcRegister = 0x03;
    i2cMcp9600->write( Mcp9600Addr, &adcRegister, 1, 1);
    
    // Read ADC Register (data[0] for MSB, data[2] for LSB)
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 1 );
    i2cMcp9600->read( Mcp9600Addr, &data[1], 1, 1 );
    i2cMcp9600->read( Mcp9600Addr, &data[2], 1, 0 );
    
    // Convert to long value
    long_val = data[0];
    long_val = long_val << 8;
    long_val = long_val + data[1];
    long_val = long_val << 8;
    long_val = long_val + data[2];

    return(long_val);
}


char MCP9600::writeStatusRegister( char statRegister )
{
    char    data[2];
    
    data[0] = 0x04;
    data[1] = statRegister;
    
    i2cMcp9600->write( Mcp9600Addr, data, 2, 1);
    
    // Readbeck Status Register
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );

    return(data[0]); 
}


char MCP9600::readStatusRegister( void )
{
    char    data;

    data = 0x04;
    i2cMcp9600->write( Mcp9600Addr, &data, 1, 0);
    
    // Read Status Register
    i2cMcp9600->read( Mcp9600Addr, &data, 1, 0 );
    
    return(data);
  
}


char MCP9600::writeSensorConfigRegister( char cnfgRegister )
{
    char    data[2];
    
    data[0] = 0x05;
    data[1] = cnfgRegister;
    
    i2cMcp9600->write( Mcp9600Addr, data, 2, 1);

    // Readback Sensor Configuration Register
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );

    return(data[0]); 
}


char MCP9600::readSensorConfigRegister( void )
{
    char    data;
    
    data = 0x05;    // Device ID / Revision Register
    i2cMcp9600->write( Mcp9600Addr, &data, 1, 1);
    
    i2cMcp9600->read( Mcp9600Addr, &data, 1, 0 );
    
    return (data);
}


char MCP9600::writeDeviceConfigRegister( char cnfgRegister )
{
    char    data[2];
    
    data[0] = 0x06;
    data[1] = cnfgRegister;

    
    i2cMcp9600->write( Mcp9600Addr, data, 2, 1);
    
    // Readback Device Configuration Register
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );

    return(data[0]); 
}


char MCP9600::readDeviceConfigRegister( void )
{
    char    data;

    data = 0x06;
    i2cMcp9600->write( Mcp9600Addr, &data, 1, 1);
    
    // Read Device Configuration Register
    i2cMcp9600->read( Mcp9600Addr, &data, 1, 0 );
    
    return(data);
  
}


char MCP9600::writeAlertConfigRegister(char alertCH, char alertConfig)
{
// alertCH: Channel Number to set the configuration (1 to 4)
// atertConfig:
// bit7: Clear Interruput (1 to clear)
// bit6: Not Used (0)
// bit5: Not Used (0)
// bit4: Alert for TH(0) or TC(1)
// bit3: Alert for Rising(0) or Falling(1)
// bit2: Active level (1:Active High / 0:Active Low)
// bit1: Alert Mode (1: Intrrupt Mode / 0:Comparator Mode)
// bit0: Alert Enable (1: Enable / 0: Disable)
    char    data[2];

    if ((alertCH > 0) && (alertCH < 5)) {
        data[0] = 0x07 + alertCH;     // Alert Configuration Register    
        data[1] = alertConfig;        // Alert Configuration data
        i2cMcp9600->write( Mcp9600Addr, data, 2, 1);

        // Read register
        i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );
        }
    return (data[0]);
}


char MCP9600::readAlertConfigRegister(char alertCH)
{
    char    data;
    
    if ((alertCH > 0) && (alertCH < 5)) {
        data = 0x07 + alertCH;     // Alert Configuration Register
        i2cMcp9600->write( Mcp9600Addr, &data, 1, 1);

        i2cMcp9600->read( Mcp9600Addr, &data, 1, 0 );
        }
    return (data);
}


char MCP9600::writeAlertHisterisisRegister(char alertCH, char alertHist)
{
// alertCH: Channel Number to set the configuration (1 to 4)
// atertHist:
// bit7: 128 C
// bit6: 64 C
// bit5: 32 C
// bit4: 16 C
// bit3: 8 C
// bit2: 4 C
// bit1: 2 C
// bit0: 1 C
    char    data[2];

    if ((alertCH > 0) && (alertCH < 5)) {
        data[0] = 0x0B + alertCH;       // Alert Histerisis Register    
        data[1] = alertHist;            // Alert Histerisis data
        i2cMcp9600->write( Mcp9600Addr, data, 2, 1);

        // Read register
        i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );
        }
    return (data[0]);
}


char MCP9600::readAlertHisterisisRegister(char alertCH)
{
    char    data;
    
    if ((alertCH > 0) && (alertCH < 5)) {
        data = 0x0B + alertCH;     // Alert Histerisis Register
        i2cMcp9600->write( Mcp9600Addr, &data, 1, 1);

        i2cMcp9600->read( Mcp9600Addr, &data, 1, 0 );
        }
    return (data);
}


float MCP9600::writeAlertLimit_MCP9600(char alertCH, float alertLimit_val)
{
// alertCH: Channel Number to set the Histerisis (1 to 4)
// atertLimitMSB
// bit7: Sign (1:Negative / 0:Positive)
// bit 6-0: Limit Value (x16)
// atertLimitLSB
// bit 7-2: Limit Value ( Div. 16)
// bit 1-0: Not Used
  
    char    data[3];
    float   float_val;
    char alertLimitMSB;
    char alertLimitLSB;
    
    // Convert to 2 bytes
    if (alertLimit_val >= 0.0f) {
        alertLimitMSB = alertLimit_val / 16.0f;
        alertLimit_val = alertLimit_val - alertLimitMSB * 16.0f;
        alertLimitLSB = alertLimit_val * 16.0f;
    } 
    else {
        alertLimit_val = alertLimit_val * (-1.0f);
        data[0] = alertLimit_val / 16.0f;
        alertLimitMSB = 1+ (2032.0f - alertLimit_val) / 16.0f;
        alertLimit_val = alertLimit_val - data[0] * 16.0f;
        alertLimit_val = 15.75f - alertLimit_val;
        alertLimitLSB = alertLimit_val * 16.0f;
        alertLimitMSB = alertLimitMSB | 0x80;   // Set Sign Flag
    }
    
    data[0] = 0x0F + alertCH;   // Alert Limit Value Register    
    data[1] = alertLimitMSB;    // Alert Limit Value (MSB)
    data[2] = alertLimitLSB;    // Alert Limit Value (LSB)
    i2cMcp9600->write( Mcp9600Addr, data, 3, 1 );    

    // Read Register
    i2cMcp9600->read( Mcp9600Addr, &data[1], 1, 1 );
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );

    // Convert to real value
    if ((data[1] & 0x80) == 0x80) {     // Limit Value < 0
        data[1] = data[1] & 0x7F;       // Clear Sign
        float_val = 2032.0f - (data[1] * 16.0f);
        float_val = float_val + (15.75f -  data[0] / 16.0f);
        float_val = (-1.0f) * float_val;
    }
    else {                              // Limit Value >= 0
        float_val = (data[1] * 16.0 + data[0] / 16.0);
    }
    return(float_val);
}


float MCP9600::readAlertLimit_MCP9600(char alertCH)
{
    char    data[2];
    float   float_val;

    data[0] = 0x0F + alertCH;   // Alert Limit Value Register    
    i2cMcp9600->write( Mcp9600Addr, &data[0], 1, 1);
    
    // Read register
    i2cMcp9600->read( Mcp9600Addr, &data[1], 1, 1 );
    i2cMcp9600->read( Mcp9600Addr, &data[0], 1, 0 );
    
    // Convert to real value
    if ((data[1] & 0x80) == 0x80) {     // Limit Value < 0
        data[1] = data[1] & 0x7F;       // Clear Sign
        float_val = 2032.0 - (data[1] * 16.0);
        float_val = float_val - (15.75 -  data[0] / 16.0);
    }
    else {                              // Limit Value >= 0
        float_val = (data[1] * 16.0 + data[0] / 16.0);
    }
    return(float_val);
}


char MCP9600::readIDRevisionRegister(void)
{
    char    data;
    
    data = 0x20;    // Device ID / Revision Register
    i2cMcp9600->write( Mcp9600Addr, &data, 1, 1);
    
    i2cMcp9600->read( Mcp9600Addr, &data, 1, 0 );
    
    return (data);
}
