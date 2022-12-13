#define DAC1 25
#define DAC2 26
 
void setup() {
//  Serial.begin(115200);
}
 
void loop() {
  for (int i=0; i<=4; i++) {
    int value = i * 255/4; // 255= 3.3V, 128=1.65V
  
    dacWrite(DAC1, value);
    dacWrite(DAC2, 255-value);
    delay(2000);
  }
}

