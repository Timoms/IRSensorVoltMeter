#include <conn.h>

ESPTelnet telnet;
WiFiManager wifiManager;
IPAddress ip;
Sensor s;
bool connected;

void Connection::setupSerial()
{
  Serial.begin(Config::serialSpeed);
  while (!Serial)
  {
  }
  delay(200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(Config::motd);
}
bool Connection::isConnected()
{
  return (WiFi.status() == WL_CONNECTED);
}
void Connection::useWiFiManager()
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
void Connection::errorMsg(String error, bool restart)
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
void Connection::sendMessage(String s)
{
  telnet.println(s);
}

void onTelnetConnect(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" connected");
  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
  connected = true;
}
void onTelnetDisconnect(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" disconnected");
  connected = false;
}

void onTelnetReconnect(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" reconnected");
  connected = true;
}

void onTelnetConnectionAttempt(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" tried to connected");
}

void onHelpRequested()
{
  telnet.println("==== ENERGY METER ====");
  telnet.println("Avaiable commands are: help, stat, raw, trigger, count, set [low] [high], cmd, data");
  telnet.println("- help (displays this message)");
  telnet.println("- stat (shows stats of the system)");
  telnet.println("- raw (retrieve and print raw data)");
  telnet.println("- trigger (enter trigger mode and print binary trigger data)");
  telnet.println("- count (enter count mode)");
  telnet.println("- cmd (cancel data acquisition and enter command mode)");
  telnet.println("- data (cancel command mode and display data)");
}

void Connection::setupTelnet()
{
  // passing on functions for various telnet events
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);

  // passing a lambda function

  telnet.onInputReceived([](String str) {
    // checks for a certain command
    if (str == "stat")
    {
      telnet.println("ESPSYS");
      telnet.println(Config::deviceNameChr);
      s.readTriggerLevels();
    }
    else if (str == "help")
    {
      onHelpRequested();
    }
    else if (str == "raw")
    {
      s.dataOutput = 'D';
      telnet.println("RAW enabled");
    }
    else if (str == "trigger")
    {
      s.dataOutput = 'T';
      telnet.println("Trigger enabled");
    }
    else if (str == "count")
    {
      s.dataOutput = 'C';
      telnet.println("Command enabled");
    }
    else if (str == "cmd")
    {
      s.mode = 'C';
      telnet.println("Command enabled");
    }
    else if (str == "data")
    {
      s.mode = 'D';
    }
    else if (str == "")
    {
      // empty line, do nothing.
    }
    else
    {
      telnet.println("Command not found, type 'help' for a list of commands.");
    }
  });

  Serial.print("- Telnet: ");
  if (telnet.begin())
  {
    Serial.println("running");
  }
  else
  {
    Serial.println("error.");
    errorMsg("Will reboot...", true);
  }
}

void Connection::loop()
{
  // if (!connected)
  // {
  //   yield();
  // }

  // telnet.loop();
  // // send serial input to telnet as output
  // if (Serial.available())
  // {
  //   telnet.print(Serial.read());
  // }
}

void Connection::setup()
{
  setupSerial();

  // Serial.print("- Wifi: ");
  // useWiFiManager();

  // if (isConnected())
  // {
  //   ip = WiFi.localIP();
  //   Serial.print(" ");
  //   Serial.println(ip);
  //   setupTelnet();
  // }
  // else
  // {
  //   Serial.println();
  //   errorMsg("Error connecting to WiFi", true);
  // }
}