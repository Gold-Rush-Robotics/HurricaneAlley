#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef PLACEACTIONH
#define PLACEACTIONH

class PlaceAction : public GoldRushAction {
    public:
        PlaceAction();
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif