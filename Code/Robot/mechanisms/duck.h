#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/digitalsensor.h"
#include "pca9685.h"

class Duck{
    public:
        Duck(std::shared_ptr<PCA9685> pca);

        void grip_duck();
        void release_duck();

        void turn_to_agitator();
        void turn_to_space_1();
        void turn_to_space_2();
        void turn_to_stack();

        void move_elbow(int pos);
        void move_shoulder(int pos);

        void turn_on_agitator();
        void turn_off_agitator();
        bool home_agitator();

    private:
        std::shared_ptr<Servo> gripper, elbow, shoulder, waist;
        std::shared_ptr<Motor> agitator;
        DigitalSensor* hall_effect;
}