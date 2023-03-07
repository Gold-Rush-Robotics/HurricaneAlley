#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"


#ifndef STOREMARSHMALLOWH
#define STOREMARSHMALLOWH

class StoreMarshmellow : public GoldRushAction {
    public:
        StoreMarshmellow(Revolver::Marshmallow color);
        GoldRushAction* run(Robot* robot);
    private:
        Revolver::Marshmallow color;

}


#endif