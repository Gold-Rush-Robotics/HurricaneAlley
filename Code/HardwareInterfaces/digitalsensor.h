#include <stdint.h>
#include <memory>
#include "../Utilities/utilFunc.h"
#include "bcm2835.h"

#ifndef DIGITALSENSORH
#define DIGITALSENSORH

class DigitalSensor{
    public:
        DigitalSensor(int pin, bool pull_up);
        bool read_sensor();
    private:
        bool trigger;
        int pin;
};

#endif