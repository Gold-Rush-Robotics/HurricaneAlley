#include <stdint.h>
#include <memory>
#include "../Robot/mechanisms/revolver.h"

#ifndef COLORSENSORH
#define COLORSENSORH

class ColorSensor{
    public:
        ColorSensor();
        MARSHMALLOWS get_color();
        bool is_start_light();
        
};

#endif