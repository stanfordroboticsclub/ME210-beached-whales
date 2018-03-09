//#include "Arduino.h"
//#include "Drivetrain.h"
//
//Drivetrain::Drivetrain(int maxSpeed, int maxAccel) {
//	pinMode(STEP_HL, OUTPUT);
//	pinMode(STEP_HR, OUTPUT);
//	pinMode(DIR_HL, OUTPUT);
//	pinMode(DIR_HR, OUTPUT);
//	pinMode(STEP_VL, OUTPUT);
//	pinMode(STEP_VR, OUTPUT);
//	pinMode(DIR_VL, OUTPUT);
//	pinMode(DIR_VR, OUTPUT);
//
//	motor_horizontalL.setMaxSpeed(maxSpeed);
//	motor_horizontalR.setMaxSpeed(maxSpeed);
//	motor_verticalL.setMaxSpeed(maxSpeed);
//	motor_verticalR.setMaxSpeed(maxSpeed);
//
//	motor_horizontalL.setAcceleration(maxAccel);
//	motor_horizontalR.setAcceleration(maxAccel);
//	motor_verticalL.setAcceleration(maxAccel);
//	motor_verticalR.setAcceleration(maxAccel);
//}
//
//void Drivetrain::setSteps(int direction, int steps) {
//	switch(direction) {
//		case RIGHT:
//			motor_horizontalL.move(steps);
//	    motor_horizontalR.move(-steps);
//	    motor_verticalL.move(0);
//	    motor_verticalR.move(0);
//	    break;
//	  case LEFT:
//			motor_horizontalL.move(-steps);
//	    motor_horizontalR.move(steps);
//	    motor_verticalL.move(0);
//	    motor_verticalR.move(0);
//	    break;
//	  case FRONT:
//			motor_horizontalL.move(0);
//		  motor_horizontalR.move(0);
//		  motor_verticalL.move(steps);
//		  motor_verticalR.move(-steps);
//	    break;
//    case BACK:
//			motor_horizontalL.move(0);
//		  motor_horizontalR.move(0);
//		  motor_verticalL.move(-steps);
//		  motor_verticalR.move(steps);
//	    break;
//	}
//}
//
//void Drivetrain::setInches(int direction, int inches) {
//	setSteps(direction, inches * STEPS_PER_INCH);
//}
//
//void Drivetrain::move() {
//	motor_horizontalL.run();
//	motor_horizontalR.run();
//	motor_verticalL.run();
//	motor_verticalR.run();
//}
//
//bool Drivetrain::isDoneMoving() {
//	return (motor_horizontalR.distanceToGo() == 0 &&
//	      motor_horizontalL.distanceToGo() == 0 &&
//	      motor_verticalR.distanceToGo() == 0 &&
//	      motor_verticalL.distanceToGo() == 0);
//}
