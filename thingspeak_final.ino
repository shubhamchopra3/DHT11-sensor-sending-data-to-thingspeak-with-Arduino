
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <dht.h>
#define DHT11_PIN 5
dht DHT;




// replace with your channel's thingspeak API key
String apiKey = "C7XPIB1LG6BH6H0C";

// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX

int i=1;


// this runs once
void setup() {                
  // initialize the sensor at 7th pin as an input.
  
  
  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
  ser.begin(9600);
  
  // reset ESP8266
  ser.println("AT+RST");
}


// the loop 
void loop() {
   int chk = DHT.read11(DHT11_PIN);
  //reading the state of sensor1
  
  
  char buf[10];
  char buf1[10];
 float t = (DHT.temperature);
 float h = (DHT.humidity);
 
  String temp =dtostrf(t,0,5,buf);
    String hum =dtostrf(h,0,5,buf1);
  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(temp);
   getStr +="&field2=";
  getStr += String(hum);
 
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}
