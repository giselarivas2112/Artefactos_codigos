
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// =====================================================
// CONFIGURACION DE WIFI
// =====================================================

#define WLAN_SSID       "Guillermo Rivas"
#define WLAN_PASS       "enmanuel5forever"

// =====================================================
// CONFIGURACION DE ADAFRUIT IO
// =====================================================

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

#define AIO_USERNAME    "gisgis"
#define AIO_KEY         "aio_NsXo19vHl1bv2tHIEPxrVfv7mWwV"

// =====================================================
// CONFIGURACION DEL LED RGB
// =====================================================

#define LED_ROJO   26
#define LED_VERDE  25
#define LED_AZUL   33

// =====================================================
// CONEXION WIFI Y MQTT
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
// CONFIGURACION DEL FEED
// =====================================================

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

  // Si recibimos ON, encender el LED RGB
  if (strcmp(data, "ON") == 0) {

    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AZUL, HIGH);

    Serial.println("LED RGB ENCENDIDO");

  }

  // Si recibimos OFF, apagar todos los colores
  else if (strcmp(data, "OFF") == 0) {

    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AZUL, LOW);

    Serial.println("LED RGB APAGADO");
  }
}

// =====================================================
// CONFIGURACION INICIAL
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(10);

  // Configurar los pines del LED RGB
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  // Comenzar con el LED apagado
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);

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

  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  // ===================================================
  // CONFIGURAR SUSCRIPCION
  // ===================================================

  sub_feed.setCallback(interruptorCallback);

  mqtt.subscribe(&sub_feed);

  Serial.println("Suscripcion al feed 'interruptor' configurada.");
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  // Asegurar conexion MQTT
  MQTT_connect();

  // Escuchar los mensajes que llegan de Adafruit IO
  mqtt.processPackets(1000);

  // Comprobar si sigue activa la conexion
  if (!mqtt.ping()) {

    mqtt.disconnect();
  }
}

// =====================================================
// FUNCION PARA CONECTAR A MQTT
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

    Serial.println("Reintentando conexion en 5 segundos...");

    mqtt.disconnect();

    delay(5000);
  }

  Serial.println("MQTT Conectado!");

}
