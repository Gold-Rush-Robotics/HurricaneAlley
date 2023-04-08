#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"
#include "GoToStack.h"
#include "GoToStore1.h"
#include "GoToStore2.h"
#include "GoToAgitator.h"
#include "StackAction.h"

#ifndef PLACEACTIONH
#define PLACEACTIONH

class PlaceAction : public GoldRushAction {
    public:
        PlaceAction(StackAction::StackHeight size);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        int size;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
        GoToAgitator* ag;
        GoToStore1* go_to_store_1;
        GoToStore2* go_to_store_2;
        GoToStack* go_to_stack;
};

#endif