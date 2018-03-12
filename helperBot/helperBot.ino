#define MOTOR_PIN 3
#define BUTTON_PIN A5
#define DUTY 90
#define DRIVE_DELAY 10
#define DEBOUNCE_DELAY 500

void setup() {
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(MOTOR_PIN,OUTPUT);
  analogWrite(MOTOR_PIN,DUTY);

  while(digitalRead(BUTTON_PIN) ) {}

  delay(DRIVE_DELAY);


  analogWrite(MOTOR_PIN,0);

  delay(DEBOUNCE_DELAY);


  while(!digitalRead(BUTTON_PIN) ) {}

  analogWrite(MOTOR_PIN,120);

  delay(200);

  analogWrite(MOTOR_PIN,0);


  //  delay(DEBOUNCE_DELAY);
//  int opened = 1;
//  while(opened){
//    opened = 0;
//    unsigned int t = millis();
//    while(millis() - t < DEBOUNCE_DELAY){
//      opened |= !digitalRead(BUTTON_PIN);
//    }
//  }

}

void loop() {

}
