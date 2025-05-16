/**
 * @file LSA08_Simplified.cpp
 * @author Joel Jojo Painuthara (joeljojop@gmail.com)
 * @brief Source code of the LSA08_Simplified library
 * @version 1.2.0
 * @date 2025-05-16
 */

#include "LSA08_Simplified.h"

/**
 * @brief Construct a new LSA08 object set to analog mode
 *
 * @param pin The analog pin to which the LSA08 is connected
 */
LSA08::LSA08(unsigned int pin)
{
    this->mode = LSA08_MODE_ANALOG;
    this->pin = pin;
}

/**
 * @brief Construct a new LSA08 object set to hardware serial mode
 *
 * @param port The hardware serial port to which the LSA08 is connected
 * @param baudrate The baud rate for the serial communication
 * @param addr The address of the LSA08
 * @param en_pin The pin to which UART enable pin of the LSA08 is connected
 */
LSA08::LSA08(HardwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin)
{
    this->mode = LSA08_MODE_SERIAL;
    this->port = port;
    this->addr = addr;
    this->pin = en_pin;
    this->baudrate = baudrate;
}

/**
 * @brief Construct a new LSA08 object set to software serial mode
 *
 * @param port The software serial port to which the LSA08 is connected
 * @param baudrate The baud rate for the serial communication
 * @param addr The address of the LSA08
 * @param en_pin The pin to which the UART enable pin of the LSA08 is connected
 */
LSA08::LSA08(SoftwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin)
{
    this->mode = LSA08_MODE_SOFT_SERIAL;
    this->soft_port = port;
    this->addr = addr;
    this->pin = en_pin;
}

/**
 * @brief Initialize the LSA08 sensor
 *
 * This function sets the pin mode and initializes the serial communication.
 * It should be called in the setup() function of the Arduino sketch when using
 * the LSA08 in serial mode (both hardware and software serial).
 */
void LSA08::init()
{
    if (this->mode == LSA08_MODE_SERIAL)
    {
        pinMode(this->pin, OUTPUT);
        digitalWrite(this->pin, HIGH);
        this->port->begin(this->baudrate);
    }
    else if (this->mode == LSA08_MODE_SOFT_SERIAL)
    {
        pinMode(this->pin, OUTPUT);
        digitalWrite(this->pin, HIGH);
        this->soft_port->begin(this->baudrate);
    }
}

/**
 * @brief Disable line sensor reading from the LSA08
 * 
 * This function sets the UART enable pin to HIGH, disabling the stream of line sensor reading from the LSA08.
 * To be used only when the LSA08 is set to serial mode (both hardware and software serial).
 * 
 * @return @ref MODE_ERROR if the LSA08 is not set to serial mode, otherwise @ref SYS_OK
 */
int LSA08::disable_stream()
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    digitalWrite(this->pin, HIGH);
    return SYS_OK;
}

/**
 * @brief Enable line sensor reading from the LSA08
 * 
 * This function sets the UART enable pin to LOW, enabling the stream of line sensor reading from the LSA08.
 * To be used only when the LSA08 is set to serial mode (both hardware and software serial).
 * 
 * @return @ref MODE_ERROR if the LSA08 is not set to serial mode, otherwise @ref SYS_OK
 */
int LSA08::enable_stream()
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    digitalWrite(this->pin, LOW);
    return SYS_OK;
}

/**
 * @brief Set the UART mode of the LSA08
 * 
 * This function sets the UART mode of the LSA08 to either none, digital, analog, or raw.
 * To be used only when the LSA08 is set to serial mode (both hardware and software serial).
 * 
 * @param mode The UART mode to set (refer to @ref uart_mode for valid modes)
 * @return @ref MODE_ERROR if the LSA08 is not set to serial mode, @ref BAD_PACKET if the
 * UART mode is invalid, otherwise @ref SYS_OK
 */
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

/**
 * @brief Calibrate the LSA08 sensor
 * 
 * This function sends a command to the LSA08 to calibrate the sensor.
 * To be used only when the LSA08 is set to serial mode (both hardware and software serial).
 * 
 * @return @ref MODE_ERROR if the LSA08 is not set to serial mode, @ref BAD_PACKET
 * if request failed, otherwise @ref SYS_OK
 */
