/**
 * Example code demonstrating the LSA08_Simplified library usage.
 * Developed by CSD Robocon NITK
 * 
 * This is free software. You can redistribute it and/or modify it under
 * the terms of MIT Licence.
 * To view a copy of this license, visit http://opensource.org/licenses/mit-license.php
 * 
 * version: 1.0.0
 */

/**
 * In this example, the LSA08 line sensor is interfaced with an Arduino UNO via SoftwareSerial
 * and the reading from the sensor is displayed on the serial monitor.
 *
 * Connections:
 *    LSA08         | Arduino UNO Pin
 *    --------------+---------------------------------------
 *    Port A - 1    | 2
 *    Port A - 2    | 3
 *    Port A - 3    | 4
 *    Port A - 9    | - (External Power Supply 7.5V - 20V)
 *    Port A - 10   | GND (External Power Supply GND)  
 */

#include "LSA08_Simplified.h"
#include <SoftwareSerial.h>

// Create a software serial object with pins 2 and 3
SoftwareSerial mySerial(2, 3);

// Create a LSA08 sensor object
LSA08 sensor = LSA08(&mySerial, 9600, 1, 4);

void setup() {
  Serial.begin(9600);

  // Disable the line position data coming from the LSA08
  sensor.disable_stream();

  // Set LSA08 UART mode to analog data
  sensor.set_uart_mode(UART_MODE_ANALOG);

  // Set line mode to read dark lines
  sensor.set_line_mode(DARK_LINE);

  // Trigger an auto-calibration. Calibration takes sometime in the sensor, hence the 5 second delay
  sensor.calibrate();
  delay(5000);

  // Enable the line position data from the LSA08
  sensor.enable_stream();
}

void loop() {
  // Get the line position data and print it
  int pos = sensor.read_line();
  Serial.println(pos);
}
