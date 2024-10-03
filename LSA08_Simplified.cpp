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

LSA08::LSA08(unsigned int pin)
{
    this->mode = LSA08_MODE_ANALOG;
    this->pin = pin;
}

LSA08::LSA08(HardwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin)
{
    this->mode = LSA08_MODE_SERIAL;
    this->port = port;
    this->addr = addr;
    pin = en_pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    this->port->begin(baudrate);
}

LSA08::LSA08(SoftwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin)
{
    this->mode = LSA08_MODE_SOFT_SERIAL;
    this->soft_port = port;
    this->addr = addr;
    pin = en_pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    this->soft_port->begin(9600);
}

int LSA08::disable_stream()
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    digitalWrite(pin, HIGH);
    return SYS_OK;
}

int LSA08::enable_stream()
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    digitalWrite(pin, LOW);
    return SYS_OK;
}

int LSA08::set_uart_mode(uart_mode mode)
{
    if (this->mode != LSA08_MODE_SERIAL && this->mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    if (mode > 3 && mode < 0)
    {
        return BAD_PACKET;
    }
    return send_packet('D', (unsigned char)mode, COM_TYPE_COMMAND);
}

int LSA08::calibrate()
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    return send_packet('C', 0, COM_TYPE_COMMAND);
}

int LSA08::set_line_mode(line_mode mode)
{
    if (this->mode != LSA08_MODE_SERIAL && this->mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    if (mode > 1 && mode < 0)
    {
        return BAD_PACKET;
    }
    return send_packet('L', (unsigned char)mode, COM_TYPE_COMMAND);
}

int LSA08::set_threshold(unsigned int threshold)
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    if (threshold > 7)
    {
        return BAD_PACKET;
    }
    return send_packet('T', (unsigned char)threshold, COM_TYPE_COMMAND);
}

unsigned int LSA08::read_line()
{
    if (mode == LSA08_MODE_SERIAL || mode == LSA08_MODE_SOFT_SERIAL)
    {
        return get_data();
    }
    else if (mode == LSA08_MODE_ANALOG)
    {
        return analogRead(pin);
    }
    else
        return 0;
}

unsigned char LSA08::send_packet(unsigned char command, unsigned char value, com_type type)
{
    if (mode == LSA08_MODE_SERIAL)
    {
        port->write(addr);
        port->write(command);
        port->write(value);
        port->write(addr + command + value);
    }
    else if (mode == LSA08_MODE_SOFT_SERIAL)
    {
        soft_port->write(addr);
        soft_port->write(command);
        soft_port->write(value);
        soft_port->write(addr + command + value);
    }
    return get_response(type);
}

unsigned char LSA08::get_data()
{
    unsigned char data = 0;
    if (mode == LSA08_MODE_SERIAL)
    {
        while (port->available() > 0)
        {
            data = port->read();
        }
    }
    else if (mode == LSA08_MODE_SOFT_SERIAL)
    {
        while (soft_port->available() > 0)
        {
            data = soft_port->read();
        }
    }
    return data;
}

unsigned char LSA08::get_response(com_type type)
{
    unsigned char data[2] = {0, 0};
    data[0] = get_data();
    if (type == COM_TYPE_DATA)
    {
        return data[0];
    }
    else if (type == COM_TYPE_COMMAND)
    {
        data[1] = get_data();
        if (data[0] == 'O' && data[1] == 'K')
        {
            return SYS_OK;
        }
        else
        {
            return BAD_PACKET;
        }
    }
    else
    {
        return 0;
    }
}