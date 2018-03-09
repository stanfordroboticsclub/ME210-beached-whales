#include<AccelStepper.h>

#define STEP_HL 14
#define DIR_HL 15
#define STEP_HR 16
#define DIR_HR 17

#define STEP_VL 18
#define DIR_VL 19
#define STEP_VR 20
#define DIR_VR 21

#define MAX_SPEED 2000
#define MAX_ACCEL 500

AccelStepper motor_horizontalL(1, STEP_HL, DIR_HL);
AccelStepper motor_horizontalR(1, STEP_HR, DIR_HR);
AccelStepper motor_verticalL(1, STEP_VL, DIR_VL);
AccelStepper motor_verticalR(1, STEP_VR, DIR_VR);

void setup() {
  Serial.begin(9600);
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
}

int mapSpeed(int speedUnmapped){
  int speedMapped = map(speedUnmapped, 0, 100, 0, MAX_SPEED);
  return speedMapped;
}

//speed input should range from 0 - 100
void runMotor(char* motor, int speedinput, int dir){
  int speedMapped = mapSpeed(speedinput);
  if(motor == "Horizontal"){
    motor_horizontalL.setSpeed(speedMapped * dir);
    motor_horizontalR.setSpeed(speedMapped * -1* dir);
    motor_horizontalL.runSpeed();
    motor_horizontalR.runSpeed();
    //Serial.println(speedMapped);
  }
  else if(motor == "Vertical"){
    motor_verticalL.setSpeed(speedMapped * dir);
    motor_verticalR.setSpeed(speedMapped * -1* dir);
    motor_verticalL.runSpeed();
    motor_verticalR.runSpeed();
  }
  else{
    Serial.println("runMotor input error!");
  }
}

void loop() {
  static int dir = 1;
  runMotor("Horizontal", 10, dir);
  runMotor("Vertical", 10, dir);
}
