#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

int analogico1 = A0; 


Adafruit_SH1106G display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

void setup() {


  Serial.begin(9600);

  
  Wire.begin();

  delay(250);
  

  
  if (!display.begin(i2c_Address, true)) {
    Serial.println("ERROR: OLED no encontrada");

    while (1);
  }

  Serial.println("OLED funcionando correctamente");

  

  
  
}

void loop() {
  float valorleido = analogRead(analogico1); 

  float temperatura = (valorleido * 5.0/1023.0) * 100;


   Serial.print("La temperatura actual es: " );
   Serial.println(temperatura);

     display.clearDisplay();

   display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 10);
  display.println("TEMPERATURA:");

  
  display.setTextSize(2);
  display.setCursor(15, 40);
  display.println(temperatura);

  
  display.display();
   delay(1000);
  
}