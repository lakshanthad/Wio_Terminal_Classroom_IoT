//include libraries
#include <rpcWiFi.h> //Wi-Fi library 
#include <WiFiClientSecure.h> //WiFiClientSecure library 
#include <ArduinoJson.h> //Arduino Json library 
#include "Free_Fonts.h" //free fonts library 
#include "TFT_eSPI.h" //TFT LCD library  

TFT_eSPI tft; //initialize TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //initialize buffer

const char* ssid     = ""; //WiFi name 
const char* password = ""; //WiFi password 
String apiKey = ""; //YouTube API key
String channelId = ""; //YouTube channel ID

const char*  server = "www.googleapis.com";  //server URL
const char* test_root_ca = \
                            "-----BEGIN CERTIFICATE-----\n"
                            "MIIESjCCAzKgAwIBAgINAeO0mqGNiqmBJWlQuDANBgkqhkiG9w0BAQsFADBMMSAw\n"
                            "HgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEGA1UEChMKR2xvYmFs\n"
                            "U2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0xNzA2MTUwMDAwNDJaFw0yMTEy\n"
                            "MTUwMDAwNDJaMEIxCzAJBgNVBAYTAlVTMR4wHAYDVQQKExVHb29nbGUgVHJ1c3Qg\n"
                            "U2VydmljZXMxEzARBgNVBAMTCkdUUyBDQSAxTzEwggEiMA0GCSqGSIb3DQEBAQUA\n"
                            "A4IBDwAwggEKAoIBAQDQGM9F1IvN05zkQO9+tN1pIRvJzzyOTHW5DzEZhD2ePCnv\n"
                            "UA0Qk28FgICfKqC9EksC4T2fWBYk/jCfC3R3VZMdS/dN4ZKCEPZRrAzDsiKUDzRr\n"
                            "mBBJ5wudgzndIMYcLe/RGGFl5yODIKgjEv/SJH/UL+dEaltN11BmsK+eQmMF++Ac\n"
                            "xGNhr59qM/9il71I2dN8FGfcddwuaej4bXhp0LcQBbjxMcI7JP0aM3T4I+DsaxmK\n"
                            "FsbjzaTNC9uzpFlgOIg7rR25xoynUxv8vNmkq7zdPGHXkxWY7oG9j+JkRyBABk7X\n"
                            "rJfoucBZEqFJJSPk7XA0LKW0Y3z5oz2D0c1tJKwHAgMBAAGjggEzMIIBLzAOBgNV\n"
                            "HQ8BAf8EBAMCAYYwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1Ud\n"
                            "EwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFJjR+G4Q68+b7GCfGJAboOt9Cf0rMB8G\n"
                            "A1UdIwQYMBaAFJviB1dnHB7AagbeWbSaLd/cGYYuMDUGCCsGAQUFBwEBBCkwJzAl\n"
                            "BggrBgEFBQcwAYYZaHR0cDovL29jc3AucGtpLmdvb2cvZ3NyMjAyBgNVHR8EKzAp\n"
                            "MCegJaAjhiFodHRwOi8vY3JsLnBraS5nb29nL2dzcjIvZ3NyMi5jcmwwPwYDVR0g\n"
                            "BDgwNjA0BgZngQwBAgIwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly9wa2kuZ29vZy9y\n"
                            "ZXBvc2l0b3J5LzANBgkqhkiG9w0BAQsFAAOCAQEAGoA+Nnn78y6pRjd9XlQWNa7H\n"
                            "TgiZ/r3RNGkmUmYHPQq6Scti9PEajvwRT2iWTHQr02fesqOqBY2ETUwgZQ+lltoN\n"
                            "FvhsO9tvBCOIazpswWC9aJ9xju4tWDQH8NVU6YZZ/XteDSGU9YzJqPjY8q3MDxrz\n"
                            "mqepBCf5o8mw/wJ4a2G6xzUr6Fb6T8McDO22PLRL6u3M4Tzs3A2M1j6bykJYi8wW\n"
                            "IRdAvKLWZu/axBVbzYmqmwkm5zLSDW5nIAJbELCQCZwMH56t2Dvqofxs6BBcCFIZ\n"
                            "USpxu6x6td0V7SvJCCosirSmIatj/9dSSVDQibet8q/7UK4v4ZUN80atnZz1yg==\n"
                            "-----END CERTIFICATE-----\n";
                            
// Use WiFiClientSecure class to create TCP connections 
WiFiClientSecure client;

//assign data types
int subs; 
int views;
int vids;

