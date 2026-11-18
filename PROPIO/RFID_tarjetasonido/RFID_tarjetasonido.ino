<SPI.h>
#include <MFRC522.h>

constexpr int RST_PIN = 22; 
constexpr int SS_PIN = 5;   

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(115200); 
  while (!Serial);     
  
  SPI.begin();         
  mfrc522.PCD_Init();  
  
  Serial.println("Lector RFID listo. Aproxime su tarjeta...");
  Serial.println();
}

void loop() {
  // Detectar si hay una nueva tarjeta presente y leerla
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // ENVIAR SEÑAL CLAVE A PYTHON
  Serial.println("TARJETA_DETECTADA");

  // Opcional: Mostrar el UID en el monitor serial
  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Detener lectura para evitar bucles
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
