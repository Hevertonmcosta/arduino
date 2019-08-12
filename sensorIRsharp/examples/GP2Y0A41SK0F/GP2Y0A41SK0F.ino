#include <sensorIRsharp.h>
sensorIR s(A0, 0.0, GP2Y0A41SK0F);
void setup()  {
  Serial.begin(9600);
  Serial.println("Starting System");
}

void loop() {
  s.loop();
  Serial.println(s.getValue());
  delay(100);
}