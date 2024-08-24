/**
 * Source code of the LSA08_Simplified library
 * Developed by CSD Robocon NITK
 * 
 * This is free software. You can redistribute it and/or modify it under
 * the terms of MIT Licence.
 * To view a copy of this license, visit http://opensource.org/licenses/mit-license.php
 * 
 * version: 1.0.0
 */

#include "lsa08_simplified.h"

LSA08::LSA08(lsa08_mode mode, uint8_t pin)
{
    this->mode = mode;
    this->pin = pin;
    if(mode == LSA08_MODE_DIGITAL)
    {
        pinMode(pin, INPUT);
    }
}

LSA08::LSA08(lsa08_mode mode, HardwareSerial *port, int baudrate)
{
    this->mode = mode;
    if(mode == LSA08_MODE_SERIAL)
    {
        this->port = port;
        port->begin(baudrate);
    }
}

int8_t LSA08::send_packet(unsigned char addr, unsigned char command, unsigned char value, com_type type)
{
    if(mode != LSA08_MODE_SERIAL)
        return MODE_ERROR;
    else
    {
        port->write(addr);
        port->write(command);
        port->write(value);
        port->write(addr + command + value);
        return 0;
    }
}