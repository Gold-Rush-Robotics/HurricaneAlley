#include "drivetrain.h"
#include <iostream>

class Robot{
    public:
        void start();
        void driveController(double s1y, double s1x, double s2y, double s2x, double multiplier);
        void stop();
    private:
        Drivetrain* drivetrain;
};