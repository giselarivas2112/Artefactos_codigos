//una pata al 3V3
//Otra pata en una misma fila - Pin ADC y resistencia  10K ohms - de la pata de la resistencia GND
const int LDR_PIN = 35;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int luz = analogRead(LDR_PIN);

  int porcentaje = map(luz, 0, 4095, 0, 100);

  Serial.print("Valor del sensor: ");
  Serial.println(luz);

  Serial.print("Porcentaje de luz: ");
  Serial.print(porcentaje);
  Serial.println("%");

  delay(500);
}
