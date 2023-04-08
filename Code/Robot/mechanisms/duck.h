#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/digitalsensor.h"
#include "pca9685.h"

#ifndef DUCKH
#define DUCKH

class Duck{
    public:
        Duck(std::shared_ptr<PCA9685> pca);

        enum JOINT_POSITIONS{
            AGITATOR,
            STORE_1,
            STORE_2,
            STACK,
            STACK_2,
            STACK_3,
            RAISED
        };


        void grip_duck();
        void release_duck();

        void turn_on_agitator();
        void turn_off_agitator();
        bool home_agitator();

        uint8_t stored_ducks = 0;

        void j3_servo(JOINT_POSITIONS state);
        void j2_servo(JOINT_POSITIONS state);
        void j1_servo(JOINT_POSITIONS state);

    private:
        std::shared_ptr<Servo> gripper, j1, j2, j3;
        std::shared_ptr<Motor> agitator;
        DigitalSensor* hall_effect;
};

#endif