#include <AccelStepper.h>
#include <Servo.h>
#include <Metro.h>
#include <MultiStepper.h>

#define RELOAD_TIME 500

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

#define MAX_SPEED 1200
#define MAX_ACCEL 700

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

long ROUND_A[2] = {ROUND_A_X, ROUNDS_Y};
long ROUND_P[2] = {PO_X, ROUNDS_Y};
long ROUND_B[2] = {ROUND_B_X, ROUNDS_Y};
long GATE[2] = {GATE_X, ROUNDS_Y};
long HOMING[2] = {-1, -1};
long RELOAD[2] = {-1, ROUNDS_Y};

MultiStepper motors;
AccelStepper motor_horizontal(1, STEP_H, DIR_H);
AccelStepper motor_vertical(1, STEP_V, DIR_V);

static Metro timer = Metro(0);
//static Metro gameTimer = Metro((2*60 + 10)*1000); // 2 min, 10 sec
IntervalTimer endTimer;

void setup() {
  Serial.begin(9600);
  motor_horizontal.setMaxSpeed(MAX_SPEED);
  motor_vertical.setMaxSpeed(MAX_SPEED);
  
  motor_horizontal.setAcceleration(MAX_ACCEL);
  motor_vertical.setAcceleration(MAX_ACCEL);

  motors.addStepper(motor_horizontal);
  motors.addStepper(motor_vertical);
  motors.setMaxSpeed(MAX_SPEED);

  pinMode(STEP_H, OUTPUT);
  pinMode(DIR_H, OUTPUT);
  pinMode(STEP_V, OUTPUT);
  pinMode(DIR_V, OUTPUT);

  latch.attach(LATCH_SERVO_PIN);
  latch.write(180);

  tilt.attach(TILT_SERVO_PIN);
  moveTilt(TILT_UP);

  endTimer.begin(endGame, (2*60 + 10)*1000000);

  resetOrigin();
  delay(500);
}

void endGame(){
  while(true);
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

//input DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, and distance that needs to travel in inches
void driveToDistance(int dir, int distance) {
  int steps = STEPS_PER_INCH * distance;
  switch(dir){
    case DIR_RIGHT:
    {
      long right[2] = {-steps, 0};
      motors.move(right);
    }
      break;
    case DIR_LEFT:
    {
      long left[2] = {steps, 0};
      motors.move(left);
    }
      break;
    case DIR_UP:
    {
      long up[2] = {0, steps};
      motors.move(up);
    }
      break;
    case DIR_DOWN:
    {
      long down[2] = {0, -steps};
      motors.move(down);
    }
      break;
    default:
      Serial.println("driveTo input error!");
      break;
  }
}

void driveToCoordinate(int x, int y) {
//    motor_horizontal.moveTo(STEPS_PER_INCH * x);
//    motor_vertical.moveTo(STEPS_PER_INCH * y);
  long pos[] = {x*STEPS_PER_INCH, y*STEPS_PER_INCH};
  motors.moveTo(pos);
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
//  return (motor_horizontal.distanceToGo() == 0 &&
//          motor_vertical.distanceToGo() == 0);
  return (!motors.run());
}

void babysit() {
//  motor_horizontal.run();
//  motor_vertical.run();
  motors.run();
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
  driveToDistance(DIR_LEFT, INCHES_START_TO_A - 8);
  while(!driveDoneMoving()) babysit();
  driveToDistance(DIR_UP, 5);
  while(!driveDoneMoving()) babysit();
}

void tiltAndDropBalls(int numBalls) {
  // Tilt down
  moveTilt(TILT_DOWN);
  while(!waitForTime(500)) babysit();

  // Drop balls
  dropBalls(numBalls);

  // Tilt up
  while(!waitForTime(1500)) babysit();
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
  while(!waitForTime(RELOAD_TIME)) babysit();
}

void goToGate() {
  driveToCoordinate(GATE_X, ROUNDS_Y - 5);
  while(!driveDoneMoving()) babysit();
}

void loop() {
  // Windup for bump
  driveToDistance(DIR_RIGHT, 10);
  while(motors.run());
  driveToDistance(D IR_LEFT, 30);
  while(motors.run());
  
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
  dropBalls(4);

  moveToRound_Absolute(ROUND_B_X);
  tiltAndDropBalls(0);

  while(true) {
    moveToReload_Absolute();
    
    moveToRound_Absolute(PO_X);
    dropBalls(4);

    moveToRound_Absolute(ROUND_A_X);
    tiltAndDropBalls(0);
  }
}
