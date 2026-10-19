#include <ESP32Servo.h>

Servo miServo;  // Crear un objeto para controlar el servomotor
int pinServo = 9; // Pin digital al que está conectado el cable de señal

void setup() {
  miServo.attach(pinServo);  // Asocia el servo al pin 9
}

void loop() {
  // Mover de 0 a 180 grados
  for (int angulo = 0; angulo <= 180; angulo += 1) {
    miServo.write(angulo);
    delay(15); // Espera 15 ms para suavizar el movimiento
  }

  delay(1000); // Pausa de 1 segundo en la posición 180°

  // Mover de 180 a 0 grados
  for (int angulo = 180; angulo >= 0; angulo -= 1) {
    miServo.write(angulo);
    delay(15);
  }

  delay(1000); // Pausa de 1 segundo en la posición 0°
}