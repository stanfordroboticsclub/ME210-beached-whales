#include "Arduino.h"
#include <AccelStepper.h>

#define STEP_VL   14
#define DIR_VL    15
#define STEP_VR   16
#define DIR_VR    17

#define STEP_HL   18
#define DIR_HL    19
#define STEP_HR   20
#define DIR_HR    21

class Drivetrain
{
public:
	Drivetrain(int maxSpeed, int maxAccel);
	void setSteps(int direction, int steps);
	void setInches(int direction, int inches);
  void move();
	bool isDoneMoving();

	static const int FRONT = 0;
	static const int RIGHT = 1;
	static const int BACK  = 2;
	static const int LEFT  = 3;

private:
  static const int STEPS_PER_INCH = 80;
};

AccelStepper motor_horizontalL(1, STEP_HL, DIR_HL);
AccelStepper motor_horizontalR(1, STEP_HR, DIR_HR);
AccelStepper motor_verticalL(1, STEP_VL, DIR_VL);
AccelStepper motor_verticalR(1, STEP_VR, DIR_VR);
