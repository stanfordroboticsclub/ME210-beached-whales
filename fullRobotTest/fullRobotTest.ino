#include <AccelStepper.h>
#include <Servo.h>
#include <Metro.h>

// Servo constants and instances
#define SERVO_PIN A9
Servo latch;

// Stepper constants and instances
#define STEP_V   14
#define DIR_V    15

#define STEP_H   20
#define DIR_H    21

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

AccelStepper motor_horizontal(1, STEP_H, DIR_H);
AccelStepper motor_vertical(1, STEP_V, DIR_V);

static Metro timer = Metro(0);
State state = INIT_0;

//predeclaration
bool waitForTime(int delayTime);
int mapSpeed(int speedUnmapped);
void runMotor(char* motor, int speedinput, int dir);
void moveLatch(int pos);

void setup() {
  Serial.begin(9600);
  motor_horizontal.setMaxSpeed(MAX_SPEED);
  motor_vertical.setMaxSpeed(MAX_SPEED);
  
  motor_horizontal.setAcceleration(MAX_ACCEL);
  motor_vertical.setAcceleration(MAX_ACCEL);

  pinMode(STEP_H, OUTPUT);
  pinMode(STEP_HR, OUTPUT);
  pinMode(DIR_H, OUTPUT);
  pinMode(DIR_HR, OUTPUT);
  pinMode(STEP_V, OUTPUT);
  pinMode(STEP_VR, OUTPUT);
  pinMode(DIR_V, OUTPUT);
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
//    motor_horizontal.setSpeed(speedMapped * dir);
//    motor_horizontalR.setSpeed(speedMapped * -1* dir);
//    motor_horizontal.runSpeed();
//    motor_horizontalR.runSpeed();
//  }
//  else if(motor == "Vertical"){
//    motor_vertical.setSpeed(speedMapped * dir);
//    motor_verticalR.setSpeed(speedMapped * -1* dir);
//    motor_vertical.runSpeed();
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
      motor_horizontal.move(-steps);
      motor_horizontalR.move(steps);
      motor_vertical.move(0);
      motor_verticalR.move(0);
      break;
    case DIR_DOWN:
      motor_horizontal.move(steps);
      motor_horizontalR.move(-steps);
      motor_vertical.move(0);
      motor_verticalR.move(0);
      break;
    case DIR_LEFT:
      motor_horizontal.move(0);
      motor_horizontalR.move(0);
      motor_vertical.move(steps);
      motor_verticalR.move(-steps);
      break;
    case DIR_RIGHT:
      motor_horizontal.move(0);
      motor_horizontalR.move(0);
      motor_vertical.move(-steps);
      motor_verticalR.move(steps);
      break;
    default:
      Serial.println("driveTo input error!");
  }
}

void driveToCoordinate(int x, int y) {
    motor_horizontal.moveTo(STEPS_PER_INCH * y);
    motor_vertical.moveTo(STEPS_PER_INCH * x);
}

void resetCoordinates() {
  motor_horizontal.setCurrentPosition(0);
  motor_vertical.setCurrentPosition(0);
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
  return (motor_horizontal.distanceToGo() == 0 &&
          motor_vertical.distanceToGo() == 0);
}

void loop() {

  motor_horizontal.run();
  motor_vertical.run();
  
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
