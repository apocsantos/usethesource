//*------------------------Include Libs------------------------*/
#include "ArduinoThread\Thread.h"
#include "ArduinoThread\ThreadController.h"
#include "DHT\DHT.h"
#include "SparkFunTSL2561.h"
#include "Wire\Wire.h"

//*------------------------End of Include Libs------------------------*/

//*------------------------Constants Defenition------------------------*/

#define DHTPIN 2 //define o pino de dados do sensor dht, neste caso o pino D2 (Digital 2)
#define DHTPIN2 7 // define o pino de dados do sensor, neste caso o pino D7 (Digital 7)
#define DHTTYPE DHT11 //define o tipo de sensor DHT, neste caso DHT11. (DHT 11, DHT22 (AM2302), DHT 21 (AM2301) também são suportados)

//*------------------------End of Constant Defenition------------------------*/

//*------------------------Object Instanciation area------------------------*/

SFE_TSL2561 light;

//*------------------------End of Object Instanciation area------------------------*/

//*------------------------Global Variables Declaration------------------------*/

DHT dht1(DHTPIN, DHTTYPE); //inicializa o sensor DHT
DHT dht2(DHTPIN2, DHTTYPE);
int humidadeDoSolo;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

//*------------------------End of Global Variables Declaration------------------------*/

//*------------------------General Scope Functions Declaration------------------------*/
void printError(byte error)
  // If there's an I2C error, this function will
  // print out an explanation.
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
//*------------------------End of General Scope Functions Declaration------------------------*/


//*------------------------Callback functions for "threads" Area------------------------*/

/*------------------------Callback for "thread" 1 named "callBackThreadDHT1" ------------------------*/
void callBackThreadDHT1()
{
	//code to be executed by this thread
	float h = dht1.readHumidity();
	// le a temperatura em graus celcius
	float t = dht1.readTemperature();
	// verifica se alguma leitura falhou e sai para recomecar
	if (isnan(h) || isnan(t)) 
	{
		Serial.println("Falha na aquisição de dados do senso!");
		return;
	}
	float hic = dht1.computeHeatIndex(t, h, false);
	Serial.print("Humidade: ");
	Serial.print(h);
	Serial.print(" %\t");
	Serial.print("Temperatura: ");
	Serial.print(t);
	Serial.print(" *C \n");
	Serial.print("Indice de calor: ");
	//HI = c1+c2T+c3R+C4TR+C5T^2+c6R^2+c7T^2R+c8TR^2+c9T^2R^2
	//ver wikipédia https://en.wikipedia.org/wiki/Heat_index
	Serial.print(hic);
	Serial.print(" *C ");
	Serial.println();
}
/*------------------------End of Callback for "thread" 1 named "callBackThreadDHT1" ------------------------*/

/*------------------------Callback for "thread" 2 named "callBackThreadDHT2" ------------------------*/
void callBackThreadDHT2()
{
	//code to be executed by this thread
	float h = dht2.readHumidity();
	// le a temperatura em graus celcius
	float t = dht2.readTemperature();
	// verifica se alguma leitura falhou e sai para recomecar
	if (isnan(h) || isnan(t)) 
	{
		Serial.println("Falha na aquisição de dados do senso!");
		return;
	}
	float hic = dht2.computeHeatIndex(t, h, false);
	Serial.print("Humidade: ");
	Serial.print(h);
	Serial.print(" %\t");
	Serial.print("Temperatura: ");
	Serial.print(t);
	Serial.print(" *C \n");
	Serial.print("Indice de calor: ");
	//HI = c1+c2T+c3R+C4TR+C5T^2+c6R^2+c7T^2R+c8TR^2+c9T^2R^2
	//ver wikipédia https://en.wikipedia.org/wiki/Heat_index
	Serial.print(hic);
	Serial.print(" *C ");
	Serial.println();
}
/*------------------------End of Callback for "thread" 1 named "callBackThreadDHT2" ------------------------*/


/*------------------------Callback for "thread" 3 named "callBackThreadMoisture1" ------------------------*/
void callBackThreadMoisture1()
{
	//code to be executed by thread2
	humidadeDoSolo = analogRead(A0); //pino analógico A0
	int Porcento = map(humidadeDoSolo, 1023, 0, 0, 100);

	Serial.print(Porcento);
	Serial.println("%");
	if (Porcento <= 70)
	{
		Serial.println("Humidade do solo abaixo dos 70%");
		digitalWrite(13, HIGH);
	}
	else
	{
		digitalWrite(13, LOW);
	}
}
/*------------------------End of Callback for "thread" 3 named "callBackThreadMoisture1" ------------------------*/

