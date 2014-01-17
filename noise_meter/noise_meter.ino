#include <SPI.h>
#include <Ethernet.h>

int microphone = 0;  //A0: returns values between 0 and 1023  
int noise = 0;

//Mac address of the ethernet shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x79, 0xC1 };
//Philips Hue server
//IPAddress server(10,164,37,209);
IPAddress server(10,48,1,158);
//int port = 4444;
int port = 80;

EthernetClient client;


void setup(){
  Serial.begin(9600); 

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  delay(1000);
  Serial.println("connecting...");
  
  do_put("newdeveloper", "1", "{\"hue\": 11730,\"on\": true, \"bri\": 10, \"sat\": 255}");  
}

void loop()
{
  noise = getNoiseLevel();    
  //Serial.println(noise);   

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    for(;;)
      ;
  } 
}


//returns values between 0 and 7
int getNoiseLevel(){
  int analogValue = analogRead(microphone);
  int noiseLevel = analogValue / 10; 
  return noiseLevel;
}


void do_get(String msg){
  Serial.println("doing get");
  if (client.connect(server, port)) {
    Serial.println("connected");
    client.print("GET ");
    client.print(msg);
    client.println(" HTTP/1.0");
    //client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  } 
  else {
    Serial.println("connection failed");
  }
}

void do_post(String username, String id, String msg){
  Serial.println("doing post");
  
  String postData = msg;


  if (client.connect(server, port)) {
    Serial.println("connected");
    client.print("POST ");
    client.print("/api/");
    client.print(username);
    client.print("/lights/");
    client.print(id);
    client.print("/state");
    client.println(" HTTP/1.1");
    client.println("Host: 10.164.37.209");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
  }
  else {
    Serial.println("connection failed");
  }
}


void do_put(String username, String id, String msg){
  Serial.println("doing put");
  
  String postData = msg;


  if (client.connect(server, port)) {
    Serial.println("connected");
    client.print("PUT ");
    client.print("/api/");
    client.print(username);
    client.print("/lights/");
    client.print(id);
    client.print("/state");
    client.println(" HTTP/1.1");
    client.println("Host: 10.164.37.209");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
  }
  else {
    Serial.println("connection failed");
  }
}

