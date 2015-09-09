#include "DHT.h"
#include <SparkFunTSL2561.h>
#include <Wire.h>

#define DHTPIN1 2     // define o pino de dados do sensor, neste caso 2
#define DHTPIN2 7     // define o pino de dados do sensor, neste caso 7
#define DHTTYPE DHT11   // DHT 11 (basta uma constante para dois sensores)
//main time delay before first run
#define mainTimeDelay 2000

//light sensor initial code
SFE_TSL2561 light;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
//alterar conforme o sensor que se use (DHT 11, DHT 22 (AM2302), DHT 21 (AM2301) )
// Initializa DHT.
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
//define relé
int Relay1 = 4; 
int Relay2 = 5;

int humidade;

//general setup ops
void setup() 
{
  Serial.begin(9600);
  Serial.println("Serial Port OKay");
  dht1.begin();
  dht2.begin();
 
  
//light setup ops
  light.begin();
  unsigned char ID;
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
  }
  else
  {
    byte error = light.getError();
    printError(error);
  }
  gain = 0;
  unsigned char time = 2;
  Serial.println("Definindo intervalo...");
  light.setTiming(gain,time,ms);
  Serial.println("Power up...");
  light.setPowerUp();
//end of ligh sensor ops   

//relay ops
  pinMode(Relay1, OUTPUT);     //Set Pin4 as output
  pinMode(Relay2, OUTPUT);    //Set Pin5 as output
  digitalWrite(Relay1, HIGH); //inicia o relay1 como desligado
  digitalWrite(Relay2, HIGH); //inicia o relay2 como desligado
}

void loop() 
{
  //delay(mainTimeDelay);
  dhtTemp1();
  dhtTemp2();
  lightSaber();
  moisture();
}

void dhtTemp1()
{
  // Aguarda um pouco antes de iniciar medicoes
  delay(2000); //pode ser um delay inicial 
  
  //as leituras demoram cerca de 250milisegundos, logo as leuituras serão 2 segundos e meio "atrazadas"
  float h = dht1.readHumidity();
  // le a temperatura em graus celcius
  float t = dht1.readTemperature();
  // verifica se alguma leitura falhou e sai para recomecar
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na aquisição de dados do sensor DHT11-1!");
    return;
  }
  float hic = dht1.computeHeatIndex(t, h, false);
  Serial.print("Humidade do ar (sensor1): ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura do ar (sensor1): ");
  Serial.print(t);
  Serial.print(" *C \n");
  //Serial.print("Indice de calor: ");
  //HI = c1+c2T+c3R+C4TR+C5T^2+c6R^2+c7T^2R+c8TR^2+c9T^2R^2
  //ver wikipédia https://en.wikipedia.org/wiki/Heat_index
  //Serial.print(hic);
  //Serial.print(" *C ");
  Serial.println();
}

void dhtTemp2()
{
  // Aguarda um pouco antes de iniciar medicoes
  delay(2000); //pode ser acomulado no delay inicial
  //as leituras demoram cerca de 250milisegundos, logo as leuituras serão 2 segundos e meio "atrazadas"
  float h = dht2.readHumidity();
  // le a temperatura em graus celcius
  float t = dht2.readTemperature();
  // verifica se alguma leitura falhou e sai para recomecar
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na aquisição de dados do sensor DHT11-2!");
    return;
  }
  float hic = dht2.computeHeatIndex(t, h, false);
  Serial.print("Humidade do ar (sensor 2): ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura do ar (sensor 2): ");
  Serial.print(t);
  Serial.print(" *C \n");
  //Serial.print("Indice de calor: ");
  //HI = c1+c2T+c3R+C4TR+C5T^2+c6R^2+c7T^2R+c8TR^2+c9T^2R^2
  //ver wikipédia https://en.wikipedia.org/wiki/Heat_index
  //Serial.print(hic);
  //Serial.print(" *C ");
  Serial.println();
}


void moisture()
{
  humidade = analogRead(A0); //pino analógico A0
  int Porcento = map(humidade, 1023, 0, 0, 100);
  Serial.print(Porcento);
  Serial.println("% humidade do solo");
  if (Porcento <= 10)
  {
    Serial.println("Humidade do solo abaixo dos 70%");
    digitalWrite (Relay1, !digitalRead(Relay1));
    digitalWrite(Relay1, HIGH);    //Turn on relay 
  }
  else
  {
    digitalWrite(Relay1, LOW); //desactuiva o sinal no pino 13 (desliga relé)
    digitalWrite (Relay1, !digitalRead(Relay1));  //Turn off relay
  }
//  delay(1000); //pode e deve ser acomulado no main time delay
}

//light sensor functions:
void printError(byte error)
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}
//funcao principal do sensor de luz
void lightSaber()
{
  delay(ms);
  unsigned int data0, data1;
  if (light.getData(data0,data1))
  {
    /*Serial.print("Luminosidade data0:  ");
    Serial.print(data0);
    Serial.print("\n Luminosidade data1: ");
    Serial.print(data1);
    */
    double lux;    
    boolean good;  
    //calculo de Lux
    good = light.getLux(gain,ms,data0,data1,lux);
    Serial.print(" lux: ");
    Serial.print(lux);
    if (good) Serial.println(" (good) \n"); else Serial.println(" (BAD) \n");
  }
  else
  {
    byte error = light.getError();
    printError(error);
  }
}
