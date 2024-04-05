#include <SPI.h>
#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>
#include "secret.h"

char ssid[] = SECRET_SSID;		// replace MySSID with your WiFi network name              
char pass[] = SECRET_PASS;

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/Sunlight/with/key/c8Fev_l40JnBljOrwJrpSx";
String queryString = "?value1=57&value2=25"; // change your EVENT-NAME and YOUR-KEY
BH1750 lightMeter(0x23);


void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);

  lightMeter.begin();

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  } 
}

/*void loop() {
  if (Serial.read() == 's') {

    // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header


    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
}*/

void loop() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  
  if(lux > 200){
    triggerIFTTT();
    //delay(1000);
    // while(lux > 60) {
    //   //delay(1000);
    //   lux = lightMeter.readLightLevel();
    //}
  //triggerIFTTT();
  }
  else if(lux < 10){
    triggerIFTTT();
  }

  //delay(1000);
  }
}

void triggerIFTTT(){
  if(client.connect(HOST_NAME, 80)){
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    while(client.connected()){
    if(client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    }

    client.stop();

  } else {
    Serial.println("failed to connect");
  }
}





