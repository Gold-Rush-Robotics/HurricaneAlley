#include "Runner.h"
#include "../Utilities/pose.h"
#include "../Actions/DriveToPointAction.h"

class Auto : Runner
{
    
    public:
    int init(int argc, char *argv[])
    {
        Runner::init(argc, argv);
        spawnThreads();
        currentAction = new DriveToPointAction(new Pose(20,0,0),1,1);
        return initLoop(argc, argv);;
    }
    int loop(int argc, char *argv[])
    {
        Robot* robotPtr = &robot;
        while(currentAction != nullptr){
            currentAction->printName();
            robot.drivetrain->encoderLogic();
            currentAction = currentAction->run(robotPtr);
        }
        return 0;
    }
    int initLoop(int argc, char *argv[]) {
        bool light = false;
        while(!light){
            break;
        }
        
        return loop(argc, argv);
    }
};

int main(int argc, char *argv[])
{
    Auto a;
    a.init(argc, argv);
    return 0;
}
