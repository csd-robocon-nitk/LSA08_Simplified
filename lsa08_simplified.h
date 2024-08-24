/**
 * Header for LSA08_Simplified library
 * Developed by CSD Robocon NITK
 * 
 * This is free software. You can redistribute it and/or modify it under
 * the terms of MIT Licence.
 * To view a copy of this license, visit http://opensource.org/licenses/mit-license.php
 * 
 * version: 1.0.0
 */

#include<Arduino.h>

#define MODE_ERROR -1

typedef enum lsa08_modes
{
    LSA08_MODE_ANALOG,
    LSA08_MODE_DIGITAL,
    LSA08_MODE_SERIAL
} lsa08_mode;

typedef enum com_types
{
    COM_TYPE_COMMAND,
    COM_TYPE_DATA
} com_type;

class LSA08
{
private:
    uint8_t pin;
    HardwareSerial *port;
    lsa08_mode mode;
    int8_t send_packet(unsigned char addr, unsigned char command, unsigned char value, com_type type);
public:
    LSA08(lsa08_mode mode, uint8_t pin);
    LSA08(lsa08_mode mode, HardwareSerial *port, int baudrate);
};
