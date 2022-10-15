#include "encoder.h";
#include "pigpio.h";

Encoder::Encoder(int aPin, int bPin){
    Encoder::aPin = aPin;
    Encoder::bPin = bPin;
    gpioSetMode(aPin, PI_INPUT);
    gpioSetMode(bPin, PI_INPUT);
}
/**DONT UNDERSTAND... THEORETICALLY WORKS*/
void Encoder::isr(){
    uint8_t p1val = gpioRead(aPin);
    uint8_t p2val = gpioRead(bPin);
    uint8_t s = state & 3;
    if (p1val) s |= 4;
    if (p2val) s |= 8;
    state = (s >> 2);
    
    switch (s) {
        case 1: case 7: case 8: case 14:
            pos++;
            return;
        case 2: case 4: case 11: case 13:
            pos--;
            return;
        case 3: case 12:
            pos += 2;
            return;
        case 6: case 9:
            pos -= 2;
            return;
    }
}

long Encoder::getPos(){
    return pos;
}