/*
* Program to read the electrical meter using a reflective light sensor
* This is the data acquisition part running on an Arduino Nano.
* It controls the infrared light barrier, detects trigger
* and communicates with a master computer (Raspberry Pi)
* over USB serial.
* Copyright 2015 Martin Kompf
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// #include <conn.h>
// #include <sensor.h>
// #include <config.h>

#include "../lib/conn/src/conn.h"
#include "../lib/sensor/src/sensor.h"
#include "../lib/config/src/config.h"
#include <Scheduler.h>
#include <Arduino.h>

Connection conn;
Sensor sensor;

/**
 * Setup.
 */
void setup()
{
  conn.setup();
  sensor.setup();
  //Scheduler.start(&conn);
  //Scheduler.start(&sensor);
  //Scheduler.begin(); // Begin scheduler functions
}

void loop()
{
  /* never call this main loop */
  // Serial.println("Warning: mainloop called.");
  conn.loop();
  sensor.loop();
}