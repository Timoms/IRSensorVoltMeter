# IRSensorVoltMeter
### Software to read and transmit three-phase meter cycles.
---

## What you need

* ESP8266 (Im using the ESP32 / Lolin v3)
* Platform IO and a PC to flash the script
* A case for the ESP
* IR LED (2 pin, mostly transparent)
* IR Receiver LED (2 pin not 3, mostly black or purple)
* Optional: Colored LED for visual feedback

## Background

I am currently using the following device to measure the kWh.  
This devices displays the kWh with a turning disk which is marked red to indicate a full turn.   
<img src="https://upload.wikimedia.org/wikipedia/commons/1/1d/Drehstromz%C3%A4hler_l%26g_hochkant.jpg" alt="drawing" width="300"/> (2)  

We can use the mark to check for changes on the reflectance of the disk.  
Our IR sensor will then send different values based on white / red surface.

<img src="https://www.kompf.de/tech/images/ireflb-emeir.png" alt="drawing" width="300"/> (1)  

Theres an easy logic behind the sensor - check out sensor.h for more information.

## Getting Started

Clone this repo and import it using VSCode and PlatformIO.  
For more information visit the official PIO documentation.

### **Step 1 - Preparing**
----

Check the config.cpp - and change values based on your network.

```c
const int Config::analogInPin = A0;   // <- Sensor
const int Config::irOutPin = D7;      // <- IR LED
const int Config::ledOutPin = D8;     // <- Visual Feedback LED
...
const bool Config::csvOutput = true;  // <- Used for fine tuning, not affecting anything
```

### **Step 2 - Uploading**
----

Check the platform.ini in the root folder.  
You need to change values according to your board.

```ini
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 9600
upload_speed = 1500000
```
Looking good? Hit upload and grab a cup of tea!

### **Step 3 - Tuning**
----

The Script you have just uploaded starts a Telnet-Server for you.  
(Don't worry about telnet, its just for some basic tasks!)

When connecting to port 23 to the device you will be greeted with a summary of the device.
> ### Attention! Sending commands will pause the Sensor library from collecting data.  
sending the command `set hi lo` will set your limits for the detection.  

We can now go ahead and connect to the device using a micro-usb cable and a laptop with PuTTY.  
The console will show lines like that:
```
1024,127,897,true
1024,127,897,true
1024,128,896,true
```
which translate to 
```
sensorValueOn, sensorValueOff, (sensorValueOn - sensorValueOff), detectTrigger
```

Feed those into Excel and create a chart:

<img src="https://www.kompf.de/tech/images/emtriglevel.png" alt="drawing" width="300"/> (1)  

The resulting limits (orange and yellow) are our high and low trigger borders.  
Set them with `set 90 85`. You will receive an output that your command worked.


## Sources
[1] [Infrarot Lichtschranke mit Arduino zum Auslesen des Stromzählers](https://www.kompf.de/tech/emeir.html)  
[2] Harry20, CC BY-SA 3.0 <http://creativecommons.org/licenses/by-sa/3.0/>, via Wikimedia Commons  
[Git Repo with base program](https://github.com/skaringa/emeir)

