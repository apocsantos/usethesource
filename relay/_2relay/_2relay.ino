int Relay1 = 2; 
int Relay2 = 3;
void setup() 
{ 
 pinMode(Relay1, OUTPUT);     //Set Pin12 as output
 pinMode(Relay2, OUTPUT);
 digitalWrite(Relay1, HIGH); //inicia o relay1 como desligado
 digitalWrite(Relay2, HIGH); //inicia o relay2 como desligado
} 
void loop() 
{ 
         digitalWrite(Relay1, LOW);   //Turn ON relay 
         delay(2000); 
         digitalWrite(Relay1, HIGH);    //Turn OFF relay 
         delay(2000);
         digitalWrite(Relay2, LOW);   //Turn ON relay 
         delay(2000); 
         digitalWrite(Relay2, HIGH);    //Turn OFF relay 
         delay(2000);
}
