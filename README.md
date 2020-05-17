# SpeakerController
ESP32 or ESP8266 Alexa controlled Relay with OTA FW update written in Arduino.

I developed this to turn on and off my outdoor speakers via a relay circuit.  When turned on, the relay will automatically turn back off again after 8 hours.
The built-in LED is used to identify whether the speakers are on or off.

This uses Espalexa to create a "Light" type device that Alexa can control.  

To add the device to Alexa:
Add a new device in the Alexa App and select Light, then the select the brand as "Other".  The device will be automatically found as a "Royal Phillips" Light called "Speakers".

Turn the speakers on and off by saying "Alexa turn speakers on" or "Alexa turn speakers off".

The device supports ArduinoOTA, so in the Arduino app, simply select Tools > Port.  There should be a Network port listed with the device IP address. The device can be updated wirelessly using this interface.
