#include "DHT.h"
#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN1 2     // define o pino de dados do sensor, neste caso 2
#define DHTPIN2 7     // define o pino de dados do sensor, neste caso 7
#define DHTTYPE DHT11   // DHT 11 (basta uma constante para dois sensores)

SFE_TSL2561 light;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
int Relay1 = 3; 
int Relay2 = 4;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);

// Initializa DHT.
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

// telnet defaults to port 77
EthernetServer server(77);
boolean alreadyConnected = false; // whether or not the client was connected previously

int ledPin = 2; 

String commandString;

void setup() {
  pinMode(Relay1, OUTPUT);     
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, HIGH); //inicia o relay1 como desligado
  digitalWrite(Relay2, HIGH); //inicia o relay2 como desligado
  dht1.begin(); //inicializa DHT1
  dht2.begin(); //inicializa DHT1
  //light setup ops
  light.begin();
  unsigned char ID;
  if (light.getID(ID))
  {
  }
  else
  {
    byte error = light.getError();
  }
  gain = 0;
  unsigned char time = 2;
  light.setTiming(gain,time,ms);
  light.setPowerUp();
  //end of ligh sensor ops
  
  
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.print("Socket Server:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();    
      commandString = ""; //clear the commandString variable

      server.println("--> Type your command and hit Return... otherwise Fork off!");
      alreadyConnected = true;
    } 

    while (client.available()) {      
      // read the bytes incoming from the client:
      char newChar = client.read();

     if (newChar == 0x0D)  //If a 0x0D is received, a Carriage Return, then evaluate the command
     {
         //server.print("Received this command: ");
         //server.print(commandString);   
         processCommand(commandString);
     } else
     {
       Serial.println(newChar);
         commandString += newChar;
     }

    }
  }
}

void processCommand(String command)
{
  //server.print("Processing command ");
  //server.println(command);

  //dht11-1
    if (command.indexOf("dht1") > -1)
    {
      Serial.print("dht1 command received"); 
      //server.print("Reading from DHT11 Sensor 1: " );  
      float h1 = dht1.readHumidity();
      float t1 = dht1.readTemperature();
      if (isnan(h1) || isnan(t1)) 
      {
        return;
      }    
      //server.print("Humidade ");
      server.println(String (h1) + ";" + String (t1));
      //server.print(";");
      //server.println(String (t1));
      commandString = "";
      return;
  } 
  //dht11-2
    if (command.indexOf("dht2") > -1)
    {
      Serial.println("dht1 command received"); 
      //server.print("Reading from DHT11 Sensor 2: " );
      
      float h2 = dht2.readHumidity();
      float t2 = dht2.readTemperature();
      if (isnan(h2) || isnan(t2)) 
      {
        return;
      }    
      //server.print("Humidade ");
      server.print(String (h2) + ";" + String (t2));
      //server.print(";");
      //server.println(String (t2));
      commandString = "";
      return;
  }
  //light
    if (command.indexOf("light") > -1)
    {
      Serial.print("light command received"); 
      //server.print("Reading from light sensor: " );
      delay(200);
      unsigned int data0, data1;
      if (light.getData(data0,data1))
        {
            double lux;    
            boolean good;  
            //calculo de Lux
            good = light.getLux(gain,ms,data0,data1,lux);
            //server.print("luminosidade em lux ");
            server.print(String (lux));
        }
      commandString = "";
      return;
    }
     //moisture
    if (command.indexOf("satsolo") > -1)
    {
      int humidade = 0;
      Serial.println("satsolo command received"); 
      //server.print("Humidade do solo: " );
      humidade = analogRead(A0); //pino analógico A0
      int Porcento = map(humidade, 1023, 0, 0, 100);
      server.print(String (Porcento));
      commandString = "";
      return;
    }
      
      //onpump
    if (command.indexOf("onpump") > -1)
      {
        digitalWrite(Relay1, LOW);   //Turn ON relay 
        server.println("Ligado" );
        commandString = "";
        return; 
     }
     
      //offpump
      if (command.indexOf("offpump") > -1)
      {  
        server.println("Desligado" );
        digitalWrite(Relay1, HIGH);    //Turn OFF relay 
        commandString = "";
        return;  
      }
      

  commandString = "";
  instructions();
}
//the crap tha was here was removed, cause this is not intended for human usage!!!
void instructions()
{
}
