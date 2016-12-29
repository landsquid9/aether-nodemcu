# aether-nodemcu

This is a library for use with the NodeMCU development board and the Arduino IDE. It provides an interface for connecting to the Aether IOT Art platform: http://aether.mod.bz

At this stage it is very much Work In Progress

Requirements:
1. A NodeMCU: http://www.nodemcu.com/index_en.html
    These are cheap, WiFi enabled microcontrollers. Traditionally you would use Lua scripts to program them, but some clever folks have       integrated it with the Arduino IDE, which brings us to ....
2. The Arduino ESP8266 Core: https://github.com/esp8266/Arduino
    Make sure to install the Arduino IDE, and then collow the instructions in the above link.
3. The Arduino Websocket Library: https://github.com/Links2004/arduinoWebSockets
    To ensure fast transfer of data, Aether requires you communicate with it via websockets.
    
Installation:
1. Install git: https://git-scm.com/book/en/v2/Getting-Started-Installing-Git
2. Clone aether-nodemcu into your arduino library folder: https://git-scm.com/book/en/v2/Git-Basics-Getting-a-Git-Repository

Example:
Here is some example code. It is included in the library's example folder.

There are few things to note:
1. By default, the log level is set to standard. The library does not initialise serial communication, however. You must either initialise the serial logger (Serial.begin(baudrate)) or set the log level to LOG_NONE.
2. 
