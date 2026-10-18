const int LDR_PIN =A1;

void setup() {
  Serial.begin(9600); 

}

void loop() {
  //leer ADC
  int adc = analogRead(LDR_PIN); 

  //Convertir ADC a voltaje 
  float voltaje = 5.0*(adc/1023.0);

  //Mostrar resultados
  Serial.print("ADC: "); 
  Serial.println(adc);
  Serial.print("| Voltaje: ");
  Serial.print(voltaje, 2); 
  Serial.println(" V"); 
  delay(500); 
}
