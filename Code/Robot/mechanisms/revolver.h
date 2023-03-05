#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/encoder.h"
#include "../../Utilities/utilFunc.h"
#include "pca9685.h"

#ifndef RevolverH
#define RevolverH

class Revolver{
    public:
        Revolver(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> h);
        
        void turn_on_agitator();
        void turn_off_agitator();
        
        void toggle_drop_servo();
        void toggle_open_servo();

        bool get_finger_in_revolver();

        enum MARSHMALLOWS { 
            EMPTY,
            RED, 
            GREEN, 
            WHITE
        };

        bool store_marshmallow(MARSHMALLOWS color);
        int load_marshmallow(MARSHMALLOWS color);
        bool rotate_revolver(double pos);
        void rotate_speed(double speed);
        void retract_loader();
        void insert_loader();

    
    private:
        std::shared_ptr<EncoderHandler> enc;
        std::shared_ptr<Servo> loader, opener, dropper;
        std::shared_ptr<Motor> agitator, motor_revolver;
        
        MARSHMALLOWS revolver[5] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
        MARSHMALLOWS can[3] = {EMPTY, EMPTY, EMPTY};
        int32_t chambers[5] = {0, 1, 2, 3, 4};    //Encoder values for each chamber position basing it off of the loader for the pringle can
        
        
        int get_color_pos(MARSHMALLOWS color);

        int32_t encoder_ticks;
        int agitator_mod;
        int check_can;

        bool dropper_up;
        bool is_open;
        bool finger_in_revolver;

        PID* revolverPID;
};

#endif