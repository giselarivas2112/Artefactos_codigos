
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <string.h>

// Wi-Fi
#define WLAN_SSID "Guillermo Rivas"
#define WLAN_PASS "enmanuel5forever"

// Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "gisgis"
#define AIO_KEY "aio_NsXo19vHl1bv2tHIEPxrVfv7mWwV"

// Pines del sensor ultrasónico
#define TRIG_PIN 32
#define ECHO_PIN 35

// Pines del LED RGB
const int ROJO = 26;
const int VERDE = 25;
const int AZUL = 33;

// Cliente MQTT
WiFiClient client;

Adafruit_MQTT_Client mqtt(
  &client,
  AIO_SERVER,
  AIO_SERVERPORT,
  AIO_USERNAME,
  AIO_KEY
);

// Feeds de Adafruit IO
Adafruit_MQTT_Publish feedDistancia(
  &mqtt, AIO_USERNAME "/feeds/distancia"
);

Adafruit_MQTT_Subscribe feedBoton(
  &mqtt, AIO_USERNAME "/feeds/interruptor"
);

// Variables
float distancia = -1;
unsigned long ultimoPublicar = 0;
bool ledEncendido = true;

// =====================================================
// FUNCIONES
// =====================================================

// Conectar al Wi-Fi
void conectarWiFi() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  Serial.print("Conectando al Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

// Conectar a Adafruit IO
void conectarMQTT() {
  if (mqtt.connected()) {
    return;
  }

  Serial.println("Conectando a Adafruit IO...");

  int8_t resultado;

  while ((resultado = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(resultado));

    mqtt.disconnect();

    Serial.println("Reintentando en 5 segundos");
    delay(5000);
  }

  // Suscribirse al feed del boton
  mqtt.subscribe(&feedBoton);

  Serial.println("Conectado a Adafruit IO");
}

// Controlar los colores del LED RGB
void color(int r, int v, int a) {
  analogWrite(ROJO, r);
  analogWrite(VERDE, v);
  analogWrite(AZUL, a);
}

// Elegir el color segun la distancia
void actualizarLED() {
  // El boton tiene prioridad sobre el sensor
  if (!ledEncendido) {
    color(0, 0, 0);
  }
  else if (distancia < 0) {
    color(255, 255, 255);  // Blanco: sin lectura valida
  }
  else if (distancia <= 20) {
    color(255, 0, 0);      // Rojo: objeto cerca
  }
  else if (distancia <= 50) {
    color(255, 255, 0);    // Amarillo: distancia intermedia
  }
  else {
    color(0, 255, 0);      // Verde: objeto alejado
  }
}

// Callback del boton de Adafruit IO
void interruptorCallback(char *data, uint16_t len) {
  // Copiar el mensaje para asegurarnos de que termine en cero
  char orden[16];

  size_t cantidad = len < sizeof(orden) - 1
                    ? len
                    : sizeof(orden) - 1;

  memcpy(orden, data, cantidad);
  orden[cantidad] = '\0';

  Serial.print("--> Mensaje recibido desde Adafruit: ");
  Serial.println(orden);

  // Si recibimos ON, permitir que el RGB se encienda
  if (strcmp(orden, "ON") == 0) {
    ledEncendido = true;
    actualizarLED();

    Serial.println("LED RGB ENCENDIDO");
  }

  // Si recibimos OFF, apagar el RGB
  else if (strcmp(orden, "OFF") == 0) {
    ledEncendido = false;
    actualizarLED();

    Serial.println("LED RGB APAGADO");
  }
}

// Leer la distancia con el HC-SR04
float leerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracion == 0) {
    return -1;
  }

  float cm = (0.0343 * duracion) / 2;

  if (cm < 2 || cm > 100) {
    return -1;
  }

  return cm;
}

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);

  // Configurar el sensor ultrasónico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configurar el LED RGB
  pinMode(ROJO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);

  // Apagar el LED al comenzar
  color(0, 0, 0);

  // Conectar al Wi-Fi
  conectarWiFi();

  // Configurar el callback del boton
  feedBoton.setCallback(interruptorCallback);

  // Conectar a Adafruit IO
  conectarMQTT();
}

// =====================================================
// LOOP
// =====================================================

void loop() {
  // Mantener la conexion MQTT
  conectarMQTT();

  // Procesar mensajes recibidos de Adafruit IO
  mqtt.processPackets(100);

  // Comprobar la conexion MQTT
  if (mqtt.connected() && !mqtt.ping()) {
    mqtt.disconnect();
  }

  // Medir y publicar cada 5 segundos
  if (millis() - ultimoPublicar >= 5000) {
    ultimoPublicar = millis();

    distancia = leerDistancia();

    if (distancia > 0) {
      Serial.print("Distancia: ");
      Serial.print(distancia, 1);
      Serial.println(" cm");

      if (feedDistancia.publish(distancia)) {
        Serial.println("Distancia publicada en Adafruit IO");
      }
      else {
        Serial.println("Error al publicar");
      }
    }
    else {
      Serial.println(
        "No se detecto eco o la distancia esta fuera del rango"
      );
    }

    // Actualizar el RGB segun la distancia
    // Si el boton esta en OFF, permanecera apagado
    actualizarLED();
  }
}
