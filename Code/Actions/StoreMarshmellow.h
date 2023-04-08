#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef STOREMARSHMALLOWH
#define STOREMARSHMALLOWH

class StoreMarshmellow : public GoldRushAction {
    public:
        StoreMarshmellow(MARSHMALLOWS color);
        GoldRushAction* run(Robot* robot);
    private:
        MARSHMALLOWS color;

};

#endif