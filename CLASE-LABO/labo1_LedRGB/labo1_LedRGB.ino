int r = 2;
int v = 3;
int a = 4; 

void setup() {
  pinMode(r,OUTPUT);
  pinMode(v,OUTPUT);
  pinMode(a,OUTPUT);

}

void loop() {
  analogWrite(r,39);
  analogWrite(v,235);
  analogWrite(a,245); 
  delay(2000); 

  analogWrite(r,138);
  analogWrite(v,43);
  analogWrite(a,226); 
  delay(2000); 

  analogWrite(r,0);
  analogWrite(v,100);
  analogWrite(a,0); 
  delay(2000); 

  analogWrite(r,218);
  analogWrite(v,165);
  analogWrite(a,32); 
  delay(2000); 

  analogWrite(r,128);
  analogWrite(v,0);
  analogWrite(a,0); 
  delay(2000); 

  analogWrite(r,250);
  analogWrite(v,128);
  analogWrite(a,114); 
  delay(2000); 

  analogWrite(r,0);
  analogWrite(v,128);
  analogWrite(a,128); 
  delay(2000); 

  analogWrite(r,255);
  analogWrite(v,99);
  analogWrite(a,71); 
  delay(2000); 

  analogWrite(r,65);
  analogWrite(v,105);
  analogWrite(a,225); 
  delay(2000); 

  analogWrite(r,75);
  analogWrite(v,0);
  analogWrite(a,130); 
  delay(2000); 

}
