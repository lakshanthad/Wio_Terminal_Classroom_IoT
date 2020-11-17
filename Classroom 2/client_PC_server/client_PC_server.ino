#include "rpcWiFi.h" //include Wi-Fi library 
 
const char* ssid = "Enter_WiFI_name"; //Wi-Fi name 
const char* password =  "Enter_WiFi_password"; //Wi-Fi password 

// Use WiFiClient class to create TCP connections
WiFiClient client;
 
void setup() {
    Serial.begin(115200); //start serial communication
    while (!Serial); //wait for Serial to be ready
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
    
    Serial.print("Connected to ");
    Serial.println(ssid); //print Wi-Fi name 
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); //print Wio Terminal's IP address
    Serial.println(""); //print empty line

}
 
void loop() {
    const uint16_t port = 80; //default port of server (80 for http and 443 for https)
    const char* host = "Enter_IP_adress_of_server";  //IP address of server 
 
    Serial.print("Connecting to ");
    Serial.println(host); //print server IP address
    
    //attempt to connect to server
    if (!client.connect(host, port)) {
        Serial.println("Connection failed!");
    } else {
        Serial.println("Connected to server!");
 
    client.print("GET /index.html HTTP/1.1\n\n"); //send HTTP GET request to obtain .html document 
 
    int maxloops = 0; //store maxloops as an integer
 
    //wait for the server's reply to become available
    while (!client.available() && maxloops < 1000) {
        maxloops++;
        delay(1); //1ms delay
    }
    
    if (client.available() > 0) { //condition when server replies
        String line = client.readString(); //read back one line from the server response
        Serial.println(line); //print string from server 
    } else {
        Serial.println("client.available() timed out");
    }
 
    Serial.println("Closing connection.");
    client.stop(); //disconnect from web server 
 
    Serial.println("Waiting 5 seconds before restarting...");
    delay(5000);
}

}
