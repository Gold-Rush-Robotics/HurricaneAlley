#include <stdint.h>
#include <memory>
#include "utilFunc.h"
#include "bcm2835.h"
#include "pca9685.h"

class Encoder{
    public:
        int aPin;
        int bPin;
        volatile long pos = 0;
        volatile uint8_t state;
        void init(int aPin, int bPin);
        void isr();
        long getPos();
};