int LSA08::calibrate()
{
    if (mode != LSA08_MODE_SERIAL && mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    return send_packet('C', 0, COM_TYPE_COMMAND);
}

/**
 * @brief Set the line mode of the LSA08 sensor
 * 
 * This function sets the line mode of the LSA08 sensor to either light or dark.
 * To be used only when the LSA08 is set to serial mode (both hardware and software serial).
 * 
 * @param mode The line mode to set (refer to @ref line_mode for valid modes)
 * @return @ref MODE_ERROR if the LSA08 is not set to serial mode, @ref BAD_PACKET if the
 * line mode is invalid, otherwise @ref SYS_OK
 */
int LSA08::set_line_mode(line_mode mode)
{
    if (this->mode != LSA08_MODE_SERIAL && this->mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    if (mode != LIGHT_LINE && mode != DARK_LINE)
    {
        return BAD_PACKET;
    }
    return send_packet('L', (unsigned char)mode, COM_TYPE_COMMAND);
}

/**
 * @brief Set the threshold for the LSA08 sensor
 * 
 * This function sets the threshold for the LSA08 sensor to detect lines.
 * The threshold value should be between 0 and 7.
 * To be used only when the LSA08 is set to serial mode (both hardware and software serial).
 * 
 * @param threshold The threshold value to set
 * @return @ref MODE_ERROR if the LSA08 is not set to serial mode, @ref BAD_PACKET if the
 * threshold value is invalid, otherwise @ref SYS_OK
 */
int LSA08::set_threshold(unsigned int threshold)
{
    if (this->mode != LSA08_MODE_SERIAL && this->mode != LSA08_MODE_SOFT_SERIAL)
    {
        return MODE_ERROR;
    }
    if (threshold > 7)
    {
        return BAD_PACKET;
    }
    return send_packet('T', (unsigned char)threshold, COM_TYPE_COMMAND);
}

/**
 * @brief Read the line sensor data from the LSA08
 * 
 * This function reads the line sensor data from the LSA08.
 * The data is read from the analog pin if the LSA08 is set to analog mode,
 * or from the serial port if it is set to serial mode (both hardware and
 * software serial).
 * 
 * @return The line sensor data
 */
unsigned int LSA08::read_line()
{
    if (this->mode == LSA08_MODE_SERIAL || this->mode == LSA08_MODE_SOFT_SERIAL)
    {
        return get_data();
    }
    else if (this->mode == LSA08_MODE_ANALOG)
    {
        return analogRead(pin);
    }
    else
        return 0;
}

/**
 * @brief Send a packet to the LSA08 sensor
 * 
 * This function sends a request and value (is applicable) to the LSA08 sensor.
 * The request and value are sent over the serial port if the LSA08 is set
 * to serial mode (both hardware and software serial).
 * 
 * @param request The request to send (refer to the LSA08 documentation for valid requests)
 * @param value The value to send with the request (if applicable)
 * @param type The type of request - command or data
 * @return The response from the LSA08 sensor
 */
unsigned char LSA08::send_packet(unsigned char request, unsigned char value, com_type type)
{
    if (mode == LSA08_MODE_SERIAL)
    {
        port->write(addr);
        port->write(request);
        port->write(value);
        port->write(addr + request + value);
    }
    else if (mode == LSA08_MODE_SOFT_SERIAL)
    {
        soft_port->write(addr);
        soft_port->write(request);
        soft_port->write(value);
        soft_port->write(addr + request + value);
    }
    return get_response(type);
}

/**
 * @brief Get the data from the LSA08 sensor
 * 
 * This function reads the data from the LSA08 sensor.
 * The data is read from the serial port if the LSA08 is set to serial mode
 * (both hardware and software serial).
 * 
 * @return The data read from the LSA08 sensor
 */
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

/**
 * @brief Get the response from the LSA08 sensor
 * 
 * This function reads the response from the LSA08 sensor.
 * The response is read from the serial port if the LSA08 is set to serial mode
 * (both hardware and software serial).
 * 
 * @param type The type of request for which response is awaited (command or data)
 * @return The response from the LSA08 sensor
 */
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