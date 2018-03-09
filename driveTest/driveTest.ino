#include <AccelStepper.h>

#define STEP_VL   14
#define DIR_VL    15
#define STEP_VR   16
#define DIR_VR    17

#define STEP_HL   18
#define DIR_HL    19
#define STEP_HR   20
#define DIR_HR    21

//#define MAX_SPEED 500
#define MAX_SPEED 2000
#define MAX_ACCEL 1500

#define HORIZONTAL_MOTORS 0
#define VERTICAL_MOTORS   1
#define OPEN 0
#define CLOSE 1

#define DIR_RIGHT -1
#define DIR_LEFT  1
#define DIR_FRONT 1
#define DIR_BACK  -1

#define STEPS_PER_INCH    80

AccelStepper motor_horizontalL(1, STEP_HL, DIR_HL);
AccelStepper motor_horizontalR(1, STEP_HR, DIR_HR);
AccelStepper motor_verticalL(1, STEP_VL, DIR_VL);
AccelStepper motor_verticalR(1, STEP_VR, DIR_VR);

void setup() {
  motor_horizontalL.setMaxSpeed(MAX_SPEED);
  motor_horizontalR.setMaxSpeed(MAX_SPEED);
  motor_verticalL.setMaxSpeed(MAX_SPEED);
  motor_verticalR.setMaxSpeed(MAX_SPEED);
  
  motor_horizontalL.setAcceleration(MAX_ACCEL);
  motor_horizontalR.setAcceleration(MAX_ACCEL);
  motor_verticalL.setAcceleration(MAX_ACCEL);
  motor_verticalR.setAcceleration(MAX_ACCEL);

  pinMode(STEP_HL, OUTPUT);
  pinMode(STEP_HR, OUTPUT);
  pinMode(DIR_HL, OUTPUT);
  pinMode(DIR_HR, OUTPUT);
  pinMode(STEP_VL, OUTPUT);
  pinMode(STEP_VR, OUTPUT);
  pinMode(DIR_VL, OUTPUT);
  pinMode(DIR_VR, OUTPUT);

  driveTo(HORIZONTAL_MOTORS, DIR_RIGHT * STEPS_PER_INCH * 55);
}

void driveTo(int dir, int distance) {
  if (dir == HORIZONTAL_MOTORS) {
    motor_horizontalL.move(distance);
    motor_horizontalR.move(-distance);
    motor_verticalL.move(0);
    motor_verticalR.move(0);
  } else if (dir == VERTICAL_MOTORS) {
    motor_horizontalL.move(0);
    motor_horizontalR.move(0);
    motor_verticalL.move(distance);
    motor_verticalR.move(-distance);
  } else {
    Serial.println("driveTo input error!");
  }
}

void loop() {
  motor_horizontalL.run();
  motor_horizontalR.run();
  motor_verticalL.run();
  motor_verticalR.run();
}
