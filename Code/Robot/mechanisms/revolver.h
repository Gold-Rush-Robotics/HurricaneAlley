#include <iostream>
#include "../../HardwareInterfaces/servo.h"
#include "../../HardwareInterfaces/motor.h"
#include "../../HardwareInterfaces/encoder.h"
#include "../../Utilities/utilFunc.h"
#include "pca9685.h"


class Revolver{
    public:
        Revolver(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> h);
        
        int turn_on_agitator();
        int turn_off_agitator();
        
        int release_can();
        int store_can();

        int store_marshmallow(MARSHMALLOWS color);
        int load_marshmallow(MARSHMALLOWS color);
        int load_marshmallow_stack(int stack_size);

        enum MARSHMALLOWS { 
            EMPTY,
            RED, 
            GREEN, 
            WHITE
        };
    
    private:
        std::shared_ptr<EncoderHandler> enc;
        std::shared_ptr<Servo> loader, opener, dropper;
        std::shared_ptr<Motor> agitator, motor_revolver;
        
        MARSHMALLOWS revolver[5], can[3];
        int32_t chambers[5];    //Encoder values for each chamber position basing it off of the loader for the pringle can
        
        int rotate_revolver(int pos);
        int get_current_chamber();
        int contains_color(MARSHMALLOWS color);

        int32_t encoder_ticks;
        int check_can;

};