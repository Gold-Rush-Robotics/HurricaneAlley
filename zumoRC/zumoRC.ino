#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;

int lDirPin = 1;
int lPwmPin = 13;
int rDirPin = 0;
int rPwmPin = 14;

void setup() {
  // put your setup code here, to run once:
 pinMode(lDirPin, INPUT);
 pinMode(lPwmPin, INPUT);
 pinMode(rDirPin, INPUT);
 pinMode(rPwmPin, INPUT);

 Serial.begin(9600);
}

int leftSpeed = 200;
int rightSpeed = 200;

static double MAX_SPEED = 400;
static double MAX_PWM = 8000;

void loop() {
  // put your main code here, to run repeatedly:
  int lDir = digitalRead(lDirPin);
  int lPwm = pulseIn(lPwmPin, HIGH);
  int rDir = digitalRead(rDirPin);
  int rPwm = pulseIn(rPwmPin, HIGH);
  
  Serial.println("x: " + String(lDir) + " , " + String(lPwm) + " H: " + String(rDir) + " , " + String(rPwm));

  leftSpeed = ((lDir > 0) ? 1 : -1) * (int)((double)lPwm * (MAX_SPEED / MAX_PWM));
  rightSpeed = ((rDir > 0) ? 1 : -1) * (int)((double)rPwm * (MAX_SPEED / MAX_PWM));

  Serial.println("L: " + String(leftSpeed) + " R: " + String(rightSpeed));
  
  motors.setSpeeds(leftSpeed, rightSpeed);
}
