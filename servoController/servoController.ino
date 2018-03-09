#include<Servo.h>

int servo = A9;
Servo latch;

void setup() {
  latch.attach(servo);
  latch.write(90);
  delay(1000);
}

void loop() {
  
//  delay(1000);
  latch.write(180);
  delay(1000);

}
