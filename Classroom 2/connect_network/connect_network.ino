#include "rpcWiFi.h" //include Wi-Fi library 
 
const char* ssid = "Enter_WiFI_name"; //Wi-Fi name 
const char* password =  "Enter_WiFi_password"; //Wi-Fi password 
 
void setup() {
    Serial.begin(115200); //start serial communication
    while(!Serial); //wait for Serial to be ready
    delay(100);
 
    WiFi.mode(WIFI_STA); //set WiFi to station mode 
    WiFi.disconnect(); //disconnect from an AP if it was previously connected
 
    Serial.println("Connecting to WiFi.."); //print string 
    WiFi.begin(ssid, password); //connect to Wi-Fi network
 
    // attempt to connect to Wi-Fi network:
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        // wait 1 second for re-trying
        delay(1000);
    }
    
    Serial.print("WiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); //print Wio Terminal's IP address

}
 
void loop() {
 
}
