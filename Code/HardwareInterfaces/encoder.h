#include <stdint.h>
#include <memory>
#include "utilFunc.h"
#include "bcm2835.h"

class EncoderHandler{
    public:
        void init();
        long getPos(int encoder);
        int32_t* getValues();
        void printReadable();
        bool resetPositions();
    private:
        
};