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
  //Serial.println("Re-inicia");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    //Serial.println("Inicializar sensor BMP180");
    delay(100);
  else
  {
    //Serial.println("Falha a iniciar o sensor BMP180 \n\n");
    while(1); // Pausa.
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  // Lê a pressão a cada 10 segundos.
  //Serial.println();
  //Serial.print("Altitude (Inserida manualmente): ");
  //Serial.print(ALTITUDE,0);
  //Serial.print(" metros, ");
  //não sou aviador, mas fica o calculo para altitude em pés: (Serial.print(ALTITUDE*3.28084,0); Serial.println("pés");)
  
  // Faz a medição da temperatura (valor necessário para o calculo da pressão baromtétrica)
  // Se a leitura foi efectuada com sucesso, um valor é retornado
  // If request is successful, the number of ms to wait is returned.
  // caso contrario é retornado zero
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Aguarda o resultado da leitura
    delay(status);
    //Devolve uma leitura de tempratura, que é armazenada na variavel T.
    //Retorna 1 se a funcao foi executada correctamente e zero se falhou
    //Note to Self: Implementar try catch era interessante

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Imprime na porta série o valor de temperatura:
      //Serial.print("temperatura: ");
      //Serial.print(T,2);
      //Serial.print(" deg C, ");
      //Em Farenhieght se formos usar terminologia americana
      /*
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      */

      //Inicia a medicao da pressao atmosférica
      //É enviado um parametro de oversampling entre zero e tres, conforme a resolucao pertendida (maior ou menor)
      //Se a funcao for executada com sucesso, retorna um valor em ms
      //caso contrario, retorna zero
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Aguarda o fim da medicao
        delay(status);
        //Devolve a medicao de pressao cmpleta.
        //O Valor da pressão é armazenado na variavel P. O valor da temperatura medida anteriormente, na variável T (valor necessário para o calculo).
        //Em ambientes de temperatura fixa, é desnecessário efectuar a medição de temperatura constantemente. Pode-se usar uma constante, ou utilizar uma temperatura vinda de outro sensor, tipo um DHT11.
        //Como em todas as funcoes anteriores, retorna 1 se a funcao executou com sucsso, e zero se falhou.
        //Note to Self: Mais um bom local para se usar um try catch e dar alguma informacao sobre o erro em caso de erro.
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          Serial.print(T);
          Serial.print(" , ");
          Serial.println(P*0.0295333727,2);
          delay(250);
          // Print out the measurement:
          /*Serial.print("Pressao Absoluta em mb: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
          // Usa-se a constante ALTITUDE anteriormetne declarada, para remover a variaçao da pressão derivada da altitude
          // Parameteros: P = pressao absoluta em mb, ALTITUDE = altitude actual em metros.
          // Result: p0 = pressao compensada ao NMAM em mb
          p0 = pressure.sealevel(P,ALTITUDE); // Estou a 92metros, e altura e a vários cafés de pressao!
          Serial.print("Pressao relativa ao NMAM: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");
          */
        }
        else Serial.println("Ocorreu um erro na medicao Cod-1\n");
      }
      else Serial.println("Ocorreu um erro na medicao Cod-2\n");
    }
    else Serial.println("Ocorreu um erro na medicao Cod-3\n");
  }
  else Serial.println("Ocorreu um erro na medicao Cod-4\n");

  delay(5000);  // Pause for 5 seconds.
}
