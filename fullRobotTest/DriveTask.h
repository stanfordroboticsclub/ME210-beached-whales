#include "Task.h"
//#include "Drivetrain.h"

class DriveTask: public Task
{
public:
  DriveTask(int direction, int inches); //Drivetrain drive, 
  void setup();
  void run();
  bool isDone();

  enum Direction {
    FRONT, RIGHT, BACK, LEFT,
  };

private:
  static const int STEPS_PER_INCH = 80;
};
