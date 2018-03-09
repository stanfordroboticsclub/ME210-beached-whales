#define MOTOR_PIN 12
#define BUTTON_PIN 2
#define DUTY 130
#define DRIVE_DELAY 10
#define DEBOUNCE_DELAY 100

void setup() {
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(MOTOR_PIN,OUTPUT);
  analogWrite(MOTOR_PIN,DUTY);

  while(digitalRead(BUTTON_PIN) ) {}
  delay(DEBOUNCE_DELAY);
  while(!digitalRead(BUTTON_PIN) ) {}
  delay(DRIVE_DELAY);

  analogWrite(MOTOR_PIN,0);

}

void loop() {

}
