int humidade;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
void loop()
{
  humidade = analogRead(A0); //pino analógico A0
  int Porcento = map(humidade, 1023, 0, 0, 100);

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
  delay(1000);
}
