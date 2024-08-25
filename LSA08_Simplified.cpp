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

#include "LSA08_Simplified.h"

LSA08::LSA08(lsa08_mode mode, uint8_t pin)
{
    this->mode = mode;
    if(this->mode == LSA08_MODE_ANALOG)
    {
        this->pin = pin;
    }
}

LSA08::LSA08(lsa08_mode mode, HardwareSerial *port, int baudrate, uint8_t addr, uint8_t en_pin)
{
    this->mode = mode;
    if(this->mode == LSA08_MODE_SERIAL)
    {
        this->port = port;
        this->addr = addr;
        pin = en_pin;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        this->port->begin(baudrate);
    }
}

int8_t LSA08::disable_stream()
{
    if(mode == LSA08_MODE_SERIAL)
    {
        return MODE_ERROR;
    }
    digitalWrite(pin, HIGH);
    return SYS_OK;
    
}

int8_t LSA08::enable_stream()
{
    if(mode != LSA08_MODE_SERIAL)
    {
        return MODE_ERROR;
    }
    digitalWrite(pin, LOW);
    return SYS_OK;
}

int8_t LSA08::set_uart_mode(uart_mode mode)
{
    if(this->mode != LSA08_MODE_SERIAL)
    {
        return MODE_ERROR;
    }
    return send_packet('D', (unsigned char)mode, COM_TYPE_COMMAND);
}

unsigned char LSA08::send_packet(unsigned char command, unsigned char value, com_type type)
{
    port->write(addr);
    port->write(command);
    port->write(value);
    port->write(addr + command + value);
    return get_response(type);
}

unsigned char LSA08::get_data()
{
    unsigned char data = 0;
    while(port->available()>0)
    {
        data = port->read();
    }
    return data;
}

unsigned char LSA08::get_response(com_type type)
{
    unsigned char data[2] = {0, 0};
    data[0] = get_data();
    if(type == COM_TYPE_DATA)
    {
        return data[0];
    }
    else if(type == COM_TYPE_COMMAND)
    {
        data[1] = get_data();
        if(data[0] == 'O' && data[1] == 'K')
        {
            return SYS_OK;
        }
        else
        {
            return BAD_PACKET;
        }
    }
}