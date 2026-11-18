import serial
import time
import winsound  # Librería nativa de Windows para emitir sonidos

# IMPORTANTE: Cambia 'COM3' por el puerto al que esté conectada tu ESP32
PUERTO_SERIAL = 'COM3' 
VELOCIDAD = 115200       # Debe coincidir exactamente con el Serial.begin de la ESP32

print(f"Conectando a la ESP32 en {PUERTO_SERIAL} a {VELOCIDAD} baudios...")

try:
    arduino = serial.Serial(PUERTO_SERIAL, VELOCIDAD)
    time.sleep(2) # Espera a que se establezca la conexión física
except Exception as e:
    print(f"No se pudo conectar al puerto {PUERTO_SERIAL}. Revisa la conexión. Error: {e}")
    exit()

print("Sistema activo. Esperando tarjeta RFID...")

while True:
    try:
        if arduino.in_waiting > 0:
            # Lee la línea enviada por la ESP32 y limpia los espacios/saltos de línea
            linea = arduino.readline().decode('utf-8', errors='ignore').strip()
            
            # Si la ESP32 avisa que leyó la tarjeta
            if linea == "TARJETA_DETECTADA":
                print("¡Tarjeta pasada con éxito! Reproduciendo sonido...")
                
                # Emite sonido en los altavoces de la PC 
                # (Frecuencia en Hz: 1500, Duración en ms: 300)
                winsound.Beep(1500, 300)
                
            elif linea.startswith("UID:"):
                # Imprime el código de la tarjeta en la consola de Python
                print(f"-> {linea}")
                
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario.")
        arduino.close()
        break