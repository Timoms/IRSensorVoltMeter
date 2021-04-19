
#include <sensor.h>

int Sensor::sensorValueOff = 0;    // value read from the photo transistor when ir LED is off
int Sensor::sensorValueOn = 0;     // value read from the photo transistor when ir LED is on
char Sensor::command[MAX_CMD_LEN]; // command buffer
int Sensor::inCount = 0;           // command buffer index
bool Sensor::cmdComplete = false;
char Sensor::mode = 'D';
char Sensor::dataOutput = 'T';
int Sensor::triggerLevelLow;
int Sensor::triggerLevelHigh;
bool Sensor::triggerState = false;
uint16_t Sensor::triggerLevelLowAddr = 0;
uint16_t Sensor::triggerLevelHighAddr = 4;

void Sensor::setTriggerLevels()
{
  char *p = &command[1];
  while (*p != 0 && *p == ' ')
  {
    ++p;
  }
  char *q = p + 1;
  while (*q != 0 && *q != ' ')
  {
    ++q;
  }
  *q = 0;
  EEPROM.write(triggerLevelLowAddr, atoi(p));

  p = q + 1;
  while (*p != 0 && *p == ' ')
  {
    ++p;
  }
  q = p + 1;
  while (*q != 0 && *q != ' ')
  {
    ++q;
  }
  *q = 0;
  EEPROM.write(triggerLevelHighAddr, atoi(p));
}
void Sensor::readTriggerLevels()
{
  triggerLevelLow = (int)EEPROM.read(triggerLevelLowAddr);
  triggerLevelHigh = (int)EEPROM.read(triggerLevelHighAddr);
  Serial.print("Trigger levels: ");
  Serial.print(triggerLevelLow);
  Serial.print(" ");
  Serial.println(triggerLevelHigh);
}
void Sensor::detectTrigger(int val)
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
    Serial.println(triggerState ? 1 : 0);
    // control internal LED
    digitalWrite(Config::ledOutPin, triggerState);
  }
}
void Sensor::doCommand()
{
  // print prompt
  Serial.print(">");
  while (!cmdComplete)
  {
    // read input
    while (Serial.available())
    {
      // get the new byte:
      char inChar = (char)Serial.read();
      if (inChar == '\n' || inChar == '\r')
      {
        command[inCount] = 0;
        Serial.println();
        cmdComplete = true;
        break; // End of line
      }
      else if (inCount < MAX_CMD_LEN - 1)
      {
        command[inCount] = inChar;
        inCount++;
        // echo
        Serial.print(inChar);
      }
    }
  }

  // interprete command
  switch (command[0])
  {
  case 'D':
    // start raw data mode
    mode = 'D';
    dataOutput = 'R';
    break;
  case 'T':
    // start trigger data mode
    mode = 'D';
    dataOutput = 'T';
    break;
  case 'S':
    // set trigger levels
    setTriggerLevels();
    readTriggerLevels();
    break;
  }

  // clear command buffer
  command[0] = 0;
  inCount = 0;
  cmdComplete = false;
}

// ! Main-Loop of Sensor.cpp

void Sensor::loop()
{
  Serial.println("Sensor loop");
  Serial.println(Sensor::mode);
  if (mode == 'C')
  {
    Serial.println("Sensor cmd loop...");
    this->doCommand();
  }
  else if (mode == 'D')
  {
    Serial.println("Sensor data loop...");
    if (Serial.available())
    {
      char inChar = (char)Serial.read();
      if (inChar == 'C')
      {
        // exit data mode
        mode = 'C';
      }
    }
  }
  if (mode == 'D')
  {
    Serial.println("Sensor data mode d loop...");
    // perform measurement
    // turn IR LED off
    digitalWrite(Config::irOutPin, LOW);
    // wait 10 milliseconds
    delay(10);
    // read the analog in value:
    sensorValueOff = analogRead(Config::analogInPin);
    // turn IR LED on
    digitalWrite(Config::irOutPin, HIGH);
    delay(10);
    // read the analog in value:
    sensorValueOn = analogRead(Config::analogInPin);

    switch (dataOutput)
    {
    case 'R':
      // print the raw data to the serial monitor
      Serial.println(sensorValueOn - sensorValueOff);
      break;
    case 'T':
      // detect and output trigger
      this->detectTrigger(sensorValueOn - sensorValueOff);
      break;
    }
  }
  delay(500);
  //delay(10);
}

void Sensor::setup()
{
  Serial.println();
  pinMode(Config::irOutPin, OUTPUT);  // initialize IR Output (clear)
  pinMode(Config::ledOutPin, OUTPUT); // initialize LED Status LED
  Serial.print("- Sensor: ");
  readTriggerLevels(); // read sensor config from EEPROM
}

// command line

// Mode of serial line:
// C - command, D - data output

// Data output mode:
// R - raw data
// T - trigger events
// C - counter

// trigger state and level

// Address of trigger levels in EEPROM

/**
 * Set trigger levels (low high) from the command line
 * and store them into EEPROM
 */

/**
 * Read trigger levels from EEPROM
 */

/**
 * Detect and print a trigger event
 */

/**
 * Read one line from serial connection and interpret it as a command
 */
