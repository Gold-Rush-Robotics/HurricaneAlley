#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <algorithm>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>

using namespace std;

// how to compile: gcc square.cpp -o square -lwiringPi -lpthread

#define NORM_SPEED 1.0
#define SLOW_SPEED 0.3
#define FAST_SPEED 0.5

// Motor layout as follows: direction pin, speed pin, reverser
static int motorPins[4][3] = {{0, 4, 1}, {2, 5, 1}};
static int motorCount = 2;

//Pink zip tie is front, Purple zip tie is back
enum motor {
    motorL = 0,
    motorR = 1,
};



//Stores commanded mecanum movement vector
struct mecanumVector {
    double x, y, h;
};

//Keeps value between minimum and maximum
double constrain(double value, double min, double max) {
    if(value < min) 
        return min;
    if(value > max)
        return max;
    return value;
}

struct mecanumVector mVec, position;

//Sets power to motor using dir pin and pwm pin
void setPower(int motorNum, double power) {
    int absPower = (int)abs(power * 80.0);
    int direct = (((float)motorPins[motorNum][2] * power) > 0) ? 1 : 0;

    if(absPower == 0) {
        absPower = 1;
    }

    //printf("motor: %.1d direct: %.1d power: %d\n", motorNum, direct, absPower);

    digitalWrite(motorPins[motorNum][0], direct);
    //analogWrite(motorPins[motorNum][1], absPower);
    softPwmWrite(motorPins[motorNum][1], absPower);
}

//Converts forward, strafe, and turn commands into motor powers then sets powers to motors
void drive(double forward, double strafe, double turn) {
  double powerL = forward - turn;
  double powerR = forward + turn;

  double powerMax = max(abs(powerL), abs(powerR));

  if (powerMax > 1.0) {
    powerL /= powerMax;
    powerR /= powerMax;
  }

  powerL = constrain(powerL, -1.0, 1.0);
  powerR = constrain(powerR, -1.0, 1.0);

  //printf("\n\n\nfl: %.2f fr: %.2f\n\nbl: %.2f br: %.2f\n", powerFL, powerFR, powerBL, powerBR);

  setPower(motorL, powerL);
  setPower(motorR, powerR);
}

void *printPeriodically(void * ignore) {
    while(true) {
        delay(500);

    }
}

int main(int argc, char *argv[]) {

    pthread_t printTID;
    printf("in code first"); 


    wiringPiSetup();

    // Sets up pinouts for drivebase
    for(int i = 0; i < motorCount; i++) {
        //printf("Creating software PWM #%.1d on pin %.1d\n", i, motorPins[i][1]);
        softPwmCreate(motorPins[i][1], 0, 100);
        pinMode(motorPins[i][0], OUTPUT);
        //pinMode(motorPins[i][1], OUTPUT);
    }

    // Thread for printing things without having to be in the loop where they print so fast they are cut off
    pthread_create(&printTID, NULL, printPeriodically, (void * )NULL);
    drive(0.5, 0.0, 0.0);
    sleep(50);
    drive(0.0,0.0,0.0);
    sleep(1000);
    drive(0.0, 0.0, 1.0);
    sleep(50);
    drive(0.5,0.0, 0.0);
    sleep(100);
    drive(0.0,0.0,0.0);
    




}
