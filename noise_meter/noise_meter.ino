int microphone = 0;  //A0: returns values between 0 and 1023  
int noise = 0;

//returns values between 0 and 7
int getNoiseLevel(){
  int analogValue = analogRead(microphone);
  int noiseLevel = analogValue / 128; // 1024/8 (number of rows in the led matrix)
  return noiseLevel;
}

void setup()
{
  Serial.begin(9600);        
}

void loop()
{
  noise = getNoiseLevel();    
  Serial.println(noise);             
}
