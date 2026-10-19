//positivo es el más largo - resistencia de 330 ohms - D32
//Negativo - GND
const int LED = 32;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(1000);

  digitalWrite(LED, LOW);
  delay(1000);
}
