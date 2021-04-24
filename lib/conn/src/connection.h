#ifndef C_H
#define C_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <config.h>
#include <Scheduler.h>
#include <commands.h>

#define MAX_TELNET_CLIENTS 2

WiFiServer telnetServer(23);

class Connection : public Task
{
private:
  WiFiManager wifiManager;
  IPAddress ip;
  bool connected;
  Commands c;
  WiFiClient telnetClient;
  uint8_t i;
  bool ConnectionEstablished;   // Flag for successfully handled connection
  String ansiPRE = "\033";      // escape code
  String ansiHOME = "\033[H";   // cursor home
  String ansiESC = "\033[2J";   // esc
  String ansiCLC = "\033[?25l"; // invisible cursor

  String ansiEND = "\033[0m"; // closing tag for styles
  String ansiBOLD = "\033[1m";

  String ansiRED = "\033[41m"; // red background
  String ansiGRN = "\033[42m"; // green background
  String ansiBLU = "\033[44m"; // blue background

  String ansiREDF = "\033[31m"; // red foreground
  String ansiGRNF = "\033[34m"; // green foreground
  String ansiBLUF = "\033[32m"; // blue foreground
  String BELL = "\a";

public:
  void loop()
  {
    handleClient();
  }
  void setup()
  {
    setupSerial();
    useWiFiManager();
    if (isConnected())
    {
      ip = WiFi.localIP();
      Serial.print(" ");
      Serial.println(ip);
      setupTelnet();
    }
    else
    {
      Serial.println();
      errorMsg("Error connecting to WiFi", true);
    }
  }
  // ! Setup Serial Connection
  void setupSerial()
  {
    Serial.begin(Config::serialSpeed);
    while (!Serial)
      ;
    delay(200);
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println(Config::motd);
  }
  bool isConnected()
  {
    return (WiFi.status() == WL_CONNECTED);
  }
  void useWiFiManager()
  {
    WiFi.hostname(Config::deviceNameChr);
    if (Config::resetWifi)
    {
      wifiManager.resetSettings();
    }
    wifiManager.setDebugOutput(false);
    wifiManager.setConfigPortalTimeout(Config::portalTimeout);
    wifiManager.setAPCallback([](WiFiManager *myWiFiManager) {
      Serial.println("- No known WiFi found");
      Serial.print("- Starting AP: ");
      Serial.println(myWiFiManager->getConfigPortalSSID());
      Serial.println(WiFi.softAPIP());
    });
    // enable autoconnect
    if (!(strlen(Config::wifiPasswordChr) == 0 ? wifiManager.autoConnect(Config::wifiSsidChr) : wifiManager.autoConnect(Config::wifiSsidChr, Config::wifiPasswordChr)))
    {
      Serial.println("- Failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    }
  }
  void errorMsg(String error, bool restart)
  {
    Serial.println(error);
    if (restart)
    {
      Serial.println("Rebooting now...");
      delay(2000);
      ESP.restart();
      delay(2000);
    }
  }
  void setupTelnet()
  {
    Serial.println("Starting Telnet...");
    telnetServer.begin(23);
    telnetServer.setNoDelay(true);
    Serial.println("Telnet running");
  }
  void handleClient()
  {
    //* Handle new client...
    if (telnetServer.hasClient())
    {
      //** client is connected
      if (!telnetClient || !telnetClient.connected())
      {
        if (telnetClient)
        {
          //** client disconnected
          telnetClient.stop();
        }
        //* ready for new client
        telnetClient = telnetServer.available();
        c.printWelcome(telnetClient);
      }
      else
      {
        //* client available, block new conections
        telnetServer.available().stop();
      }
    }

    //* Handle client connection
    if (telnetClient && telnetClient.connected() && telnetClient.available())
    {
      //* client input processing
      while (telnetClient.available())
      {
        c.processCmd(telnetClient.readStringUntil('\r'), telnetClient);
      }
      //* do other stuff with client input here
    }
  }
} conn;

#endif