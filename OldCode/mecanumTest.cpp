#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <algorithm>
#include <softPwm.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <cstdio>
#include <cstdint>
#include <signal.h>

using namespace std;

#include "ps5Controller.cpp"

#include "bcm2835.h"
#include "pca9685.h"

// compiler command
// g++ -o mecanumTest mecanumTest.cpp -lpthread -I "../../rpidmx512/lib-pca9685/include/" -L "../../rpidmx512/lib-pca9685/lib_linux/" -l pca9685 -l bcm2835; sudo ./mecanumTest

#define NORM_SPEED 0.5
#define SLOW_SPEED 0.3
#define FAST_SPEED 1.0

// Motor layout as follows: direction pin, pwm channel, reverser
static int motorPins[4][3] = {{4, 0, -1}, {17, 1, 1}, {22, 3, -1}, {27, 2, -1}};
static int motorCount = 4;

// Pink zip tie is front, Purple zip tie is back
enum motor
{
    motorFL = 0,
    motorFR = 1,
    motorBL = 2,
    motorBR = 3
};


volatile sig_atomic_t flag = 1;
void onInterupt(int sig){
    flag = 0;
}


// Encoder layout as follows: encoder A pin, encoder B pin
static int odoPins[3][2] = {{21, 20}, {16, 12}, {25, 24}};
static int odoCount = 3;

enum odo {
    odoL = 0,
    odoR = 1,
    odoB = 2
};

// Reads controller event and insures it is of correct type
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

// Stores commanded mecanum movement vector
struct mecanumVector
{
    double x, y, h;
};

