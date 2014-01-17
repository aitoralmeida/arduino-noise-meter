#include <SPI.h>
#include <Ethernet.h>

//Colors
String BLUE = "46920";
String GREEN = "25500";
String YELLOW = "18000";
String ORANGE = "11000";
String RED = "0";

//Msg
String MSG_START = "{\"hue\": ";
String MSG_END = ",\"on\": true, \"bri\": 10, \"sat\": 255}";


int microphone = 0;  //A0:  theoretically returns values between 0 and 1023, actually it is between 300 and 700. 
int accumulatedMeassures = 0;
int totalMeassures = 0;
int accumulatedLevel = 0;
int counter = 0;

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
  
  //check for correct initialization
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  delay(1000);
  
 
  //Initialize it to pink
  Serial.println("Initializing to pink");
  do_put("newdeveloper", "1", "{\"hue\": 56100,\"on\": true, \"bri\": 10, \"sat\": 255}");  
                    
}

void loop()
{
  delay (10);  
  
  //meassures taken for 1 second, calculate avg and change lightbulb color
  if (counter == 100){ 
    
    int avgMeassure = accumulatedMeassures / totalMeassures;
    int level = getNoiseLevel(avgMeassure);
    String color = toColor(level);
    
    //create the msg with the color
    String msg = MSG_START + color;
    msg += MSG_END;
    Serial.print("New msg: ");
    Serial.println(msg);
    
    //change color
    do_put("newdeveloper", "1", msg); 
    
    //restart
    counter = 0;
    totalMeassures = 0;
    accumulatedMeassures = 0;    
  } 
  //continue taking meassures
  else { 
    int noiseMeassure = getAnalogValue();   
    //if the microphone is no acting funny (note to self, buy a better microphone)
    if (noiseMeassure != 0){
      totalMeassures++;
      accumulatedMeassures += noiseMeassure;
    }
    
    counter++;
  }
  
}

//normalizes the microphone noise value
int getAnalogValue(){
  int ANALOG_BASE = 300; //base noise level for the used microphone
  int analogValue = analogRead(microphone);
  //sometimes the use microphone returns 0 :-/
  if (analogValue <= ANALOG_BASE){
    analogValue = ANALOG_BASE;
  }
  analogValue = analogValue - ANALOG_BASE;
  return analogValue;
}

//returns values between 0 and 4
int getNoiseLevel(int analogValue){
  int noiseLevel = analogValue / 100; //five noise leves [0..4]. analogValue takes values [0..400] (700-300)
  return noiseLevel;
}

//gives the color for each level
String toColor(int level){
  String color = "";
  if (level == 0){
    color = BLUE;
  } else if (level == 1){
    color = GREEN;
  } else if (level == 2){
    color = YELLOW;
  } else if (level == 3){
    color = ORANGE;
  } else if (level == 4){
    color = RED;
  }
  
  return color;
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
    Serial.println("sent");
    client.stop();
  }
  else {
    Serial.println("connection failed");
  }
}

