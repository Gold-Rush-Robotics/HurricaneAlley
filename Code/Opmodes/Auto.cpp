#include "Runner.h"

class Auto : Runner
{
    public:
    int init(int argc, char *argv[])
    {
        Runner::init(argc, argv);
        spawnThreads();
        return initLoop(argc, argv);;
    }
    int loop(int argc, char *argv[])
    {

        while(currentAction != nullptr){
            robot.drivetrain->encoderLogic();
            currentAction = currentAction->run(robot);
        }
        return 0;
    }
    int initLoop(int argc, char *argv[]) {
        bool light = true;
        while(!light){
            
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
