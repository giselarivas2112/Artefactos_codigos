#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Credenciales de tu red Wi-Fi
#define WLAN_SSID       "TU_RED_WIFI"
#define WLAN_PASS       "TU_CONTRASENA_WIFI"

// Credenciales de Adafruit IO (Consíguelas en io.adafruit.com)
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "TU_USUARIO_ADAFRUIT"
#define AIO_KEY         "TU_KEY_ADAFRUIT"

// Variables globales para Wi-Fi y MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Configurar el feed al que te deseas suscribir (ejemplo: "interruptor")
// El formato del topic es: usuario/feeds/nombre_del_feed
Adafruit_MQTT_Subscribe sub_feed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/interruptor");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Conectando a Wi-Fi..."));
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado.");

  // Configurar la llamada de retorno (callback) para cuando llegue un mensaje
  sub_feed.setCallback(feedCallback);

  // Registrar el feed de suscripción en el cliente MQTT
  mqtt.subscribe(&sub_feed);
}

void loop() {
  // Asegurar la conexión MQTT
  MQTT_connect();

  // Mantener la escucha de los datos entrantes (procesa los callbacks)
  mqtt.processPackets(10000);

  // Comprobar si sigue conectado al ping del servidor
  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Función que se ejecuta automáticamente cuando llega un mensaje al feed suscrito
void feedCallback(double current) {
  Serial.print(F("¡Nuevo valor numérico recibido! "));
  Serial.println(current);
}

// Sobrecarga de callback por si el valor es un texto/string
void feedCallback(char *data, uint16_t len) {
  Serial.print(F("¡Nuevo texto recibido! "));
  Serial.println(data);
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print(F("Conectando a MQTT... "));

  while ((ret = mqtt.connect()) != 0) { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println(F("Reintentando conexión MQTT en 5 segundos..."));
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println(F("¡MQTT Conectado!"));
}
