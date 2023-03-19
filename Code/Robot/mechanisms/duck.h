#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/digitalsensor.h"
#include "pca9685.h"

class Duck{
    public:
        Duck(std::shared_ptr<PCA9685> pca);

        enum ELBOW_POSITIONS{
            AGITATOR,
            STORE_1,
            STORE_2,
            STACK,
            RAISED
        };

        enum SHOULDER_POSITIONS{
            AGITATOR,
            STORE_1,
            STORE_2,
            STACK
        };

        enum WAIST_POSITIONS{
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

        void elbow_servo(ELBOW_POSITIONS state);
        void shoulder_servo(SHOULDER_POSITIONS state);
        void waist_servo(WAIST_POSITIONS state);

    private:
        std::shared_ptr<Servo> gripper, elbow, shoulder, waist;
        std::shared_ptr<Motor> agitator;
        DigitalSensor* hall_effect;
}