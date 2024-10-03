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

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MODE_ERROR -1
#define BAD_PACKET 1
#define SYS_OK 0

typedef enum
{
    LSA08_MODE_ANALOG,
    LSA08_MODE_DIGITAL,
    LSA08_MODE_SERIAL,
    LSA08_MODE_SOFT_SERIAL
} lsa08_mode;

typedef enum
{
    UART_MODE_NONE,
    UART_MODE_DIGITAL,
    UART_MODE_ANALOG,
    UART_MODE_RAW
} uart_mode;

typedef enum
{
    DARK_LINE,
    LIGHT_LINE
} line_mode;

typedef enum
{
    COM_TYPE_COMMAND,
    COM_TYPE_DATA
} com_type;

class LSA08
{
private:
    lsa08_mode mode;
    unsigned int pin;
    unsigned int addr;
    HardwareSerial *port;
    SoftwareSerial *soft_port;
    unsigned char send_packet(unsigned char command, unsigned char value, com_type type);
    unsigned char get_data();
    unsigned char get_response(com_type type);

public:
    LSA08(unsigned int pin);
    LSA08(HardwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin);
    LSA08(SoftwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin);
    int disable_stream();
    int enable_stream();
    int set_uart_mode(uart_mode mode);
    int calibrate();
    int set_line_mode(line_mode mode);
    int set_threshold(unsigned int threshold);
    unsigned int read_line();
};
