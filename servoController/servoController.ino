#include<Servo.h>

int servo = A0;
Servo latch;

void setup() {
  latch.attach(servo);
}

void loop() {
  latch.write(0);
  delay(1000);
  latch.write(180);
  delay(1000);

}
