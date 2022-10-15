#include <stdint.h>

class Encoder{
    public:
    int aPin;
    int bPin;
    volatile long pos = 0;
    volatile uint8_t state;
    Encoder(int aPin, int bPin);
    Encoder();
    void isr();
    long getPos();
};