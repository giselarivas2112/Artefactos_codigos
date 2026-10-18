#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// =====================================================
// CONFIGURACIÓN DE WIFI
// =====================================================

#define WLAN_SSID       "TU_RED_WIFI"
#define WLAN_PASS       "TU_CONTRASENA_WIFI"

// =====================================================
// CONFIGURACIÓN DE ADAFRUIT IO
// =====================================================

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

#define AIO_USERNAME    "TU_USUARIO_ADAFRUIT"
#define AIO_KEY         "TU_KEY_ADAFRUIT"

// =====================================================
// CONFIGURACIÓN DEL LED
// =====================================================

#define LED_PIN 13

// =====================================================
// CONEXIÓN WIFI Y MQTT
// =====================================================

WiFiClient client;

Adafruit_MQTT_Client mqtt(
  &client,
  AIO_SERVER,
  AIO_SERVERPORT,
  AIO_USERNAME,
  AIO_KEY
);

// =====================================================
// CONFIGURACIÓN DE FEEDS
// =====================================================

// ESP32 → Adafruit IO
Adafruit_MQTT_Publish pub_feed =
  Adafruit_MQTT_Publish(
    &mqtt,
    AIO_USERNAME "/feeds/temperatura"
  );

// Adafruit IO → ESP32
Adafruit_MQTT_Subscribe sub_feed =
  Adafruit_MQTT_Subscribe(
    &mqtt,
    AIO_USERNAME "/feeds/interruptor"
  );

// =====================================================
// CALLBACK DEL INTERRUPTOR
// =====================================================

void interruptorCallback(char *data, uint16_t len) {

  Serial.print("--> Mensaje recibido desde Adafruit: ");
  Serial.println(data);

  // Si recibimos ON
  if (strcmp(data, "ON") == 0) {

    digitalWrite(LED_PIN, HIGH);

    Serial.println("LED ENCENDIDO");
  }

  // Si recibimos OFF
  else if (strcmp(data, "OFF") == 0) {

    digitalWrite(LED_PIN, LOW);

    Serial.println("LED APAGADO");
  }
}

// =====================================================
// CONFIGURACIÓN INICIAL
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(10);

  // Configurar LED
  pinMode(LED_PIN, OUTPUT);

  // Comenzar con LED apagado
  digitalWrite(LED_PIN, LOW);

  // ===================================================
  // CONECTAR AL WIFI
  // ===================================================

  Serial.println();
  Serial.println("Conectando a Wi-Fi...");

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado.");

  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // ===================================================
  // CONFIGURAR SUSCRIPCIÓN
  // ===================================================

  sub_feed.setCallback(interruptorCallback);

  mqtt.subscribe(&sub_feed);

  Serial.println("Suscripción al feed 'interruptor' configurada.");
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  // Asegurar conexión MQTT
  MQTT_connect();

  // ===================================================
  // PUBLICAR TEMPERATURA
  // ===================================================

  float valorSensor = random(20, 35);

  Serial.print("<-- Publicando temperatura: ");
  Serial.print(valorSensor);

  if (!pub_feed.publish(valorSensor)) {

    Serial.println(" ¡Falló la publicación!");

  } else {

    Serial.println(" ¡Publicado con éxito!");
  }

  // ===================================================
  // ESCUCHAR MENSAJES DE ADAFRUIT IO
  // =====================================================

  // Durante estos 5 segundos el ESP32
  // también estará pendiente de mensajes ON/OFF.

  mqtt.processPackets(5000);

  // ===================================================
  // MANTENER CONEXIÓN MQTT
  // =====================================================

  if (!mqtt.ping()) {

    mqtt.disconnect();
  }
}

// =====================================================
// FUNCIÓN PARA CONECTAR A MQTT
// =====================================================

void MQTT_connect() {

  int8_t ret;

  // Si ya estamos conectados, no hacemos nada
  if (mqtt.connected()) {

    return;
  }

  Serial.print("Conectando a MQTT... ");

  // Intentar conectar hasta conseguirlo
  while ((ret = mqtt.connect()) != 0) {

    Serial.println(mqtt.connectErrorString(ret));

    Serial.println("Reintentando conexión en 5 segundos...");

    mqtt.disconnect();

    delay(5000);
  }

  Serial.println("¡MQTT Conectado!");
}