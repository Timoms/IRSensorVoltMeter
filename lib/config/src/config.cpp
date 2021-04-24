#include <config.h>

const int Config::analogInPin = A0;               // Analog input pin that the photo transistor is attached to
const int Config::irOutPin = D7;                  // Digital output pin that the IR-LED is attached to
const int Config::ledOutPin = D8;                 // Signal LED output pin
const int Config::serialSpeed = 9600;             // Serial Speed
const int Config::portalTimeout = 3600;           // Spawned WiFi Portal timeout in s
const char *Config::apPassword = "PWD";           // AP Password for Portal
const string Config::wifiSsid = "BB-DP-EM01";     // WiFi SSID for Portal
const char *Config::wifiSsidChr = "";             // WiFi SSID to connect
const char *Config::wifiPasswordChr = "";         // Password of Wifi to connect
const string Config::wifiPassword = "";           // Password of Wifi to connect
const string Config::deviceName = "BB-DP-EM01";   // Device identifier
const char *Config::deviceNameChr = "BB-DP-EM01"; // Device identifier
const String Config::motd = "Initializing...";    // Message when connecting to Telnet
const bool Config::resetWifi = false;             // Reset Wifi Config
const bool Config::csvOutput = true;              // Show output as CSV for fine tuning