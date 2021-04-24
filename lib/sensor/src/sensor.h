#ifndef SENSOR_H
#define SENSOR_H

#include <connection.h>
#include <EEPROM.h>
#include <config.h>
#include <Arduino.h>
#include <stdint.h>
#include <Scheduler.h>

class Sensor : public Task
{
private:
public:
  int buffer[256][4]; // buffer for telnet output
  int bufferCounter = 0;
  int sensorValueOff = 0; // value read from the photo transistor when ir LED is off
  int sensorValueOn = 0;  // value read from the photo transistor when ir LED is on
  int triggerLevelLow;
  int triggerLevelHigh;
  bool triggerState = false;
  uint16_t triggerLevelLowAddr = 0;
  uint16_t triggerLevelHighAddr = 4;

  void setTriggerLevels(int low, int high)
  {
    EEPROM.write(triggerLevelLowAddr, low);
    triggerLevelLow = low;
    EEPROM.write(triggerLevelHighAddr, high);
    triggerLevelHigh = high;
  }
  void readTriggerLevels()
  {
    triggerLevelLow = (int)EEPROM.read(triggerLevelLowAddr);
    triggerLevelHigh = (int)EEPROM.read(triggerLevelHighAddr);
    Serial.printf(" LOW: %i | HIGH: %i\r\n", triggerLevelLow, triggerLevelHigh);
  }
  bool detectTrigger(int val)
  {
    bool nextState = triggerState;
    if (val > triggerLevelHigh)
    {
      nextState = true;
    }
    else if (val < triggerLevelLow)
    {
      nextState = false;
    }
    if (nextState != triggerState)
    {
      triggerState = nextState;
      // Connection::println(triggerState ? 1 + "" : 0 + "");
      // control internal LED
      digitalWrite(Config::ledOutPin, triggerState);
    }
    return triggerState;
  }
  void loop()
  {
    if (bufferCounter > 256)
    {
      bufferCounter = 0;
    }

    //? perform measurement
    digitalWrite(Config::irOutPin, LOW);
    delay(10);
    //? read sensor value
    sensorValueOff = analogRead(Config::analogInPin);
    digitalWrite(Config::irOutPin, HIGH);
    delay(10);
    sensorValueOn = analogRead(Config::analogInPin);
    //? Print results
    if (Config::csvOutput)
    {
      Serial.printf("%i,%i,%i,%s\r\n", sensorValueOn, sensorValueOff, (sensorValueOn - sensorValueOff), detectTrigger(sensorValueOn - sensorValueOff) ? "true" : "false");
    }
    else
    {
      Serial.printf("S: ON[%i], OFF[%i] TRIGER[%i] -> %s\r\n", sensorValueOn, sensorValueOff, (sensorValueOn - sensorValueOff), detectTrigger(sensorValueOn - sensorValueOff) ? "✔" : "✘");
    }

    buffer[bufferCounter][0] = sensorValueOn;
    buffer[bufferCounter][1] = sensorValueOff;
    buffer[bufferCounter][2] = (sensorValueOn - sensorValueOff);
    buffer[bufferCounter][3] = detectTrigger(sensorValueOn - sensorValueOff);

    delay(500);
    bufferCounter++;
  }
  void setup()
  {
    pinMode(Config::irOutPin, OUTPUT);  // initialize IR Output (clear)
    pinMode(Config::ledOutPin, OUTPUT); // initialize LED Status LED
    Serial.println("- Sensor: ");
    readTriggerLevels(); // read sensor config from EEPROM
  }
} sensor;

#endif