void setup() {
  Serial.begin(115200); //start serial communication
  delay(100);

  WiFi.mode(WIFI_STA); //set WiFi to station mode 
  WiFi.disconnect(); //disconnect from an AP if it was previously connected

  Serial.print("Attempting to connect to SSID: "); //print string
  Serial.println(ssid); //print WiFi name 
  WiFi.begin(ssid, password); //connect to WiFi 

  tft.begin(); //start TFT LCD
  tft.setRotation(3); //set TFT rotation
  tft.fillScreen(TFT_BLACK); //fill background color 
  tft.setFreeFont(FMB12); //set font type 
  tft.setCursor((320 - tft.textWidth("Connecting to Wi-Fi.."))/2, 120); //set cursor location 
  tft.print("Connected to Wi-Fi:"); //display text string
  tft.setTextColor(TFT_DARKGREEN); //set text color 
  tft.setCursor((320 - tft.textWidth(ssid))/2, 160);
  tft.print(ssid); //display WiFi name
  delay(2000); //hold on screen 

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }
  
  Serial.print("Connected to ");
  Serial.println(ssid);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor((320 - tft.textWidth("Connected!")) / 2, 120);
  tft.print("Connected!");

  delay(2000);

  tft.setRotation(3);

  //for title
  tft.fillScreen(TFT_DARKGREEN);
  tft.setFreeFont(FF18);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor((320 - tft.textWidth("Seeed Studio on YouTube"))/2, 30);
  tft.print("Seeed Studio on YouTube");

  //for round corner rectangles
  tft.fillRoundRect(10, 45, 300, 55, 5, TFT_WHITE); //draw round corner rectangle with fill color
  tft.fillRoundRect(10, 105, 300, 55, 5, TFT_WHITE);
  tft.fillRoundRect(10, 165, 300, 55, 5, TFT_WHITE);

  //for other text 
  tft.setFreeFont(FS12);
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Subscribers:", 40, 60);
  tft.drawString("Videos:", 40, 120);
  tft.drawString("Total Views:", 40, 180);

}

void loop()
{
  getSum(); //run this function in a loop 
  delay(50);
}

//fucntion to loop 
void getSum() {
  String data; //assign data type 
  client.setCACert(test_root_ca); //initialize root CA 

  Serial.print("Connecting to ");
  Serial.println(server); //print server IP address

  //check whether Wio Terminal is connected to server
  if (!client.connect(server, 443)) { 
    Serial.println("Connection failed!");
  } else {
    Serial.println("Connected to server!");

  //make an HTTP GET request
  client.println("GET https://www.googleapis.com/youtube/v3/channels?part=statistics&id=" + channelId + "&key=" + apiKey + " HTTP/1.1");
  client.println("Host: www.googleapis.com");
  client.println("Connection: close");
  client.println();

  //assign data types and initialize 
  String line,buf="";
  int startJson=0;

  // if there are incoming bytes available from the server, read them 
  while (client.connected() && client.available()){    
    line = client.readStringUntil('\n');
    if(line[0]=='{') startJson=1;
    if(startJson)
    {
      for(int i=0;i<line.length();i++)
        if(line[i]=='[' || line[i]==']') line[i]=' ';
      buf+=line+"\n"; //inputs for Json file
    }
  }
  client.stop(); //disconnect from server

  //ArduinoJson to parse data 
  DynamicJsonDocument doc(1024); //stores a JSON document in memory
  deserializeJson(doc, buf); //parses JSON input and puts the result in the JsonDocument

  //grab items from JsonDocument
  subs = doc["items"]["statistics"]["subscriberCount"]; 
  views = doc["items"]["statistics"]["viewCount"];
  vids = doc["items"]["statistics"]["videoCount"];

  //print values on serial monitor for debugging 
  Serial.println(subs); 
  Serial.println(views);
  Serial.println(vids);

  //sprite buffer for subscribers
  spr.createSprite(100, 20); //create buffer
  spr.fillSprite(TFT_WHITE); //fill backgrlound color of buffer
  spr.setFreeFont(FMB12); //set font type 
  spr.setTextColor(TFT_BLACK); //set text color
  spr.drawNumber(subs, 0, 0); //display number 
  spr.pushSprite(170, 60); //push to LCD 
  spr.deleteSprite(); //clear buffer
    
  //sprite buffer for videos 
  spr.createSprite(100, 20);
  spr.fillSprite(TFT_WHITE);
  spr.setFreeFont(FMB12);
  spr.setTextColor(TFT_BLACK);
  spr.drawNumber(vids, 0, 0); 
  spr.pushSprite(170, 120);
  spr.deleteSprite();

  //sprite buffer for total views 
  spr.createSprite(100, 20);
  spr.fillSprite(TFT_WHITE);
  spr.setFreeFont(FMB12);
  spr.setTextColor(TFT_BLACK);
  spr.drawNumber(views, 0, 0); 
  spr.pushSprite(170, 180);
  spr.deleteSprite();

  }
}
