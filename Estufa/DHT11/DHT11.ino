//utiliza a lib DHT da Adafruit Industries, disponivel em licenca MIT
#include "DHT.h"
#define DHTPIN 2     // define o pino de dados do sensor, neste caso 2

//alterar conforme o sensor que se use (DHT 11, DHT 22 (AM2302), DHT 21 (AM2301) )
#define DHTTYPE DHT11   // DHT 11

// Initializa DHT.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 checked!");
  dht.begin();
}

void loop() {
  // Aguarda um pouco antes de iniciar medicoes
  delay(2000);

  //as leituras demoram cerca de 250milisegundos, logo as leuituras serão 2 segundos e meio "atrazadas"
  float h = dht.readHumidity();
  // le a temperatura em graus celcius
  float t = dht.readTemperature();
  // verifica se alguma leitura falhou e sai para recomecar
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na aquisição de dados do senso!");
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidade: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  //Serial.print("Indice de calor: ");
  //HI = c1+c2T+c3R+C4TR+C5T^2+c6R^2+c7T^2R+c8TR^2+c9T^2R^2
  //ver wikipédia https://en.wikipedia.org/wiki/Heat_index
  //  Serial.print(hic);
  //Serial.print(" *C ");
  Serial.println();
}
