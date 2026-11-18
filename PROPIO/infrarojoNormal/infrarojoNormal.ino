const int sensorIR = 4;

void setup() {
    Serial.begin(115200);
    pinMode(sensorIR, INPUT);
}

void loop() {
    int valor = digitalRead(sensorIR);

    Serial.println(valor);

    delay(500);
}