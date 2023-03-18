#include <stdint.h>
#include <memory>
#include "../Utilities/utilFunc.h"
#include "bcm2835.h"

class DigitalSensor{
    public:
        DigitalSensor(int pin, bool pull_up);
        bool read_sensor();
    private:
        bool trigger;
        int pin;
}