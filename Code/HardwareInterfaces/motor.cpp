
#include <algorithm>;


#include "motor.h";
#include "utilFunc.h";
#include "pigpio.h";

using namespace std;

Motor::Motor(int directionPin, int speedPin){
    Motor::directionPin = directionPin;
    Motor::speedPin = speedPin;
    gpioInitialise();
    gpioSetMode(directionPin, PI_OUTPUT);
    gpioSetMode(speedPin, PI_OUTPUT);
}
void Motor::reverse(){
    reversed = !reversed;
}
bool Motor::isReversed(){
    return reversed;
}
void Motor::setPower(double power){
    power = constrain(-1, 1, power);
    int absPower = int(abs(power));
    int direct = int(reversed) * power > 0 ? 0 : 1;
    gpioWrite(directionPin, direct);
    gpioWrite(speedPin, absPower);
}