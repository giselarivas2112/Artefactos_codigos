#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       ""
#define WLAN_PASS       ""

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    ""
#define AIO_KEY         ""

const int TRIGER_PIN = 26;
const int ECHO_PIN = 25;
const int LED_PIN = 12;

bool ledPorDistancia = false;
bool ledManual = false;

const float UMBRAL_CM = 20.0;

const unsigned long INTERVALO_MEDICION = 500;
const unsigned long INTERVALO_PUBLICACION = 5000;
const unsigned long INTERVALO_PING = 60000;

unsigned long ultimaMedicion = 0;
unsigned long ultimaPublicacion = 0;
unsigned long ultimoPing = 0;

float ultimaDistancia = -1.0;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish distancia_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/distancia");
Adafruit_MQTT_Subscribe led_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/boton-led");

void actualizarLed() {
  if (ledPorDistancia || ledManual) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void ledCallback(char *data, uint16_t len) {
  Serial.print(F("Mensaje recibido desde Adafruit: "));
  Serial.println(data);

  if (strcmp(data, "ON") == 0 || strcmp(data, "1") == 0) {
    ledManual = true;
  } else if (strcmp(data, "OFF") == 0 || strcmp(data, "0") == 0) {
    ledManual = false;
  }

  actualizarLed();
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print(F("Conectando a MQTT... "));
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println(F("Reintentando conexion en 5 segundos..."));
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println(F("MQTT conectado"));
}

float leerDistanciaCm() {
  digitalWrite(TRIGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGER_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracion == 0) {
    return -1.0;
  }
  return duracion * 0.0343 / 2.0;
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(TRIGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println(F("Conectando a Wi-Fi..."));
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("\nWi-Fi conectado"));

  led_sub.setCallback(ledCallback);
  mqtt.subscribe(&led_sub);
}

void loop() {
  MQTT_connect();
  mqtt.processPackets(100);

  if (millis() - ultimaMedicion >= INTERVALO_MEDICION) {
  ultimaMedicion = millis();

  float distanciaCm = leerDistanciaCm();

  if (distanciaCm < 0) {
    Serial.println(F("Sin objetos cerca"));
    ultimaDistancia = -1.0;
    ledPorDistancia = false;
  } else {
    Serial.print(distanciaCm);
    Serial.println(F(" cm"));
    ultimaDistancia = distanciaCm;

    if (distanciaCm < UMBRAL_CM) {
      ledPorDistancia = true;
    } else {
      ledPorDistancia = false;
    }
  }

  actualizarLed();
}

  if (millis() - ultimaPublicacion >= INTERVALO_PUBLICACION) {
    ultimaPublicacion = millis();

    if (ultimaDistancia >= 0) {
      Serial.print(F("Publicando distancia... "));
      if (distancia_pub.publish((double)ultimaDistancia)) {
        Serial.println(F("publicado"));
      } else {
        Serial.println(F("fallo al publicar"));
      }
    }
  }

  if (millis() - ultimoPing >= INTERVALO_PING) {
    ultimoPing = millis();
    if (!mqtt.ping()) {
      mqtt.disconnect();
    }
  }
}