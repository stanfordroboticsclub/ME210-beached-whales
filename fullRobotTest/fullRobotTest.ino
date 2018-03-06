#include<AccelStepper.h>
#include<Servo.h>
#include <Metro.h>

int servo = A9;
Servo latch;

#define STEP_HL 14
#define DIR_HL 15
#define STEP_HR 16
#define DIR_HR 17

#define STEP_VL 18
#define DIR_VL 19
#define STEP_VR 20
#define DIR_VR 21

#define MAX_SPEED 280

AccelStepper motor_horizontalL(1, STEP_HL, DIR_HL);
AccelStepper motor_horizontalR(1, STEP_HR, DIR_HR);
AccelStepper motor_verticalL(1, STEP_VL, DIR_VL);
AccelStepper motor_verticalR(1, STEP_VR, DIR_VR);

static Metro timer = Metro(0);
int state = 0;

//predeclaration
bool waitForTime(int delayTime);
int mapSpeed(int speedUnmapped);
void runMotor(char* motor, int speedinput, int dir);
void moveLatch(char* pos);

void setup() {
  Serial.begin(9600);
  motor_horizontalL.setMaxSpeed(MAX_SPEED);
  motor_horizontalR.setMaxSpeed(MAX_SPEED);
  motor_verticalL.setMaxSpeed(MAX_SPEED);
  motor_verticalR.setMaxSpeed(MAX_SPEED);

  pinMode(STEP_HL, OUTPUT);
  pinMode(STEP_HR, OUTPUT);
  pinMode(DIR_HL, OUTPUT);
  pinMode(DIR_HR, OUTPUT);
  pinMode(STEP_VL, OUTPUT);
  pinMode(STEP_VR, OUTPUT);
  pinMode(DIR_VL, OUTPUT);
  pinMode(DIR_VR, OUTPUT);

  latch.attach(servo);
  moveLatch("close");
  delay(2000);
}

bool waitForTime(int delayTime){
  timer.interval(delayTime);
  if(timer.check()){
    timer.reset();
    return true;
  }
  else return false;
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

void moveLatch(char* pos){
  if(pos == "open"){
    latch.write(180);
  }
  else if(pos == "close"){
    latch.write(0);
  }
  else{
    Serial.println("moveLatch input error!");
  }
}

void loop() {
  static int dir = 1;

  switch(state){
    case 0:
      runMotor("Horizontal", 100, dir);
      if(waitForTime(2000)) state += 1;
      break;

    case 1:
      runMotor("Horizontal", 0, dir);
      runMotor("Vertical", 100, dir);
      if(waitForTime(2000)) state = 0;
      break;
  }
  /*
  runMotor("Horizontal", 100, dir);
  runMotor("Vertical", 100, dir);
  moveLatch("close");

  if(waitForTime(2000)){
    Serial.println("done!");
  }
  */
}
