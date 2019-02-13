#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"
#include "credentials.h"

#define SERIAL_BAUDRATE                 115200

#define ONBOARDLED                      D4  // wemos pin onboard led

#define LED                             D5  // wemos pin channel 1 LED strip
#define LED2                            D0  // wemos pin channel 2 LED strip


 //pin D4//LED PIN 2
 //pin D0//LED PIN 2
 //pin D5//LED PIN 2

/*`
 * 
 * wemos number = arduino pin
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;//ONBOARD LED PIN
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t RX   = 3;
static const uint8_t TX   = 1;
*/

fauxmoESP fauxmo;

int ledStatus = 0;
int led2Status = 0;

int maxbrightness = 1023;


int midbrightness = 255;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {
    Serial.println("STARTING WIFI SETUP");

    //onboard led off
    digitalWrite(ONBOARDLED, HIGH);
    
    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {

        Serial.print(".");
        delay(100);
    }
    Serial.println();
    Serial.println("CONNECTED");
    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());


    // turn onboard LED on
    digitalWrite(ONBOARDLED, LOW);
}

void setup() {

      // LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    // LED2
    pinMode(LED2, OUTPUT);
    digitalWrite(LED2, LOW);


    // going LOW turns ON onboard blue led
    pinMode(ONBOARDLED, OUTPUT);
    digitalWrite(ONBOARDLED, HIGH);

    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    Serial.println("STARTED AlexaESP8266KitchenSwitch.ino v0.2");


    // Wifi
    wifiSetup();

    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // Add virtual devices
    fauxmo.addDevice("dim kitchen light"); 
    fauxmo.addDevice("dim kitchen lights"); 
    fauxmo.addDevice("kitchen light"); 
    fauxmo.addDevice("kitchen lights"); 
    fauxmo.addDevice("dim lower deck");
    fauxmo.addDevice("lower deck");
    fauxmo.addDevice("dim lights");
    fauxmo.addDevice("all lights");

    // fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
    // this way it's easier to match devices to action without having to compare strings.
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state) {
        //digitalWrite(ONBOARDLED, LOW);
        Serial.println("SetState() called");
      
        Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        if(device_id==0){
          Serial.println("dim kitchen light: " + state);
          led2Status = state;

          if(state){
            analogWrite(LED2, midbrightness);
          }else{
            analogWrite(LED2, 0);
          }
        }else if(device_id==1){
          Serial.println("dim kitchen lights: " + state);
          led2Status = state;

          if(state){
            analogWrite(LED2, midbrightness);
          }else{
            analogWrite(LED2, 0);
          }
        }else if(device_id==2){
          Serial.println("kitchen light: " + state);
          led2Status = state;

          if(state){
            analogWrite(LED2, maxbrightness);
          }else{
            analogWrite(LED2, 0);
          }

        }else if(device_id==3){
          Serial.println("kitchen lights: " + state);
          led2Status = state;

          if(state){
            analogWrite(LED2, maxbrightness);
          }else{
            analogWrite(LED2, 0);
          }
       }else if(device_id==4){
          Serial.println("dim lower deck: " + state);
          ledStatus = state;
          if(state){
            analogWrite(LED, midbrightness);
          }else{
            analogWrite(LED, 0);
          }

       }else if(device_id==5){
          Serial.println("lower deck: " + state);
          ledStatus = state;
          if(state){
            analogWrite(LED, maxbrightness);
          }else{
            analogWrite(LED, 0);
          }
       }else if(device_id==6){
          Serial.println("dim lights: " + state);

          //turn both cannels on/off
          led2Status = state;
          ledStatus = state;
          if(state){
            analogWrite(LED, midbrightness);
            analogWrite(LED2, midbrightness);
          }else{
            analogWrite(LED, 0);
            analogWrite(LED2, 0);
          }
       }else if(device_id==7){
          Serial.println("all lights: " + state);

          //turn both cannels on/off
          led2Status = state;
          ledStatus = state;
          if(state){
            analogWrite(LED, maxbrightness);
            analogWrite(LED2, maxbrightness);
          }else{
            analogWrite(LED, 0);
            analogWrite(LED2, 0);
          }
        }else{
          Serial.println("unknown device");
        }
        
    });

    // Callback to retrieve current state (for GetBinaryState queries)

    fauxmo.onGetState([](unsigned char device_id, const char * device_name) {
        Serial.println("GetState() called");

        if(device_id==0){
          if(led2Status>0){
            return(true);
          }else{
            return(false);
          }
        }else if(device_id==1){
          if(led2Status>0){
            return(true);
          }else{
            return(false);
          }
        }else if(device_id==2){
          if(led2Status>0){
            return(true);
          }else{
            return(false);
          }
        }else if(device_id==3){
          if(led2Status>0){
            return(true);
          }else{
            return(false);
          }          
        }else if(device_id==4){
          if(ledStatus>0){
            return(true);
          }else{
            return(false);
          }
        }else if(device_id==5){
          if(ledStatus>0){
            return(true);
          }else{
            return(false);
          }
        }else if(device_id==6){
          if(ledStatus>0 && led2Status>0){
            return(true);
          }else{
            return(false);
          }
        }else{
          Serial.println("unknown device");
        }
        return false;
    });
}

void loop() {

    fauxmo.handle();

    static unsigned long last = millis();
    
    if (millis() - last > 10000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
        if (WiFi.status() != WL_CONNECTED) {
           // going HIGH turns OFF onboard blue led
          digitalWrite(ONBOARDLED, HIGH);
          Serial.println("NOT CONNECTED");
          wifiSetup();

        }else{
          // going LOW turns OFF onboard blue led
          digitalWrite(ONBOARDLED, LOW);
          Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
        }
    }
}
