#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/encoder.h"
#include "../../HardwareInterfaces/colorsensor.h"
#include "../../Utilities/utilFunc.h"
#include "../../Utilities/pid.h"

#include "pca9685.h"

#ifndef RevolverH
#define RevolverH

class Revolver{
    public:
        
        enum PRINGLE_STATES {
            CLOSED,
            ACCEPTING,
            OPEN
        };

        Revolver(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> h);
        
        void turn_on_agitator();
        void turn_off_agitator();
        
        void drop_servo(bool down);
        void pringle_servo(PRINGLE_STATES state);

        bool get_finger_in_revolver();

        bool store_marshmallow();
        bool load_marshmallow(MARSHMALLOWS color);
        bool rotate_revolver(double pos);
        void rotate_speed(double speed);
        void retract_loader();
        void insert_loader();
        MARSHMALLOWS get_color();
    
    private:
        std::shared_ptr<EncoderHandler> enc;
        std::shared_ptr<Servo> loader, opener, dropper;
        std::shared_ptr<Motor> agitator, motor_revolver;
        
        // For now setting initial positions of food chips in here
        // Could probably do it with teleop at the start if we wanted
        MARSHMALLOWS revolver[5] = {RED_CHIPS, GREEN_CHIPS, EMPTY, EMPTY, EMPTY};
        MARSHMALLOWS can[3] = {EMPTY, EMPTY, EMPTY};
        int32_t chambers[5] = {1792, 2534, 3254, 320, 1024};    //Encoder values for each chamber position basing it off of the loader for the pringle can
        
        
        int get_color_pos(MARSHMALLOWS color);

        int32_t encoder_ticks;
        int agitator_mod = -1792;
        int check_can;

        bool dropper_up;
        bool is_open;
        bool finger_in_revolver;

        PID* revolverPID;

        ColorSensor* colorsensor;
};

#endif