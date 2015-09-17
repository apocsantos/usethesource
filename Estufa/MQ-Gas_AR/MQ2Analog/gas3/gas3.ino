
const int analogInPin = A0;  // Pino analógico de ligação
const int analogOutPin = 9; // Pino digital do led de alarme
 
int sensorValue = 0;        // Valor a ser lido do sensor
int outputValue = 0;        // Valor pwm a se escrito no led
float thickness;
int count,count1;
 
void setup() 
{
  Serial.begin(9600); 
  pinMode(7, OUTPUT);  
}
 
void loop() {
 
  count1++;
  sensorValue = analogRead(analogInPin);            
  outputValue = map(sensorValue, 0, 1023, 0, 255);  
  analogWrite(analogOutPin, outputValue);           
 
  if(count1==30)
  {
    count1=0;
    Serial.print("sensor = " );                       
    Serial.print(sensorValue);      
    Serial.print("\t output = ");      
    Serial.println(outputValue);
 
    thickness = 20000-(5000*(1023/(float)sensorValue)-1);// É preciso corrigir esta equação
 
    Serial.print("Densidade do ar = ");
    Serial.println( thickness);
  }
  count++;
  if(count>=(255-outputValue))
  {
    count=0;
  }
  if(count < (255-outputValue)/2)
  { 
    digitalWrite(7, HIGH);   // set the LED on
    delay(20);
  }
  else
  {
    digitalWrite(7, LOW);    // set the LED off
    delay(20);
  }
}
