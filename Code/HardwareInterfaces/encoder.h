#include <stdint.h>
#include <memory>
#include "utilFunc.h"
#include "bcm2835.h"
#ifndef ENCODERHANDLER_H
#define ENCODERHANDLER_H
class EncoderHandler{
    public:
        void init();
        int32_t getPos(int encoder);
        int32_t* getValues();
        void printReadable();
        bool resetPositions();
    private:
        
};
#endif