/*
 * This is a basic example on how to use Espalexa and its device declaration methods.
 */ 
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include <Ticker.h>

#include "my_wifi_ssid_pass.h"    //this is just a header file which contains something like this:
//#define MY_WIFI_SSID  "My Wifi SSID"
//#define MY_WIFI_PASS  "My Wifi Password"

Ticker systick;

// prototypes
boolean connectWifi();

//callback functions
void SpeakersChanged(uint8_t brightness);

const char* ssid = MY_WIFI_SSID;       // your network SSID (name)
const char* password = MY_WIFI_PASS;       // your network password

boolean wifiConnected = false;

Espalexa espalexa;

EspalexaDevice* speakersdevice; //this is optional

void setup()
{
  Serial.begin(115200);

  Serial.println("Speaker controller Started.");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if(wifiConnected){
    
    // Define your devices here. 
    speakersdevice = new EspalexaDevice("Speakers", SpeakersChanged); //you can also create the Device objects yourself like here
    espalexa.addDevice(speakersdevice); //and then add them
    speakersdevice->setValue(255);      //255 is on.
    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }

  //systick.attach(0.1, Systick100mS);
  systick.attach(1, Systick1S);
  
}

//void SetSpeakers(uint8_t brightness) {
//  speakersdevice->setValue(brightness);
//  //SpeakersChanged(brightness);
//}

void  Systick1S() {
  //keep track of how long the speakers have been on and turn them off when its time...
  
}

void loop()
{
   espalexa.loop();
   delay(1);
}

//our callback functions
void SpeakersChanged(uint8_t brightness) {
    Serial.print("SpeakersChanged changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else  {
      Serial.println("OFF");
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
