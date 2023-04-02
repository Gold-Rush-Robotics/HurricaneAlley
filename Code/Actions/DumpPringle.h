#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef DUMPPRINGLEH
#define DUMPPRINGLEH

class DumpPringle : public GoldRushAction {
    public:
        DumpPringle();
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif