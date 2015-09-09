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

#include <SFE_BMP180.h> //biblioteca do sensor
#include <Wire.h> //biblioteca de barramento I2C

//cria-se um objecto SFE_BMP180 chamado pressure

SFE_BMP180 pressure;

#define ALTITUDE 305.1 //(1655.0) Altitude (em pés) actual da minha casota nas coordenadas 41.379562N, -8.746893W

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(5000);  // Pause for 5 seconds.
}
