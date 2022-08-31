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

#include "ps5Controller.cpp"

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

//Reads controller event and insures it is of correct type
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

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
    const char *device;
    int js;
    struct js_event event;
    ps5Controller ctr = ps5Controller();
    pthread_t printTID;

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

    //Selects correct controller device
    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0) {
        ctr.eventHandler(&event);
        
        //ctr.printStates();

        double multiplier = NORM_SPEED;
        if(ctr.lBump) {
            multiplier = FAST_SPEED;
        } else if(ctr.lTrigBool) {
            multiplier = SLOW_SPEED;
        }

        mVec.y = multiplier * ctr.lx;
        mVec.x = multiplier * ctr.ly;
        mVec.h = multiplier * -ctr.rx;

        drive(mVec.x, 0.0, mVec.h);     

        printf("x: %.2f y: %.2f h: %.2f\n", mVec.x, mVec.y, mVec.h);

        fflush(stdout);
    }
}
