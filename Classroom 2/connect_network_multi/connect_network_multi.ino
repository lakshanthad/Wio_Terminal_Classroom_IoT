//include Wi-Fi libraries
#include "rpcWiFi.h" 
#include "WiFiMulti.h"  
 
WiFiMulti wifiMulti; //initialize library calls
 
void setup() {
    Serial.begin(115200); //start serial communication
    while(!Serial); //wait for Serial to be ready
    delay(100);
    
    WiFi.mode(WIFI_STA); //set WiFi to station mode 
    WiFi.disconnect(); //disconnect from an AP if it was previously connected
    
    //add multiple Wi-Fi networks
    wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");
    wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
 
    Serial.println("Connecting to WiFi.."); //print string
    wifiMulti.run(); //connect to Wi-Fi network
    
    //attempt to connect to Wi-Fi network:
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        // wait 1 second for re-trying
        delay(1000);
    }
        
    Serial.println("WiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); //print Wio Terminal's IP address

}
     
void loop() {

}
