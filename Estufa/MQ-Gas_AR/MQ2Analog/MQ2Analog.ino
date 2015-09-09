/* YourDuino.com Electronic Bricks Set - MQ Gas Sensors
See it here: http://yourduino.com/sunshop2/index.php?l=product_detail&p=364
Starter Example: Sketch Template
 - MQ Gas Sensor Brick with an LM393 and both digital and analog outputs.
 - SEE the comments after "//" on each line below
 - CONNECTIONS:
   - GND  - GND
   - +5V  - VCC
   - DOUT - 2
   - AOUT - A0
 - V1.00 04/12/2015
   Questions: terry@yourduino.com */

/*-----( Import needed libraries )-----*/
//NONE YET
/*-----( Declare Constants and Pin Numbers )-----*/
#define LED_PIN 13          // define on-board LED 
#define ANALOG_IN_PIN   A0  // define the analog input pin

/*-----( Declare objects )-----*/
//NONE YET

/*-----( Declare Variables )-----*/
float sensorValue;  // read analoog value

void setup()   /****** SETUP: RUNS ONCE ******/
{
  pinMode (LED_PIN, OUTPUT);      // Onboard LED
  pinMode (ANALOG_IN_PIN, INPUT);// analog  input signal (Not actually required; INPUT is default)
  Serial.begin(9600);              // Start the Serial Monitor connection
  delay(100);
  Serial.println("YourDuino.com MQ Gas Sensor Test ");
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  sensorValue = analogRead(ANALOG_IN_PIN);
  Serial.print("Analog value = ");
  Serial.print(sensorValue), DEC; // display analog value
  Serial.println();
  delay(1000);

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/


//*********( THE END )***********
