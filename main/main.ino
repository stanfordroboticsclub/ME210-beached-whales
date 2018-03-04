#include <Servo.h>
#include <Metro.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define MAX_SPEED 200
#define MAX_ACCELERATION 200

//temporary pin values
#define X_DIR_Pin  0
#define X_STEP_Pin 1

#define Y_DIR_Pin  2
#define Y_STEP_Pin 3

AccelStepper Xstepper(AccelStepper::DRIVER, X_STEP_Pin, X_DIR_Pin);
AccelStepper Ystepper(AccelStepper::DRIVER, Y_STEP_Pin, Y_DIR_Pin);

void setup() {
    Xstepper.setMaxSpeed(MAX_SPEED);
    Xstepper.setAcceleration(MAX_ACCELERATION);

    Ystepper.setMaxSpeed(MAX_SPEED);
    Ystepper.setAcceleration(MAX_ACCELERATION);

}

void loop() {
  // put your main code here, to run repeatedly:

}
