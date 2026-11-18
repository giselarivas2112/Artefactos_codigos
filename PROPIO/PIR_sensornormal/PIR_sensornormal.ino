const int pinPIR = 4;
int movimiento = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pinPIR, INPUT);
}

void loop() {
  movimiento = digitalRead(pinPIR);

  if (movimiento == HIGH) {
    Serial.println(1);
  } else {
    Serial.println(0);
  }

  delay(500);
}
