/*

Este sensor mede a pressão absoluta (pressão ambiente actual), que varia de acordo com a meteorologia e a altitude
Antes de ler a pressão, é necessário ler a temperatura. Isto é feito usando as funções startTemperature() e getTemperature(), que devolvem o resultado (float) em graus celcius

Uma vez obtida a temperatura, pode-se começar a ler a pressão barométrica usando as funções startPressure() e getPressure. O resultado será em milibares, (hectopascai hPa)
Para se defenir a altutide relativa ao NMAM (Nível Médio das Águas do Mar) da cota actual, usa-se a função sealevel(). Esta altitude deve ser obtida usando por exemplo um equipamento de GPS, ou um smartphone.
Para se calcular a altitude, é necessário criar uma linha base de altitude. Para isto pode-se utilizar uma média de pressão ao NMAM, ou uma leitura de altitude anterior. Neste caso usar-se ia a função (altitude).

Wiring: 
- (GND) to GND
+ (VDD) to 3.3V

ATENCAO: NÃO LIGAR O SENSOR A 5VDC, CASO CONTRARIO AVARIA.

Também vão ser necessários os pinos de barramento I2C (SCL e SDA), que variam de circuito para circuito. Ver tabela abaixo para arduino.

Arduino pins            :  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3

O pino VDDIO fica livre, pois apenas é usado em circuitos lógicos de 1.8V

Bibliotecas usadas: 

SFE_BMP180 

Equações utilizadas: Equações de vigula flutuante, desenvolvidas pelo projeto Weather Station Data Logger: http://wmrx00.sourceforge.net/

Baseado no trabalho de Grusin 2013 e Arduino Comunityu 2015, adaptado por Apocsantos

ILM: "My four legged princess... Dady misses you a bunch"
*/

#include <SFE_BMP180.h> //biblioteca do sensor
#include <Wire.h> //biblioteca de barramento I2C

//cria-se um objecto SFE_BMP180 chamado pressure

SFE_BMP180 pressure;

#define ALTITUDE 98.0 //(1655.0) Altitude (em pés) actual da minha casota nas coordenadas 41.379562N, -8.746893W

void setup()
{
  Serial.begin(115200);
  if (pressure.begin())
    delay(100);
  else
  {
    while(1); // Pausa.
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Aguarda o resultado da leitura
    delay(status);
    //Note to Self: Implementar try catch era interessante
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Imprime na porta série o valor de temperatura:
      //Serial.print("temperatura: ");
      Serial.print(T,2);
      Serial.print(",");
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
         // Serial.println(P*0.0295333727,2);
          delay(250);
          // Print out the measurement:
          p0 = pressure.sealevel(P,ALTITUDE); // Estou a 92metros, e altura e a vários cafés de pressao!
          Serial.println(p0,2);
        }
       }
    }
  }
  delay(5000);  // Pause for 5 seconds.
}
