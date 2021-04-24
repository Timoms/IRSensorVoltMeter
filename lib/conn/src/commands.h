#ifndef CMD_H
#define CMD_H

#include <Arduino.h>
#include <sensor.h>
#include <string.h>
#include <WiFiClient.h>

class Commands
{
private:
  Sensor s;
  String text =
      "Avaiable commands are:\r\n"
      "- \033[32mhelp\u001b[0m (displays this message)\r\n"
      "- \033[32mclear\u001b[0m (clears the screen)\r\n"
      "- \033[32mstat\u001b[0m (shows stats of the system)\r\n"
      "- \033[32mset hi lo\u001b[0m (sets trigger level)\r\n"
      "- \033[32mshow\u001b[0m (shows data from buffer)";
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
  String getValue(String data, char separator, int index)
  {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
      if (data.charAt(i) == separator || i == maxIndex)
      {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
      }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
  }

public:
  Commands()
  {
  }
  void printWelcome(WiFiClient telnetClient)
  {
    telnetClient.flush();
    telnetClient.println("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬");
    telnetClient.println("\u001b[31m\033[1mIntegrated EM Shell\u001b[0m");
    telnetClient.println("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬");
    telnetClient.printf("Host: \t\t%s\r\n", Config::deviceNameChr);
    telnetClient.printf("Millis: \t%lu\r\n", millis());
    telnetClient.printf("Heap RAM: \t%i\r\n", ESP.getFreeHeap());
    telnetClient.println("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬");
    telnetClient.println();
    telnetClient.flush();
  }
  void processCmd(String cmd, WiFiClient telnetClient)
  {
    Serial.write(cmd.c_str());
    if (strstr(cmd.c_str(), "help"))
    {
      //! Print help
      telnetClient.println(text);
    }
    else if (strstr(cmd.c_str(), "clear"))
    {
      //! clear screen
      //// telnetClient.print(ansiHOME + ansiCLC);
      telnetClient.println("\033[31mfeature not enabled\033[0m");
      yield();
    }
    else if (strstr(cmd.c_str(), "stat"))
    {
      //! print stats and trigger level
      telnetClient.printf("Host: \t\t%s\r\n", Config::deviceNameChr);
      telnetClient.printf("Millis: \t%lu\r\n", millis());
      telnetClient.printf("Heap RAM: \t%i\r\n", ESP.getFreeHeap());
      telnetClient.printf("Trigger H: \t%i\r\n", s.triggerLevelHigh);
      telnetClient.printf("Trigger L: \t%i\r\n", s.triggerLevelLow);
      yield();
    }
    else if (strstr(cmd.c_str(), "show"))
    {
      //! shows data for x seconds
      telnetClient.println("\033[31mfeature not enabled\033[0m");
      // for (int i = 0; i < s.bufferCounter; i++)
      // {
      //   telnetClient.printf("%i, %i, %i, %i\r\n", s.buffer[i][0], s.buffer[i][1], s.buffer[i][2], s.buffer[i][3]);
      // }
    }
    else if (strstr(cmd.c_str(), "set"))
    {
      String part01 = getValue(cmd, ' ', 0);
      int high = getValue(cmd, ' ', 1).toInt();
      int low = getValue(cmd, ' ', 2).toInt();
      telnetClient.printf("Setting trigger level to high: %i and low: %i\r\n", high, low);
      s.setTriggerLevels(low, high);
    }
    else if (strcmp(cmd.c_str(), "\n") == 0)
    {
      //! Empty line, ignore
      yield();
    }
    else
    {
      //! unknown command
      telnetClient.println("\033[31munknown command, type 'help' for a list of options\033[0m");
      yield();
    }
  }
};

#endif