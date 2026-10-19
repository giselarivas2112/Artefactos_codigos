//cada color - resistencia 330 ohms - pin digital
//catodo es el más largo - GND
const int ROJO = 27;
const int VERDE = 26;
const int AZUL = 25;

void setup() {
  pinMode(ROJO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
}

void loop() {
  color(255, 0, 0);       // Rojo
  delay(1000);

  color(255, 255, 0);     // Amarillo
  delay(1000);

  color(180, 0, 255);     // Morado
  delay(1000);

  color(0, 255, 180);     // Turquesa
  delay(1000);

  color(255, 255, 255);   // Blanco
  delay(1000);

  color(255, 116, 180);   // Rosado pastel
  delay(1000);
}

void color(int r, int v, int a) {
  analogWrite(ROJO, r);
  analogWrite(VERDE, v);
  analogWrite(AZUL, a);
}
