
const int pinBuzzer = 5;

void setup() {
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {

  // Encender el buzzer con una frecuencia de 1000 Hz
  ledcAttach(pinBuzzer, 1000, 8);
  ledcWriteTone(pinBuzzer, 1000);

  delay(1000);

  // Apagar el buzzer
  ledcWriteTone(pinBuzzer, 0);

  delay(1000);
}
