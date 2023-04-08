#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef LOADMARSHMELLOWH
#define LOADMARSHMELLOWH

class LoadMarshmellow : public GoldRushAction {
    public:
        LoadMarshmellow(MARSHMALLOWS color);
        GoldRushAction* run(Robot* robot);
    private:
        MARSHMALLOWS color;

};

#endif