// Keeps value between minimum and maximum
double constrain(double value, double min, double max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

volatile long pos0 = 0;
volatile uint8_t state0;
volatile long pos1 = 0;
volatile uint8_t state1;
volatile long pos2 = 0;
volatile uint8_t state2;

// Interupt Service Routine for encodder #0
/**void pin_isr0(void)
{
    uint8_t p1val = digitalRead(odoPins[0][0]);
    uint8_t p2val = digitalRead(odoPins[0][1]);
    uint8_t s = state0 & 3;
    if (p1val)
        s |= 4;
    if (p2val)
        s |= 8;
    state0 = (s >> 2);

    switch (s)
    {
    case 1:
    case 7:
    case 8:
    case 14:
        pos0++;
        return;
    case 2:
    case 4:
    case 11:
    case 13:
        pos0--;
        return;
    case 3:
    case 12:
        pos0 += 2;
        return;
    case 6:
    case 9:
        pos0 -= 2;
        return;
    }
}

// Interupt Service Routine for encodder #1
void pin_isr1(void)
{
    uint8_t p1val = digitalRead(odoPins[1][0]);
    uint8_t p2val = digitalRead(odoPins[1][1]);
    uint8_t s = state1 & 3;
    if (p1val)
        s |= 4;
    if (p2val)
        s |= 8;
    state1 = (s >> 2);

    switch (s)
    {
    case 1:
    case 7:
    case 8:
    case 14:
        pos1++;
        return;
    case 2:
    case 4:
    case 11:
    case 13:
        pos1--;
        return;
    case 3:
    case 12:
        pos1 += 2;
        return;
    case 6:
    case 9:
        pos1 -= 2;
        return;
    }
}

// Interupt Service Routine for encodder #2
void pin_isr2(void)
{
    uint8_t p1val = digitalRead(odoPins[2][0]);
    uint8_t p2val = digitalRead(odoPins[2][1]);
    uint8_t s = state2 & 3;
    if (p1val)
        s |= 4;
    if (p2val)
        s |= 8;
    state2 = (s >> 2);

    switch (s)
    {
    case 1:
    case 7:
    case 8:
    case 14:
        pos2++;
        return;
    case 2:
    case 4:
    case 11:
    case 13:
        pos2--;
        return;
    case 3:
    case 12:
        pos2 += 2;
        return;
    case 6:
    case 9:
        pos2 -= 2;
        return;
    }
}**/

struct mecanumVector mVec, position;

long oldLPos = 0, currLPos = 0, oldRPos = 0, currRPos = 0, oldBPos = 0, currBPos = 0;

// Update Values --Specifically Wheel radius, L and B
static double ODO_R = 1.903190; // Wheel radius in cm
static double ODO_N = 8192;     // Encoder ticks per Revolution
static double CM_PER_TICK = 2.0 * M_PI * ODO_R / ODO_N;

static double ODO_L = 18.225; // Tuneable Distance between Left and Right Encoders
static double ODO_B = 9.5;    // Tuneable Distance between Back Encoder to center of robot (only in the X direction)

/**void *odometry(void *ignore)
{
    // Sets up pin inputs for odometry
    // Odometer 0 Setup (Right)
    pinMode(odoPins[0][0], INPUT);
    pinMode(odoPins[0][1], INPUT);
    pullUpDnControl(odoPins[0][0], PUD_UP);
    pullUpDnControl(odoPins[0][1], PUD_UP);
    wiringPiISR(odoPins[0][0], INT_EDGE_BOTH, &pin_isr0);
    wiringPiISR(odoPins[0][1], INT_EDGE_BOTH, &pin_isr0);

    // Odometer 1 Setup (Left)
    pinMode(odoPins[1][0], INPUT);
    pinMode(odoPins[1][1], INPUT);
    pullUpDnControl(odoPins[1][0], PUD_UP);
    pullUpDnControl(odoPins[1][1], PUD_UP);
    wiringPiISR(odoPins[1][0], INT_EDGE_BOTH, &pin_isr1);
    wiringPiISR(odoPins[1][1], INT_EDGE_BOTH, &pin_isr1);

    // Odometer 2 Setup (Back)
    pinMode(odoPins[2][0], INPUT);
    pinMode(odoPins[2][1], INPUT);
    pullUpDnControl(odoPins[2][0], PUD_UP);
    pullUpDnControl(odoPins[2][1], PUD_UP);
    wiringPiISR(odoPins[2][0], INT_EDGE_BOTH, &pin_isr2);
    wiringPiISR(odoPins[2][1], INT_EDGE_BOTH, &pin_isr2);

    while (true)
    {
        oldLPos = currLPos;
        oldRPos = currRPos;
        oldBPos = currBPos;

        currRPos = pos0;
        currLPos = pos1;
        currBPos = pos2;

        // Delta change in odometers since last loop
        long dnR = currRPos - oldRPos;
        long dnL = currLPos - oldLPos;
        long dnH = currBPos - oldBPos;

        // Convert odometer changes to locally referenced position change
        double dT = CM_PER_TICK * (dnR - dnL) / ODO_L;
        double dX = CM_PER_TICK * (dnR + dnL) / 2;
        double dY = CM_PER_TICK * (dnH + (((dnR - dnL) / ODO_L) * ODO_B));

        // Takes average heading change so that we get the most accurate change
        double tempH = position.h + (dT / 2.0);

        // Calculate triginometric functions once
        double cosTempH = cos(tempH);
        double sinTempH = sin(tempH);

        // Convert locally referenced position change to globally referenced position
        position.x += dX * cosTempH - dY * sinTempH;
        position.y += dX * sinTempH + dY * cosTempH;
        position.h += dT;
    }
}**/

// Sets power to motor using dir pin and pwm pin
void setPower(int motorNum, double power, PCA9685 pca9685)
{
    double absPower = (double)abs(power);
    int direct = (((double)motorPins[motorNum][2] * power) > 0) ? 1 : 0;

    //printf("motor: %.1d direct: %.1d power: %f pwmChannel: %d\n", motorNum, direct, power, motorPins[motorNum][1]);

    // digitalWrite(motorPins[motorNum][0], direct);
    if (direct) {
        //printf("setting pin #%d to HIGH\n", motorPins[motorNum][0]);
        bcm2835_gpio_set(motorPins[motorNum][0]);
    }
    else {
        //printf("setting pin #%d to LOW\n", motorPins[motorNum][0]);
        bcm2835_gpio_clr(motorPins[motorNum][0]);
    }
    pca9685.Write(CHANNEL(motorPins[motorNum][1]), VALUE(absPower * (PCA9685_VALUE_MAX - 1)));
    // analogWrite(motorPins[motorNum][1], absPower);
    // softPwmWrite(motorPins[motorNum][1], absPower);
}

// Converts forward, strafe, and turn commands into motor powers then sets powers to motors
void drive(double forward, double strafe, double turn, PCA9685 pwmHat)
{
    double powerFL = forward + strafe - turn;
    double powerFR = forward - strafe + turn;
    double powerBL = forward - strafe - turn;
    double powerBR = forward + strafe + turn;

    double powerMax = max(abs(powerFL), max(abs(powerFR), max(abs(powerBL), abs(powerBR))));

    if (powerMax > 1.0)
    {
        powerFL /= powerMax;
        powerFR /= powerMax;
        powerBL /= powerMax;
        powerBR /= powerMax;
    }

    powerFL = constrain(powerFL, -1.0, 1.0);
    powerFR = constrain(powerFR, -1.0, 1.0);
    powerBL = constrain(powerBL, -1.0, 1.0);
    powerBR = constrain(powerBR, -1.0, 1.0);

    // printf("\n\n\nfl: %.2f fr: %.2f\n\nbl: %.2f br: %.2f\n", powerFL, powerFR, powerBL, powerBR);

    setPower(motorFL, powerFL, pwmHat);
    setPower(motorFR, powerFR, pwmHat);
    setPower(motorBL, powerBL, pwmHat);
    setPower(motorBR, powerBR, pwmHat);
}

void *printPeriodically(void *ignore)
{
    while (true)
    {
        sleep(1);
        printf("posX: %f posY: %f posH: %f\n", position.x, position.y, position.h);
    }
}

int main(int argc, char *argv[]) {
    const char *device;
    int js;
    struct js_event event;
    ps5Controller ctr = ps5Controller();
    pthread_t odoTID;
    pthread_t printTID;
    signal(SIGINT, onInterupt); 


    // Set up pwm hat connection
    if (getuid() != 0)
    {
        fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
        return -1;
    }

    if (bcm2835_init() != 1)
    {
        fprintf(stderr, "bcm2835_init() failed\n");
        return -2;
    }

    PCA9685 pca9685;
    pca9685.Dump();
    pca9685.SetFrequency(1000);

    // Sets up pinouts for drivebase
    for (int i = 0; i < motorCount; i++) {
        bcm2835_gpio_fsel(motorPins[i][0], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_set_pud(motorPins[i][0], BCM2835_GPIO_PUD_DOWN);
    }

    // Sets up pin inputs for odometry
    for (int i = 0; i < odoCount; i++) {
        bcm2835_gpio_fsel(odoPins[i][0], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(odoPins[i][0], BCM2835_GPIO_PUD_DOWN);

        bcm2835_gpio_fsel(odoPins[i][1], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(odoPins[i][1], BCM2835_GPIO_PUD_DOWN);
    }

    /* Create odometry thread. Things to consider: If we get corrupted values from odo and drive accessing position at same time we
    can add semaphores to block off the variable while its being accessed. Can also add core affinity to allow odo to favor a less
    utilized core if necessary.*/
    //pthread_create(&odoTID, NULL, odometry, (void *)NULL);

    // Thread for printing things without having to be in the loop where they print so fast they are cut off
    pthread_create(&printTID, NULL, printPeriodically, (void * )NULL);

    // Selects correct controller device
    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1) {
        perror("Could not open joystick");
        flag = 0;
    }


    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0 && flag) {
        ctr.eventHandler(&event);

        // ctr.printStates();

        double multiplier = NORM_SPEED;
        if (ctr.lBump) {
            multiplier = FAST_SPEED;
        }
        else if (ctr.lTrigBool) {
            multiplier = SLOW_SPEED;
        }

        mVec.x = multiplier * ctr.ly;   // Drive
        mVec.y = multiplier * ctr.lx;   // Strafe
        mVec.h = multiplier * -ctr.rx;  // Turn

        drive(mVec.x, mVec.y, mVec.h, pca9685);

        // printf("x: %.2f y: %.2f h: %.2f\n", mVec.x, mVec.y, mVec.h);

        fflush(stdout);
    }

    for (int i = 0; i < motorCount; i++) {
        setPower(i, 0, pca9685);
    }
    pca9685.Dump();
    printTID.~pthread_t();

}