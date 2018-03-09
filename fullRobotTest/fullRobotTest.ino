#include <AccelStepper.h>
#include <Servo.h>
#include <Metro.h>

// Servo constants and instances
#define SERVO_PIN A9
Servo latch;

// Stepper constants and instances
#define STEP_VL   14
#define DIR_VL    15
#define STEP_VR   16
#define DIR_VR    17

#define STEP_HL   20
#define DIR_HL    21
#define STEP_HR   18
#define DIR_HR    19

#define MAX_SPEED 1000
//#define MAX_SPEED 100
#define MAX_ACCEL 500
//#define MAX_ACCEL 50

#define OPEN 0
#define CLOSE 1

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define STEPS_PER_INCH    80

#define INCHES_CORNER_TO_A 45
#define INCHES_START_TO_A 19 
#define INCHES_A_TO_PO    18
#define INCHES_PO_TO_B    17
#define INCHES_B_TO_GATE  21
#define INCHES_THRU_GATE  32

//#define ABSOLUTE_A   
//#define ABSOLUTE_PO
//#define ABSOLUTE_B

int robotX, robotY;

enum State {
  INIT_0,
  FORWARD_1,
  DROP_BALLS_2,
  FORWARD_3,
  DROP_BALLS_4,
  FORWARD_5,
  RIGHT_6,

  RAM_1,
  RAM_2,
};

AccelStepper motor_horizontalL(1, STEP_HL, DIR_HL);
//AccelStepper motor_horizontalR(1, STEP_HR, DIR_HR);
AccelStepper motor_verticalL(1, STEP_VL, DIR_VL);
//AccelStepper motor_verticalR(1, STEP_VR, DIR_VR);

static Metro timer = Metro(0);
State state = INIT_0;

//predeclaration
bool waitForTime(int delayTime);
int mapSpeed(int speedUnmapped);
void runMotor(char* motor, int speedinput, int dir);
void moveLatch(int pos);

void setup() {
  Serial.begin(9600);
  motor_horizontalL.setMaxSpeed(MAX_SPEED);
//  motor_horizontalR.setMaxSpeed(MAX_SPEED);
  motor_verticalL.setMaxSpeed(MAX_SPEED);
//  motor_verticalR.setMaxSpeed(MAX_SPEED);
  
  motor_horizontalL.setAcceleration(MAX_ACCEL);
//  motor_horizontalR.setAcceleration(MAX_ACCEL);
  motor_verticalL.setAcceleration(MAX_ACCEL);
//  motor_verticalR.setAcceleration(MAX_ACCEL);

  pinMode(STEP_HL, OUTPUT);
  pinMode(STEP_HR, OUTPUT);
  pinMode(DIR_HL, OUTPUT);
  pinMode(DIR_HR, OUTPUT);
  pinMode(STEP_VL, OUTPUT);
  pinMode(STEP_VR, OUTPUT);
  pinMode(DIR_VL, OUTPUT);
  pinMode(DIR_VR, OUTPUT);

  latch.attach(SERVO_PIN);
  latch.write(0);

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
//void runMotor(char* motor, int speedinput, int dir){
//  int speedMapped = mapSpeed(speedinput);
//  if(motor == "Horizontal"){
//    motor_horizontalL.setSpeed(speedMapped * dir);
//    motor_horizontalR.setSpeed(speedMapped * -1* dir);
//    motor_horizontalL.runSpeed();
//    motor_horizontalR.runSpeed();
//  }
//  else if(motor == "Vertical"){
//    motor_verticalL.setSpeed(speedMapped * dir);
//    motor_verticalR.setSpeed(speedMapped * -1* dir);
//    motor_verticalL.runSpeed();
//    motor_verticalR.runSpeed();
//  }
//  else{
//    Serial.println("runMotor input error!");
//  }
//}

//input DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, and distance that needs to travel in inches
void driveTo(int dir, int distance) {
  int steps = STEPS_PER_INCH * distance;
  switch(dir){
    case DIR_UP:
      motor_horizontalL.move(-steps);
      motor_horizontalR.move(steps);
      motor_verticalL.move(0);
      motor_verticalR.move(0);
      break;
    case DIR_DOWN:
      motor_horizontalL.move(steps);
      motor_horizontalR.move(-steps);
      motor_verticalL.move(0);
      motor_verticalR.move(0);
      break;
    case DIR_LEFT:
      motor_horizontalL.move(0);
      motor_horizontalR.move(0);
      motor_verticalL.move(steps);
      motor_verticalR.move(-steps);
      break;
    case DIR_RIGHT:
      motor_horizontalL.move(0);
      motor_horizontalR.move(0);
      motor_verticalL.move(-steps);
      motor_verticalR.move(steps);
      break;
    default:
      Serial.println("driveTo input error!");
  }
}

void driveToCoordinate(int x, int y) {
    motor_horizontalL.moveTo(STEPS_PER_INCH * y);
    motor_verticalL.moveTo(STEPS_PER_INCH * x);
}

void resetCoordinates() {
  motor_horizontalL.setCurrentPosition(0);
  motor_verticalL.setCurrentPosition(0);
}

void moveLatch(int pos){
  static int lastPos  = CLOSE;
  if(pos == OPEN && lastPos != OPEN){
    Serial.println("OPENING");
    lastPos = OPEN;
    latch.write(45);
  }
  else if(pos == CLOSE && lastPos != CLOSE){
    Serial.println("CLOSING");
    lastPos = CLOSE;
    latch.write(0);
  }
//  else{
//    Serial.println("moveLatch input error!");
//  }
}

bool driveDoneMoving() {
  return //(motor_horizontalR.distanceToGo() == 0 &&
          (motor_horizontalL.distanceToGo() == 0 &&
          //motor_verticalR.distanceToGo() == 0 &&
          motor_verticalL.distanceToGo() == 0);
}

void loop() {

  motor_horizontalL.run();
//  motor_horizontalR.run();
  motor_verticalL.run();
//  motor_verticalR.run();
  
  switch(state){
    case INIT_0:
      driveTo(DIR_LEFT,INCHES_START_TO_A);
      state = FORWARD_1;
      break;
    
    case FORWARD_1:
      if (driveDoneMoving()) {
        driveTo(DIR_UP, 2);
        state = RAM_1;
//        state = DROP_BALLS_2;
//        timer.reset();
      }
      break;

    case RAM_1:
      if (driveDoneMoving()) {
        state = DROP_BALLS_2;
        timer.reset();
      }
      break;
      
    case DROP_BALLS_2:
      moveLatch(OPEN);
      if (waitForTime(140)) {
        moveLatch(CLOSE);
        driveTo(DIR_LEFT, INCHES_A_TO_PO + INCHES_PO_TO_B);
        state = FORWARD_3;
      }
      break;
    
    case FORWARD_3:
      if (driveDoneMoving()) {
        driveTo(DIR_UP, 2);
        state = RAM_2;
      }
      break;

    case RAM_2:
      if (driveDoneMoving()) {
        state = DROP_BALLS_4;
        timer.reset();
      }
      break;
    
    case DROP_BALLS_4:
      moveLatch(OPEN);
      if (waitForTime(160)) {
        moveLatch(CLOSE);
        driveTo(DIR_LEFT, INCHES_B_TO_GATE);
        state = FORWARD_5;
      }
      break;
    
    case FORWARD_5:
      if (driveDoneMoving()) {
        driveTo(DIR_UP, INCHES_B_TO_GATE * 1.5);
        state = RIGHT_6;
      }
      break;
    
    case RIGHT_6:
      break;
  }
}
