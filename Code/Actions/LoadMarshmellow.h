#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef LOADMARSHMELLOWH
#define LOADMARSHMELLOWH

class LoadMarshmellow : public GoldRushAction {
    public:
        LoadMarshmellow(Revolver::MARSHMALLOWS color);
        GoldRushAction* run(Robot* robot);
    private:
        Revolver::MARSHMALLOWS color;

};

#endif