#include "Runner.h"

class Auto : Runner
{
    int init() override
    {
        Runner::init();
    }
    int loop() override
    {
        if(currentAction != nullptr){
            currentAction = currentAction.run();
        }
    }
    int initLoop() override {
        bool light = true;
        while(!light){
            
        }
        loop();
    }
};

int main(int argc, char const *argv[])
{
    Auto::init();
    return 0;
}
