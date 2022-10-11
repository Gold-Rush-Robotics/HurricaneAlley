#include <stdint.h>

class Encoder{
    int aPin;
    int bPin;
    volatile long pos = 0;
    volatile uint8_t state;
    Encoder(int aPin, int bPin);
    void isr();
    long getPos();
};