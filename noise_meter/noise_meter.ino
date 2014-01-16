#include <SPI.h>
#include <Ethernet.h>

int microphone = 0;  //A0: returns values between 0 and 1023  
int noise = 0;

//Mac address of the ethernet shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x79, 0xC1 };
//Philips Hue server
IPAddress server(10,164,37,209);
int port = 4444;

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

  do_get("/philipshue?foo=bar");  
}

void loop()
{
  noise = getNoiseLevel();    
  Serial.println(noise);   

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

void do_post(String msg){
  Serial.println("doing post");
  
  String postData = msg;


  if (client.connect(server, port)) {
    Serial.println("connected");
    client.println("POST /HueControl HTTP/1.1");
    client.println("Host: 10.164.37.209");
    client.println("User-Agent: Arduino/1.0");
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

