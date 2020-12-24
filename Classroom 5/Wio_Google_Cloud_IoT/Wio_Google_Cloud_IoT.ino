/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#if defined(ARDUINO_SAMD_MKR1000) or defined(ESP8266)
#define __SKIP_ESP32__
#endif

#if defined(ESP32) || defined(WIO_TERMINAL)
#define __ESP32_MQTT_H__
#endif

#ifdef __SKIP_ESP32__

#include <Arduino.h>

void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.println("Hello World");
}

#endif

#ifdef __ESP32_MQTT_H__
#include "esp32-mqtt.h"
#include "DHT.h" //DHT library
#include "TFT_eSPI.h" //TFT LCD library 
#include "Free_Fonts.h" //free fonts library 

TFT_eSPI tft; //initialize TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //initialize sprite buffer

#define DHTPIN 0 //define Signal Pin of DHT
#define DHTTYPE DHT11 //define DHT Sensor Type
DHT dht(DHTPIN, DHTTYPE); //initialize DHT sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  setupCloudIoT();

  dht.begin(); //Start DHT sensor

  //  ----------------LCD------------------------

  //LCD setup
  tft.begin(); //start TFT LCD
  tft.setRotation(3); //set TFT LCD rotation
  tft.fillScreen(TFT_BLACK); //fill background

  //header title 
  tft.fillRect(0,0,320,50,TFT_YELLOW); //fill rectangle 
  tft.setFreeFont(FSB12); //set font type 
  tft.setTextColor(TFT_BLACK); //set text color
  tft.drawString("WEATHER STATION", 40, 15); //draw string 
    
  //text strings 
  tft.setTextColor(TFT_WHITE); 
  tft.setFreeFont(FS18); 
  tft.drawString("Temperature:", 10,75);
  tft.drawString("Humidity:", 10,175);
  
  tft.setTextColor(TFT_YELLOW); 
  tft.setFreeFont(FMB24);  
  tft.drawString("C",260,70);
  tft.drawString("%RH", 215,170);
  
  tft.drawFastHLine(0,140,320,TFT_YELLOW); //draw horizontal line

}

unsigned long lastMillis = 0;
void loop() {
  int temperature = dht.readTemperature(); //Assign variable to store temperature
  int humidity = dht.readHumidity(); //Assign variable to store humidity

  
  mqtt->loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqttClient->connected()) {
    connect();
  }

  // TODO: replace with your code
  // publish a message roughly every second.
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    //publishTelemetry(mqttClient, "/sensors", getDefaultSensor());

    String payload = String("{\"timestamp\":") + getNTPtime() +
                     String(",\"temperature\":") + temperature +
                     String(",\"humidity\":") + humidity +
                     String("}");
                
    publishTelemetry(payload);

  }

  //sprite buffer for temperature
  spr.createSprite(55, 40); //create buffer
  spr.fillSprite(TFT_BLACK); //fill background color of buffer
  spr.setFreeFont(FMB24); //set font type 
  spr.setTextColor(TFT_YELLOW); //set text color
  spr.drawNumber(temperature, 0, 0); //display number 
  spr.pushSprite(200, 70); //push to LCD 
  spr.deleteSprite(); //clear buffer

  //sprite buffer for humidity 
  spr.createSprite(55, 40);
  spr.fillSprite(TFT_BLACK);
  spr.setFreeFont(FMB24);
  spr.setTextColor(TFT_YELLOW);
  spr.drawNumber(humidity, 0, 0); 
  spr.pushSprite(155, 170);
  spr.deleteSprite();

  
}
#endif
