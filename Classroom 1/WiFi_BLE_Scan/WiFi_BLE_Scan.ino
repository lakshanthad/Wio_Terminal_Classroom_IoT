//include libraries
#include "rpcWiFi.h" //Wi-Fi library
#include "rpcBLEDevice.h" //bluetooth library 
#include "BLEScan.h" //bluetooth library 
#include "BLEAdvertisedDevice.h" //bluetooth library

int scanTime = 5; //scan time for bluetooth 

BLEScan* pBLEScan; //create buffer to allocate memory

//get MAC addresses of bluetooth devices available 
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks{
  void onResult(BLEAdvertisedDevice advertisedDevice){
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str()); //print MAC addresses of bluetooth devices
  }
};

void setup() {
  Serial.begin(115200); //start serial communication
  while(!Serial); //wait until serial monitor/ serial port is opened 
  delay(100);

  //Wi-Fi setup
  WiFi.mode(WIFI_STA); //configure Wi-Fi as Station(STA) Mode
  WiFi.disconnect(); //disconnect from an AP(access point) if it was previously connected

  //Bluetooth setup
  BLEDevice::init(""); //initialize bluetooth
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //set device callbacks 
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); //scan interval 
  pBLEScan->setWindow(99); //less than or equal to setInterval value
}

void loop() {
  //Wi-Fi scan
  Serial.println("Wi-Fi scan start");
  int n = WiFi.scanNetworks(); //return the number of Wi-Fi networks found and store in variable

  if (n == 0){
    Serial.println("no networks found"); //print string if no networks found
  } else{
    Serial.print(n); //print the number of networks found 
    Serial.println(" network(s) found");
    for (int i = 0; i < n; ++i) { //iterate the network found in the loop
        //print SSID and RSSI for each network found 
        Serial.print(i + 1); //found network number 
        Serial.print(": ");
        Serial.print(WiFi.SSID(i)); //print Wi-Fi name 
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i)); //print Wi-Fi signal strength
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*"); //print "*" if password protected & print nothing if open
        delay(10);
    }
  }

  Serial.println("Wi-Fi scan done!");
  Serial.println(""); //print an empty line

  //Bluetooth scan 
  Serial.println("BLE scan start");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false); //scan for bluetooth devices and print MAC address
  Serial.print(foundDevices.getCount()); //print number of bluetooth devices found
  Serial.println(" BLE device(s) found");
  Serial.println("BLE scan done!");
  pBLEScan->clearResults(); //delete results from pBLEScan buffer to release memory
  Serial.println("");

  delay(1000); //wait before scanning again



  






  

}
