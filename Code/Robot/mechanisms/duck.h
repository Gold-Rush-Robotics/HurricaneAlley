#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/digitalsensor.h"
#include "pca9685.h"

class Duck{
    public:
        Duck(std::shared_ptr<PCA9685> pca);

        // "Elbow"
        // Makes arm travel along z-axis
        enum J3_POSITIONS{
            AGITATOR,
            STORE_1,
            STORE_2,
            STACK_2,
            STACK_3,
            RAISED
        };

        // "Shoulder"
        // Makes arm travel along a vertical arc
        enum J2_POSITIONS{
            AGITATOR,
            STORE_1,
            STORE_2,
            STACK,
            RAISED
        };

        // "Waist"
        // Rotates the whole arm along z axis
        enum J1_POSITIONS{
            AGITATOR,
            STORE_1,
            STORE_2,
            STACK
        };

        void grip_duck();
        void release_duck();

        void turn_on_agitator();
        void turn_off_agitator();
        bool home_agitator();

        uint8_t stored_ducks;

        void j3_servo(J3_POSITIONS state);
        void j2_servo(J2_POSITIONS state);
        void j1_servo(J1_POSITIONS state);

    private:
        std::shared_ptr<Servo> gripper, j1, j2, j3;
        std::shared_ptr<Motor> agitator;
        DigitalSensor* hall_effect;
}