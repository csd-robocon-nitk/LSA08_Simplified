/**
 * @file LSA08_Simplified.h
 * @author Joel Jojo Painuthara (joeljojop@gmail.com)
 * @brief Header file of the LSA08_Simplified library
 * @version 1.2.0
 * @date 2025-05-16
 */

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * @brief Error code when using LSA08 in the wrong mode
 * 
 */
#define MODE_ERROR -1
/**
 * @brief Error code when data being sent to LSA08 is invalid or connection
 * is not established
 * 
 */
#define BAD_PACKET 1
/**
 * @brief Code when no error occurs
 * 
 */
#define SYS_OK 0

/**
 * @brief Mode of communication with the LSA08 sensor
 * 
 */
typedef enum
{
    /** Analog mode via analog pin */
    LSA08_MODE_ANALOG, 
    /** Digital mode via 8 digital pins */
    LSA08_MODE_DIGITAL, 
    /** Hardware serial mode via UART pins*/
    LSA08_MODE_SERIAL,
    /** Software serial mode */
    LSA08_MODE_SOFT_SERIAL
} lsa08_mode;

/**
 * @brief UART mode of the LSA08 sensor
 * 
 */
typedef enum
{
    /** No UART mode (No data output)*/
    UART_MODE_NONE,
    /** Digital UART mode (1 byte digital value from sensor)*/
    UART_MODE_DIGITAL,
    /** Analog UART mode (1 byte line position value. Value between 0-70 of
     * line is detected, 255 otherwise)*/
    UART_MODE_ANALOG,
    /** Raw UART mode (9 bytes of raw analog value from sensor)*/
    UART_MODE_RAW
} uart_mode;

/**
 * @brief Line mode of the LSA08 sensor
 * 
 */
typedef enum
{
    /** Detect dark line */
    DARK_LINE,
    /** Detect light line */
    LIGHT_LINE
} line_mode;

/**
 * @brief Type of request sent to LSA08 sensor
 * 
 */
typedef enum
{
    /** Command request (Response is OK if command is valid) */
    COM_TYPE_COMMAND,
    /** Data request (Response is the data from the sensor) */
    COM_TYPE_DATA
} com_type;

/**
 * @brief Definition of LSA08 object
 * 
 * This class provides methods to initialize, configure, and read data from
 * the LSA08 line sensor. It currently supports analog, hardware and software serial
 * communication modes.
 */
class LSA08
{
private:
    lsa08_mode mode;
    unsigned int pin;
    unsigned int addr;
    unsigned int baudrate;
    HardwareSerial *port;
    SoftwareSerial *soft_port;
    unsigned char send_packet(unsigned char command, unsigned char value, com_type type);
    unsigned char get_data();
    unsigned char get_response(com_type type);

public:
    LSA08(unsigned int pin);
    LSA08(HardwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin);
    LSA08(SoftwareSerial *port, int baudrate, unsigned int addr, unsigned int en_pin);
    void init();
    int disable_stream();
    int enable_stream();
    int set_uart_mode(uart_mode mode);
    int calibrate();
    int set_line_mode(line_mode mode);
    int set_threshold(unsigned int threshold);
    unsigned int read_line();
};
