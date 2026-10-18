
const int LM35_PIN1 = 2;
const int LM35_PIN2 = 4;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  int lectura1 = analogRead(LM35_PIN1);
  int lectura2 = analogRead(LM35_PIN2);

  // Sensor 1
  float voltaje1 = lectura1 * 3.3 / 4095.0;
  float milivoltios1 = voltaje1 * 1000.0;
  float temperatura1 = milivoltios1 / 10.0;

  // Sensor 2
  float voltaje2 = lectura2 * 3.3 / 4095.0;
  float milivoltios2 = voltaje2 * 1000.0;
  float temperatura2 = milivoltios2 / 10.0;

  // Mostrar datos del sensor 1
  Serial.println("SENSOR 1");
  Serial.print("Valor ADC: ");
  Serial.println(lectura1);

  Serial.print("Voltaje: ");
  Serial.print(voltaje1);
  Serial.println(" V");

  Serial.print("Milivoltios: ");
  Serial.println(milivoltios1);

  Serial.print("Temperatura: ");
  Serial.print(temperatura1);
  Serial.println(" °C");

  // Mostrar datos del sensor 2
  Serial.println("SENSOR 2");
  Serial.print("Valor ADC: ");
  Serial.println(lectura2);

  Serial.print("Voltaje: ");
  Serial.print(voltaje2);
  Serial.println(" V");

  Serial.print("Milivoltios: ");
  Serial.println(milivoltios2);

  Serial.print("Temperatura: ");
  Serial.print(temperatura2);
  Serial.println(" °C");

  Serial.println("-------------------");

  delay(1000);
}
