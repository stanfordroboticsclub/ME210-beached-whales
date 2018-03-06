#ifndef Task_h
#define Task_h

class Task
{
public:
	virtual void setup();
	virtual void run();
	virtual bool isDone();	
};

#endif