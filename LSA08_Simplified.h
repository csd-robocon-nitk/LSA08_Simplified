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
#define BAD_PACKET 1
#define SYS_OK 0

typedef enum
{
    LSA08_MODE_ANALOG,
    LSA08_MODE_DIGITAL,
    LSA08_MODE_SERIAL
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
    COM_TYPE_COMMAND,
    COM_TYPE_DATA
} com_type;

class LSA08
{
private:
    uint8_t pin;
    uint8_t addr;
    HardwareSerial *port;
    lsa08_mode mode;
    unsigned char send_packet(unsigned char command, unsigned char value, com_type type);
    unsigned char get_data();
    unsigned char get_response(com_type type);
public:
    LSA08(lsa08_mode mode, uint8_t pin);
    LSA08(lsa08_mode mode, HardwareSerial *port, int baudrate, uint8_t addr, uint8_t en_pin);
    int8_t disable_stream();
    int8_t enable_stream();
    int8_t set_uart_mode(uart_mode mode);
};
