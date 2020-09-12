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
 
#ifndef MBED_MCP9600_H
#define MBED_MCP9600_H

#include "mbed.h"

class MCP9600 {
public:
    // Constructor with I2C instance and I2C Device Address of MCP9600
    MCP9600(I2C *i2c, char addr);
    
    // Set I2C Device Address of MCP9600
    void setAddress(char addr);
    
    // Get I2C Device Address of this instance
    char getAddress(void);
    
    // Read Temperature from Temperature Register
    // tempRegister  0:TH / 1:Tdelta / 2: TC
    // Return value: temperature in float value (degree C)
    float readTempRegister(char tempRegister);

    // Read ADC register
    // Return value: 24 bit ADC register value
    // Refer MCP9600 datasheet for details.
    long readADCRegister(void);
    
    // Write to Status Register
    // statRegister: Status data to write
    // About register bit, refer MCP9600 datasheet.
    char writeStatusRegister( char statRegister );
    
    // Read Status Register
    // About register bit, refer MCP9600 datasheet.
    char readStatusRegister( void );

    // Write to Sensor Configuration Register
    // cnfgRegister: Sensor Config data to write
    // About register bit, refer MCP9600 datasheet.
    char writeSensorConfigRegister( char cnfgRegister );
    
    // Read Sensor Configuration Register
    // About register bit, refer MCP9600 datasheet.
    char readSensorConfigRegister( void );
    
    // Write to Device Configuration Register
    // cnfgRegister: Device Config data to write
    // About register bit, refer MCP9600 datasheet.
    char writeDeviceConfigRegister( char cnfgRegister );
    
    // Read Device Configuration Register
    // About register bit, refer MCP9600 datasheet.
    char readDeviceConfigRegister( void );
 
    // Write to Alert Configuration Register
    // alertCH: Alert CH (1 to 4)
    // alertConfig: Alert Configuration data to write to register
    // Return Value: Alert Configuration Register value (Read from register)
    // About register bit, refer MCP9600 datasheet.
    char writeAlertConfigRegister(char alertCH, char alertConfig);

    // Read Alter Configuration Register
    // alertCH: Alert CH (1 to 4)
    // Return Value: Alert Configuration Register value
    // About register bit, refer MCP9600 datasheet.
    char readAlertConfigRegister(char alertCH);

    // Write to Alert Histerisis Register
    // alertCH: Alert CH (1 to 4)
    // alertHist: Alert Histerisis data to write to register
    // Return Value: Alert Histerisis Register value (Read from register)
    char writeAlertHisterisisRegister(char alertCH, char alertHist);

    // Read Alter Histerisis Register
    // alertCH: Alert CH (1 to 4)
    // Return Value: Alert Histerisis Register value
    char readAlertHisterisisRegister(char alertCH);
    
    // Write to Alert Limit Value Register
    // alertCH: Alert CH (1 to 4)
    // alertLimit_val: Alert Limit value to write to register
    // Return Value: Alert Limit value in real value (Read from register)
    float writeAlertLimit_MCP9600(char alertCH, float alertLimit_val);

    // Read Alert Limit Value Register
    // alertCH: Alert CH (1 to 4)
    // Return Value: Alert Limit value in real value (Read from register)
    float readAlertLimit_MCP9600(char alertCH);

    // Read Device ID / Revision Register
    // Return Value: Device ID / Revision (Read from register)
    char readIDRevisionRegister(void);

    // Destructor
    ~MCP9600();

private:
    char Mcp9600Addr ;  // I2C Slave Address of MCP9600
    I2C *i2cMcp9600;    // I2C bus reference
};

#endif
