const int LM35_PIN1 = 2;
const int LM35_PIN2 = 4;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Leer directamente el voltaje en milivoltios
  float milivoltios1 = analogReadMilliVolts(LM35_PIN1);
  float milivoltios2 = analogReadMilliVolts(LM35_PIN2);

  // Convertir milivoltios a grados Celsius
  float temperatura1 = milivoltios1 / 10.0;
  float temperatura2 = milivoltios2 / 10.0;

  Serial.print("Temperatura sensor 1: ");
  Serial.print(temperatura1);
  Serial.println(" °C");

  Serial.print("Temperatura sensor 2: ");
  Serial.print(temperatura2);
  Serial.println(" °C");

  Serial.println("-------------------");

  delay(1000);
}