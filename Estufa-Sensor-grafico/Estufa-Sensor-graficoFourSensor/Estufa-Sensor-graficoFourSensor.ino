#include "DHT.h"
#include <SparkFunTSL2561.h>
#include <Wire.h>

/*
 * ligações: DHT11:
 * Data pin ao DHTPIN correspondente
 * VCC -> 3b3
 * GND -> GND
 * ------------------------------------
 * Pin Analogico do sensor -> Pin analogico do arduino a ser utilizado
 * VCC -> 3v3
 * GND -> GND
 * 
 * ----------------------------------------------------
 * Sensor de Luminosidade TSL2561
 * 3v3 -> 3v3
 * GND -> GND
 * SCL - A5
 * SDA -> A4
 * 
*/

#define DHTPIN1 2     // define o pino de dados do sensor, neste caso 2
#define DHTPIN2 7     // define o pino de dados do sensor, neste caso 7
#define DHTTYPE DHT11   // DHT 11 (basta uma constante para dois sensores)
#define mainTimeDelay 2000
SFE_TSL2561 light;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
//alterar conforme o sensor que se use (DHT 11, DHT 22 (AM2302), DHT 21 (AM2301) )
// Initializa DHT.
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
int humidade;
//general setup ops
void setup() 
{
  Serial.begin(115200);
  dht1.begin();
  dht2.begin(); 
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
}

void loop() 
{
//  delay(mainTimeDelay);
  dhtTemp1();
  dhtTemp2();
  moisture();
  lightSaber();

}

void dhtTemp1()
{
  delay(2000); //pode ser um delay inicial 
  float h = dht1.readHumidity();
  float t = dht1.readTemperature();
  if (isnan(h) || isnan(t)) 
  {
    return;
  }
  float hic = dht1.computeHeatIndex(t, h, false);
  Serial.print(t);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
//  Serial.println();
}

void dhtTemp2()
{
  // Aguarda um pouco antes de iniciar medicoes
  //delay(2000); //pode ser acomulado no delay inicial
  float h = dht2.readHumidity();
  float t = dht2.readTemperature();
  if (isnan(h) || isnan(t)) 
  {
      return;
  }
  float hic = dht2.computeHeatIndex(t, h, false);
  Serial.print(t);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
//  Serial.println();
}


void moisture()
{
  humidade = analogRead(A0); //pino analógico A0
  int Porcento = map(humidade, 1023, 0, 0, 100);
  Serial.print(Porcento);
  Serial.print(",");
  //Serial.println("% humidade do solo");
  if (Porcento <= 10)
  {
    digitalWrite(13, HIGH); //activa o sinal no pino 13 (liga relé)
  }
  else
  {
    digitalWrite(13, LOW); //desactuiva o sinal no pino 13 (desliga relé)
  }
//  delay(1000); //pode e deve ser acomulado no main time delay
}

//light sensor functions:
void printError(byte error)
{
}
//funcao principal do sensor de luz
void lightSaber()
{
  delay(ms);
  unsigned int data0, data1;
  if (light.getData(data0,data1))
  {
    double lux;    
    boolean good;  
    //calculo de Lux
    good = light.getLux(gain,ms,data0,data1,lux);
    Serial.println(lux);
//    Serial.println();
  }
}
