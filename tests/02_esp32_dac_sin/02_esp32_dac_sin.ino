#define DAC1 25
#define DAC2 26
 
#define M_PI (3.1415926535897932385)
#define M_TWO_PI (2*PI)

float freq = 440;
int steps = 20;
int delay_mcs= 1000000 / (freq*steps);

void setup() {
  Serial.begin(115200);
  Serial.print("Delay: ");
  Serial.println(delay_mcs); 
}
 
void loop() {
  for (int i=0; i<steps; i++) {
    int value = 128 + int(10*sin(M_TWO_PI * i / steps)); 
    //Serial.println(value);  
    dacWrite(DAC1, value);
    dacWrite(DAC2, value);
    delayMicroseconds(delay_mcs);
  }
}

