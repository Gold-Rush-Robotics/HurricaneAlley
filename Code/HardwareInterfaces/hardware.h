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

class Motor{
    public:
        PCA9685* pca9685;
        int directionPin = 0;
        int speedPin = 0;
        bool reversed;
        void init(int directionPin, int speedPin);
        void reverse();
        bool isReversed();
        void setPower(double power, PCA9685* pca9685A);
};