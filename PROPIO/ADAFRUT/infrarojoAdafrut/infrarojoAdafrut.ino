#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ==================================
// CONFIGURACION WIFI
// ==================================

#define WIFI_SSID "TU_WIFI"
#define WIFI_PASSWORD "TU_CONTRASENA"

// ==================================
// CONFIGURACION ADAFRUIT IO
// ==================================

#define AIO_USERNAME "gisgis"
#define AIO_KEY "TU_NUEVA_AIO_KEY"

// Servidor MQTT de Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883

// ==================================
// SENSOR INFRARROJO
// ==================================

const int sensorIR = 2;

// ==================================
// VARIABLES
// ==================================

int contadorPersonas = 0;
int estadoAnterior = HIGH;

unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 5000;

// ==================================
// WIFI Y MQTT
// ==================================

WiFiClient cliente;

Adafruit_MQTT_Client mqtt(
  &cliente,
  AIO_SERVER,
  AIO_SERVERPORT,
  AIO_USERNAME,
  AIO_KEY
);

// Feed de Adafruit IO
Adafruit_MQTT_Publish feedPersonas =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/conteo");

// ==================================
// CONECTAR A WIFI
// ==================================

void conectarWiFi() {

  Serial.print("Conectando al WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
}

// ==================================
// CONECTAR A ADAFRUIT IO
// ==================================

void conectarMQTT() {

  if (mqtt.connected()) {
    return;
  }

  Serial.println("Conectando a Adafruit IO...");

  int8_t resultado;

  while ((resultado = mqtt.connect()) != 0) {

    Serial.println(mqtt.connectErrorString(resultado));

    mqtt.disconnect();

    delay(5000);
  }

  Serial.println("Conectado a Adafruit IO");
}

// ==================================
// SETUP
// ==================================

void setup() {

  Serial.begin(115200);

  pinMode(sensorIR, INPUT_PULLUP);

  conectarWiFi();

  conectarMQTT();

  estadoAnterior = digitalRead(sensorIR);

  Serial.println("Contador de personas iniciado");

  // Enviar el contador inicial
  if (feedPersonas.publish((int32_t)contadorPersonas)) {
    Serial.println("Contador inicial enviado");
  } else {
    Serial.println("Error al enviar el contador inicial");
  }
}

// ==================================
// LOOP
// ==================================

void loop() {

  // Comprobar conexion WiFi
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  // Comprobar conexion MQTT
  conectarMQTT();

  // Leer sensor infrarrojo
  int estadoActual = digitalRead(sensorIR);

  // Contar cuando el sensor pasa de HIGH a LOW
  if (estadoAnterior == HIGH && estadoActual == LOW) {

    contadorPersonas++;

    Serial.print("Personas detectadas: ");
    Serial.println(contadorPersonas);

    // Enviar contador a Adafruit IO
    if (feedPersonas.publish((int32_t)contadorPersonas)) {
      Serial.println("Contador enviado a Adafruit IO");
    } else {
      Serial.println("Error al enviar el contador");
    }

    delay(250);
  }

  // Guardar el estado actual
  estadoAnterior = estadoActual;

  // Mantener la conexion MQTT activa
  mqtt.processPackets(10);

  // Publicar el total cada 5 segundos
  if (millis() - ultimoEnvio >= intervaloEnvio) {

    ultimoEnvio = millis();

    if (feedPersonas.publish((int32_t)contadorPersonas)) {
      Serial.println("Total actualizado en Adafruit IO");
    } else {
      Serial.println("No se pudo actualizar el total");
    }
  }

  delay(20);
}