/*------------------------Callback for "thread" 4 named "callBackThreadLigh1" ------------------------*/
void callBackThreadLight1()
{
	//code to be executed by thread4
	delay(ms);
	// Retrieve the data from the device:

  unsigned int data0, data1;
  
  if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1); 
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    // Perform lux calculation:
    good = light.getLux(gain,ms,data0,data1,lux);
    // output results:
    Serial.print("lux: ");
    Serial.print(lux);
    if (good) Serial.println(" (Qualidade de luminosidade Boa)"); else Serial.println(" (Qualidade de luminosidade Má)");
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);
  }
}
/*------------------------End of Callback for "thread" 3 named "callBackThreadMoisture1" ------------------------*/

//*------------------------End of Callback functions for "threads" Area------------------------*/

/*-----------------------"Thread" controllers declaration Area------------------------*/

ThreadController pool; //Thread controller named "pool" declaration

/*-----------------------End of "Thread" controllers declaration Area------------------------*/

/*-----------------------"Thread" Declaration Area------------------------*/

//this.threadcontroller threads //another way to declarate threads adding them to the threadcontroller

Thread thread1; //Creates on "thread called thread1 
Thread thread2; //Creates on "thread called thread2
Thread thread3; //Creates on "thread called thread3
Thread thread4; //Creates on "thread called thread4

/*-----------------------End of "Thread" Declaration Area------------------------*/

/*-----------------------Main Setup Function------------------------*/
void setup()
{
	/*-----------------------"Thread's" proprieties setting Area------------------------*/
	
	//set thread proprieties for "thread"1
	thread1.setInterval(2000);
	thread1.onRun(callBackThreadDHT1);
	//set thread proprieties for "thread"2
	thread1.setInterval(2200);
    thread2.onRun(callBackThreadDHT2);
	//set thread proprieties for "thread"3
	thread3.setInterval(2500);
	thread3.onRun(callBackThreadMoisture1);
	//set thread proprieties for "thread"4
	thread4.setInterval(2500);
	thread4.onRun(callBackThreadLight1);

	/*----------------------- End of "Thread's" proprieties setting Area------------------------*/

	/*-----------------------This.ThreadController "Thread's" operations Area------------------------*/
	
	//add threads to threadcontroller
	pool.add(&thread1); //adds by name
	pool.add(&thread2); //adds by name
	pool.add(&thread3); //adds by name
	pool.add(&thread4); //adds by name
	
	/*-----------------------End of This.ThreadController "Thread's" operations Area------------------------*/
	
	/*-----------------------General Setup Code Area------------------------*/
	
	Serial.begin(9600); //115200 may be better if transmiting large ammounts of data
	pinMode(13, OUTPUT); //set pin 13 in output mode

	/*-----------------------End of General Setup Code Area------------------------*/
	
	/*-----------------------Sensor Setup Code Area------------------------*/

	/*DHT sensor 1 setup code*/
	dht1.begin();//starts sensor DHT1 typeof DHT11
	dht2.begin();//starts sensor DHT1 typeof DHT11

	/*Light sensor 1 setup code*/
	light.begin();
	unsigned char ID;

	if (light.getID(ID))
	{}
	// Most library commands will return true if communications was successful,
	// and false if there was a problem. You can ignore this returned value,
	// or check whether a command worked correctly and retrieve an error code:
	else
	{
		byte error = light.getError();
		printError(error);
	}
	// If gain = false (0), device is set to low gain (1X)
	// If gain = high (1), device is set to high gain (16X)
	gain = 0;
	// If time = 0, integration will be 13.7ms
	// If time = 1, integration will be 101ms
	// If time = 2, integration will be 402ms
	// If time = 3, use manual start / stop to perform your own integration

	unsigned char time = 2;
	// setTiming() will set the third parameter (ms) to the
	// requested integration time in ms (this will be useful later):
	//Serial.println("Set timing..."); //just for debug and initialize purposes
	light.setTiming(gain,time,ms);

	// Time to start taking measurements, lets power up the sensor:
	//Serial.println("Power up...");//for debug porpuses only
	light.setPowerUp();

	// The sensor will now gather light during the integration time.
	// After the specified time, you can retrieve the result from the sensor.
	// Once a measurement occurs, another integration period will start.

	/*-----------------------End of Sensor Setup Code Area------------------------*/

}
//*-----------------------End of Main Setup Function------------------------*/

void loop()
{
	pool.run(); //runs all threads in the threadController named pool!
	/* add main program code here */

}
