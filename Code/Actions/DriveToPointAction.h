#include "GoldRushAction.h"
#include "../Utilities/pose.h"

#ifndef DTPAH
#define DTPAH

class DriveToPointAction : public GoldRushAction {
    public:
        DriveToPointAction(Pose* point, double distanceTolerance, double angleTolerance);
        GoldRushAction* run(Robot* robot);
    private:
        Pose* point;
        double disT;
        double angT;
};

#endif