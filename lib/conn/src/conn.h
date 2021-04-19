#ifndef CONN_H
#define CONN_H

#include <IPAddress.h>
#include <WiFiManager.h>
#include <ESPTelnet.h>
#include <config.h>
#include <Scheduler.h>
#include <sensor.h>

using namespace std;

class Connection : public Task
{
public:
  bool connected;
  int portalTimeout;
  char *apName;
  char *apPassword;
  int serialSpeed;
  string wifiSsid;
  string wifiPassword;
  IPAddress ip;
  WiFiManager wifiManager;
  void setupSerial();
  bool isConnected();
  void useWiFiManager();
  void errorMsg(String, bool);
  void sendMessage(String s);
  void inputReceived(String msg);
  void setupTelnet();
  void loop();
  void setup();
};
#endif