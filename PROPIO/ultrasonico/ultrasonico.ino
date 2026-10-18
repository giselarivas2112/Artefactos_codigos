#define TRIG_PIN 25
#define ECHO_PIN 26

void setup() {
  Serial.begin(115200);

  //Configurar los pines
  //TRIG es salida porque el ESP32 envía una señal. ECHO es entrada porque el ESP32 recibe la respuesta.
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {

  //Enviar el pulso ultrasónico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  //Medir el tiempo de respuesta
  //El 30000 establece un tiempo máximo de espera de 30 000 microsegundos.

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracion == 0) {
    Serial.println("No se detectó eco");
  } else {
    //v = d / t    →    d = v × t
    //El tiempo que mide el sensor corresponde al recorrido de ida y vuelta del sonido
    //la distancia real hasta el objeto es la mitad de lo que el sonido recorrió en ese tiempo
    float distancia = (0.0343 * duracion) / 2;

    Serial.print("Distancia: ");
    Serial.print(distancia, 1);
    Serial.println(" cm");
  }

  delay(500);
}