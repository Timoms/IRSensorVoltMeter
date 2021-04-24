#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <Arduino.h>

using namespace std;

class Config
{
public:
  static const int analogInPin; // Analog input pin that the photo transistor is attached to
  static const int irOutPin;    // Digital output pin that the IR-LED is attached to
  static const int ledOutPin;   // Signal LED output pin
  static const int serialSpeed; // Serial Speed
  static const int portalTimeout;
  static const char *apPassword;
  static const string wifiSsid;
  static const char *wifiSsidChr;
  static const char *wifiPasswordChr;
  static const string wifiPassword;
  static const string deviceName; // Device identifier
  static const char *deviceNameChr;
  static const String motd;
  static const bool resetWifi;
  static const bool csvOutput;
};

#endif