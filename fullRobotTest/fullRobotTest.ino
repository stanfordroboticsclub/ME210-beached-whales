#include <AccelStepper.h>
#include <Servo.h>
#include <Metro.h>
#include <MultiStepper.h>

// Servo constants and instances
#define LATCH_SERVO_PIN A8
Servo latch;

#define TILT_SERVO_PIN A9
Servo tilt;

// Stepper constants and instances
#define STEP_V   14
#define DIR_V    15

#define STEP_H   20
#define DIR_H    21

#define MAX_SPEED 1000
#define MAX_ACCEL 500

// Latch constants
#define OPEN 0
#define CLOSE 1

// Tilter constants
#define TILT_UP   0
#define TILT_DOWN 1

// Drivetrain directions
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define STEPS_PER_INCH    80

// Relative distances
#define INCHES_CORNER_TO_A 45
#define INCHES_START_TO_A 19 
#define INCHES_A_TO_PO    18
#define INCHES_PO_TO_B    17
#define INCHES_B_TO_GATE  21
#define INCHES_THRU_GATE  32

// Absolute coordinates
#define ROUNDS_Y   13
#define ROUND_A_X  42
#define PO_X       60 // TODO
#define ROUND_B_X  77 // TODO
#define GATE_X     98 // TODO

MultiStepper motors;
AccelStepper motor_horizontal(1, STEP_H, DIR_H);
AccelStepper motor_vertical(1, STEP_V, DIR_V);

static Metro timer = Metro(0);
static Metro gameTimer = Metro((2*60 + 10)*1000); // 2 min, 10 sec

void setup() {
  Serial.begin(9600);
  motor_horizontal.setMaxSpeed(MAX_SPEED);
  motor_vertical.setMaxSpeed(MAX_SPEED);

  motors.addStepper(motor_horizontal);
  motors.addStepper(motor_vertical);
  
  motor_horizontal.setAcceleration(MAX_ACCEL);
  motor_vertical.setAcceleration(MAX_ACCEL);

  pinMode(STEP_H, OUTPUT);
  pinMode(DIR_H, OUTPUT);
  pinMode(STEP_V, OUTPUT);
  pinMode(DIR_V, OUTPUT);

  latch.attach(LATCH_SERVO_PIN);
  latch.write(180);

  tilt.attach(TILT_SERVO_PIN);
  tilt.write(140);

  resetOrigin();

  delay(2000);

  gameTimer.reset();
}

bool waitForTime(int delayTime){
  static bool isFirstTime = true;
  if(isFirstTime) timer.reset();
  timer.interval(delayTime);
  if(timer.check()){
    timer.reset();
    isFirstTime = true;
    return true;
  }
  else{
    isFirstTime = false;
    return false;
  }
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
void driveToDistance(int dir, int distance) {
  int steps = STEPS_PER_INCH * distance;
  switch(dir){
    case DIR_RIGHT:
      motor_horizontal.move(-steps);
      motor_vertical.move(0);
      break;
    case DIR_LEFT:
      motor_horizontal.move(steps);
      motor_vertical.move(0);
      break;
    case DIR_UP:
      motor_horizontal.move(0);
      motor_vertical.move(steps);
      break;
    case DIR_DOWN:
      motor_horizontal.move(0);
      motor_vertical.move(-steps);
      break;
    default:
      Serial.println("driveTo input error!");
  }
}

void driveToCoordinate(int x, int y) {
    motor_horizontal.moveTo(STEPS_PER_INCH * x);
    motor_vertical.moveTo(STEPS_PER_INCH * y);
}

void resetOrigin() {
  motor_horizontal.setCurrentPosition(0);
  motor_vertical.setCurrentPosition(0);
}

void moveLatch(int pos){
  static int lastPos  = CLOSE;
  if(pos == OPEN && lastPos != OPEN){
    Serial.println("OPENING");
    lastPos = OPEN;
    latch.write(135);
  }
  else if(pos == CLOSE && lastPos != CLOSE){
    Serial.println("CLOSING");
    lastPos = CLOSE;
    latch.write(180);
  }
}

void moveTilt(int pos) {
  if (pos == TILT_UP) {
    tilt.write(145);
  } else if (pos == TILT_DOWN) {
    tilt.write(0);
  }
}

bool driveDoneMoving() {
  return (motor_horizontal.distanceToGo() == 0 &&
          motor_vertical.distanceToGo() == 0);
}

void babysit() {
  // If 2:10 timer hasn't expired, run motors
  if (!gameTimer.check()) {
    motor_horizontal.run();
    motor_vertical.run();
  }
}

void lineUpInOrigin_Relative() {
  driveToCoordinate(-30, -16);
  while(!driveDoneMoving()) babysit();
  
  resetOrigin();
}

void lineUpInOrigin_Absolute() {
  driveToCoordinate(-3, -3);
  while(!driveDoneMoving()) babysit();
  
  resetOrigin();
}

void moveToA_Relative() {
  // Drive in X
  driveToDistance(DIR_LEFT, INCHES_START_TO_A);
  while(!driveDoneMoving()) babysit();
}

void tiltAndDropBalls(int numBalls) {
  // Tilt down
  moveTilt(TILT_DOWN);
  while(!waitForTime(500)) babysit();

  // Drop balls
  dropBalls(numBalls);

  // Tilt up
  while(!waitForTime(1000)) babysit();
  moveTilt(TILT_UP);
}

void dropBalls(int numBalls) {
  moveLatch(OPEN);
  while(!waitForTime(100 * numBalls)) babysit();
  moveLatch(CLOSE);
}

void moveToRound_Absolute(int roundX) {
  // Go slightly behind
  driveToCoordinate(roundX, ROUNDS_Y-2);
  while(!driveDoneMoving()) babysit();

  // Recalibrate against wall
  driveToCoordinate(roundX, ROUNDS_Y+2);
  while(!driveDoneMoving()) babysit();
  motor_vertical.setCurrentPosition(ROUNDS_Y * STEPS_PER_INCH);
}

void moveToReload_Absolute() {
  // Aim to not touch reload button first, recalibrate x
  driveToCoordinate(-2, ROUNDS_Y - 5);
  while(!driveDoneMoving()) babysit();
  motor_horizontal.setCurrentPosition(0);

  // Overshoot touching reload button, recalibrate y
  driveToCoordinate(0, ROUNDS_Y + 2);
  while(!driveDoneMoving()) babysit();
  motor_vertical.setCurrentPosition(ROUNDS_Y * STEPS_PER_INCH);

  // Wait while being reloaded
  while(!waitForTime(1000)) babysit();
}

void goToGate() {
  driveToCoordinate(GATE_X, ROUNDS_Y - 5);
  while(!driveDoneMoving()) babysit();
}

void loop() {
  moveToA_Relative();
  tiltAndDropBalls(4);
  
  lineUpInOrigin_Relative();
  moveToReload_Absolute();
  
  moveToRound_Absolute(ROUND_A_X);
  tiltAndDropBalls(4);

  goToGate();
  
  moveToRound_Absolute(ROUND_B_X);
  tiltAndDropBalls(0);

  moveToReload_Absolute();

  moveToRound_Absolute(PO_X);
  tiltAndDropBalls(4);

  moveToRound_Absolute(ROUND_B_X);
  tiltAndDropBalls(0);

  while(true);
}
