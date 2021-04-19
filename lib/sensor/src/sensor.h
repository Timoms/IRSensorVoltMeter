#ifndef SENSOR_H
#define SENSOR_H

#include <EEPROM.h>
#include <config.h>
#include <Arduino.h>
#include <stdint.h>
#include <Scheduler.h>

#define MAX_CMD_LEN 80

class Sensor : public Task
{
public:
  static int sensorValueOff;
  static int sensorValueOn;         // value read from the photo transistor when ir LED is on
  static char command[MAX_CMD_LEN]; // command buffer
  static int inCount;               // command buffer index
  static bool cmdComplete;
  static char mode;
  static char dataOutput;
  static int triggerLevelLow;
  static int triggerLevelHigh;
  static bool triggerState;
  static uint16_t triggerLevelLowAddr;
  static uint16_t triggerLevelHighAddr;

  void setTriggerLevels();
  void readTriggerLevels();
  void detectTrigger(int val);
  void doCommand();
  void loop();
  void setup();
};

#endif