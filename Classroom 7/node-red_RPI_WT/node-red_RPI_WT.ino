#include "rpcWiFi.h" //Wi-Fi library
#include <PubSubClient.h> //PubSubClient library
#include "DHT.h" //DHT library
#include "TFT_eSPI.h" //TFT LCD library 
#include "Free_Fonts.h" //free fonts library 

#define DHTPIN 0 //define Signal Pin of DHT
#define DHTTYPE DHT11 //define DHT Sensor Type
DHT dht(DHTPIN, DHTTYPE); //initialize DHT sensor

TFT_eSPI tft; //initialize TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //initialize sprite buffer
 
// Update these corresponding to your network
const char *ssid = "Enter_SSID";      
const char *password = "Enter_Password"; 

const char *ID = "Wio-Terminal-Client";  // Name of our device, must be unique
const char *TOPIC1 = "home/temperature";  // Topic to publish to
const char *TOPIC2 = "home/humidity";  // Topic to publish to
const char *TOPIC3 = "home/lamp";  // Topic to subcribe to
const char *server = "192.168.1.214"; // Server URL
 
//Initializes the wifiClient
WiFiClient wifiClient;
PubSubClient client(wifiClient);

//declare data types
long lastMeasure = 0;
float t;
float h;

// This function is executed when a client publishes a message to a topic that the Wio Terminal is subscribed to 
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;

  String mytopic = String(topic); //conver char to string 
  
  for (int i=0;i<length;i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(mytopic=="home/lamp"){
     Serial.print("Changing Room lamp to ");
     if(messageTemp == "on"){
       digitalWrite(D1, HIGH); //turn ON the LED
       Serial.print("On");
     }
     else if(messageTemp == "off"){
       digitalWrite(D1, LOW); //turn OFF the LED
       Serial.print("off");
     }
  }
  Serial.println();
}

// This function reconnects your Wio Terminal to your MQTT broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      // Subscribe to a topic
      client.subscribe(TOPIC3); //subscribe to topic
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC3);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(D1, OUTPUT); // set D1 pin as OUTPUT for LED
  
  dht.begin(); // start DHT sensor
  
  Serial.begin(115200); //begin serial communication
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connect to Wi-Fi
 
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    WiFi.begin(ssid, password);
    // wait 1 second for re-trying
    delay(1000);
  }
 
  Serial.print("Connected to ");
  Serial.println(ssid);

  //connect to server
  client.setServer(server, 1883);
  client.setCallback(callback);

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

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect(ID);

  long now = millis();
  // Publishes new temperature and humidity every 3 seconds
  if (now - lastMeasure > 3000){
    lastMeasure = now;

    t = dht.readTemperature(); //Assign variable to store temperature
    h = dht.readHumidity(); //Assign variable to store temperature


    //convert float to char for temperature
    static char temp[7];
    dtostrf(t, 6, 2, temp);

    //convert float to char for humidity
    static char humi[7];
    dtostrf(h, 6, 2, humi);

    // Publishes Temperature and Humidity values
    client.publish(TOPIC1, temp);
    client.publish(TOPIC2, humi);
  }
  
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



 
