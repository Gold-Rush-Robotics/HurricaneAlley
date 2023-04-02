#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"

#include "pca9685.h"

#ifndef INTAKEH
#define INTAKEH

class Intake{
    public:

        Intake(std::shared_ptr<PCA9685> pca);
        
        void turn_on_intake();
        void turn_off_intake();
        
        void intake_marshmallow();
        void intake_duck_agitator();
        void intake_duck_bag();
    
    private:
        std::shared_ptr<Servo> splitter;
        std::shared_ptr<Motor> intake_motor;
};

#endif