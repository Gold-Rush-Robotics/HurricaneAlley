#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"
#include "GoToStore1.h"
#include "GoToStore2.h"
#include "GoToAgitator.h"
#include "GoToStack.h"

#ifndef PLACEDUCKH
#define PLACEDUCKH

class PlaceDuck : public GoldRushAction {
    public:
        PlaceDuck(double seconds, int height);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
        GoldRushAction* delay4;
        GoToAgitator* ag;
        GoToStore2* go_to_store_2;
        GoToStore1* go_to_store_1;
        GoToStack*  go_to_stack;
};

#endif