//include libraries
#include "rpcWiFi.h" //Wi-Fi library 
#include "DHT.h" //DHT library
#include "TFT_eSPI.h" //TFT LCD library 
#include "Free_Fonts.h" //free fonts library 

#include "DNSServer.h"
#include "WebServer.h"
#include "WiFiManager.h" 

TFT_eSPI tft; //initialize TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //initialize sprite buffer

#define DHTPIN 0 //define Signal Pin of DHT
#define DHTTYPE DHT11 //define DHT Sensor Type
DHT dht(DHTPIN, DHTTYPE); //initialize DHT sensor
 
const char* serverName = "io.adafruit.com"; //Domain Name for HTTP POST Request
String AIOKey = "aio_jcYG00EN7hL1HGvjjC0MyOy2mMyl"; //

// Use WiFiClient class to create TCP connections
WiFiClient client;
 
void setup() {
  Serial.begin(115200); //start serial communication
  while (!Serial); //wait for Serial to be ready
  delay(100);

  WiFiManager wifiManager; //initialize library     
  wifiManager.resetSettings(); //reset saved settings
  wifiManager.autoConnect("AutoConnectAP"); //starts an access point 

  WiFi.begin(); //connect to Wi-Fi network
 
  // attempt to connect to Wi-Fi network:
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      // wait 1 second for re-trying
      delay(1000);
  }
    
  Serial.print("Connected to WiFi ");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); //print Wio Terminal's IP address
  Serial.println(""); //print empty line

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
 
void loop() {
  Serial.print("Connecting to ");
  Serial.println(serverName); //print server IP address

  //check whether Wio Terminal is connected to server
  if (!client.connect(serverName, 80)) {  //80 is default port of HTTP
      Serial.println("Connection failed!");
  } else {
      Serial.println("Connected to server!");

  int t = dht.readTemperature(); //Assign variable to store temperature
  int h = dht.readHumidity(); //Assign variable to store humidity

  //data needed to send along with API key
  String tempStr;
  tempStr +="value=";
  tempStr += String(t);
  String humiStr;
  humiStr +="value=";
  humiStr += String(h);

  //make an HTTP POST request
  client.println("POST /api/v2/lakshanthad/feeds/temperature/data HTTP/1.1");
  client.println("Host: io.adafruit.com");
  client.println("Connection: keep-alive");
  client.println("X-AIO-Key: aio_jcYG00EN7hL1HGvjjC0MyOy2mMyl");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(tempStr.length());
  client.println();
  client.println(tempStr);
 
  //make an HTTP POST request
  client.println("POST /api/v2/lakshanthad/feeds/humidity/data HTTP/1.1");
  client.println("Host: io.adafruit.com");
  client.println("Connection: close");
  client.println("X-AIO-Key: aio_jcYG00EN7hL1HGvjjC0MyOy2mMyl");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(humiStr.length());
  client.println();
  client.println(humiStr);

  //display sensor data on serial monitor 
  Serial.print("temperature: "); //print string
  Serial.println(t); //print data stored in t
  Serial.print("Humidity:");
  Serial.println(h);
                             
  Serial.println("Sent to Adafruit IO");

      
  //sprite buffer for temperature
  spr.createSprite(55, 40); //create buffer
  spr.fillSprite(TFT_BLACK); //fill background color of buffer
  spr.setFreeFont(FMB24); //set font type 
  spr.setTextColor(TFT_YELLOW); //set text color
  spr.drawNumber(t, 0, 0); //display number 
  spr.pushSprite(200, 70); //push to LCD 
  spr.deleteSprite(); //clear buffer

  //sprite buffer for humidity 
  spr.createSprite(55, 40);
  spr.fillSprite(TFT_BLACK);
  spr.setFreeFont(FMB24);
  spr.setTextColor(TFT_YELLOW);
  spr.drawNumber(h, 0, 0); 
  spr.pushSprite(155, 170);
  spr.deleteSprite();
  
  }
  
  client.stop(); //disconnect from server
  //wait 3 seconds to send the next POST request
  Serial.println("Waiting...");
  delay(3000);
}
