#include <timer.h>      //global:  Ticker systick;  setup:  systick.attach(1, Systick1S);  callback: Systick1S.    (No loop handler needed)

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

#define   SPEAKER_MAX_ON_TIME     (8*60*60)   //8 hours in seconds

const int speakerPin = 15; 
const int ledPin = LED_BUILTIN;


Ticker systick;

// prototypes
boolean connectWifi();

//callback functions
void SpeakersChanged(uint8_t brightness);

const char* ssid = MY_WIFI_SSID;       // your network SSID (name)
const char* password = MY_WIFI_PASS;       // your network password

boolean wifiConnected = false;

uint32_t SpeakersOnDuration = 0;

Espalexa espalexa;

EspalexaDevice* speakersdevice; //this is optional

void setup()
{
  Serial.begin(115200);

  Serial.println("Speaker controller Started.");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);

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
  systick.attach(1, Systick1S);         //ticker.h

  //timer.every(100, TimerCallback);      //timer.h
}

//void SetSpeakers(uint8_t brightness) {
//  speakersdevice->setValue(brightness);
//  //SpeakersChanged(brightness);
//}

//bool TimerCallback(void) {
//  return true;    //must return true to keep the timer alive
//}

void  Systick1S() {
  //keep track of how long the speakers have been on and turn them off when its time...

  if (digitalRead(speakerPin)) {
    SpeakersOnDuration++;
    if (SpeakersOnDuration >= SPEAKER_MAX_ON_TIME) {
      SpeakersOnDuration = 0;
      TurnSpeakersOff();
    }
  }

//  //toggle the LED to show 1 second tick rate:
//  if (digitalRead(ledPin)) digitalWrite(ledPin, LOW);
//  else digitalWrite(ledPin, HIGH);
  
}

void loop()
{
   espalexa.loop();
   delay(1);
}

//Alexa callback function:
void SpeakersChanged(uint8_t brightness) {
    if (brightness) {
      TurnSpeakersOn();
    }
    else  {
      TurnSpeakersOff();
    }
}

void TurnSpeakersOn(void) {
  SpeakersOnDuration = 0;
  Serial.println("speakers turned ON");
  digitalWrite(ledPin, HIGH);
  digitalWrite(speakerPin, HIGH);
}

void TurnSpeakersOff(void) {
  SpeakersOnDuration = 0;
  Serial.println("speakers turned OFF");
  digitalWrite(ledPin, LOW);
  digitalWrite(speakerPin, LOW);
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
