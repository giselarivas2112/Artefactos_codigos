#include <SPI.h>
#include <MFRC522.h>

// Definimos los pines configurables para el ESP32
#define SS_PIN    5   // Pin SDA (Selector) en el GPIO 5
#define RST_PIN   22  // Pin Reset en el GPIO 22

MFRC522 mfrc522(SS_PIN, RST_PIN); // Creamos la instancia del lector

void setup() {
  Serial.begin(115200); // Iniciamos el monitor serie a alta velocidad para ESP32
  while (!Serial);      // Espera activa (necesaria en algunas placas basadas en USB)

  SPI.begin();          // Inicializa el bus SPI usando los pines por defecto (18, 19, 23)
  mfrc522.PCD_Init();   // Inicializa físicamente el chip MFRC522

  Serial.println("--- ESP32 Lector RFID Inicializado ---");
  Serial.println("Acerca una tarjeta o llavero...");
}

void loop() {
  // Filtro 1: ¿Hay una tarjeta físicamente cerca del lector?
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return; // Si no hay nada, el código se detiene aquí y vuelve a empezar el loop
  }

  // Filtro 2: ¿Podemos leer con éxito los datos seriales de esa tarjeta?
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return; // Si la lectura falla o se interrumpe, sale del loop
  }

  // Si pasamos los dos filtros anteriores, procedemos a imprimir el UID
  Serial.print("Tarjeta Detectada! UID Hexadecimal:");

  // Un UID suele medir 4 bytes (ej: MIFARE Classic). Recorremos cada byte individualmente:
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    
   
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print(" 0");
    } else {
      Serial.print(" ");
    }
    
    // Imprime el valor del byte actual transformándolo a formato HEX (Hexadecimal)
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.println(); // Salto de línea final para la siguiente tarjeta
  //evita bloquear la tarjeta
  mfrc522.PICC_HaltA();
}