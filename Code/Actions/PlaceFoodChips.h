#include "GoldRushAction.h"

#ifndef PLACEFOODCHIPSH
#define PLACEFOODCHIPSH

class PlaceFoodChips : public GoldRushAction {
    public:
        enum CHIPCOLORS {
            RED,
            GREEN
        };
        PlaceFoodChips(CHIPCOLORS color);
        GoldRushAction* run(Robot* robot);
    private:
        CHIPCOLORS color;
};

#endif