#include "Runner.h"

/* This function initializes all the needed values
Base Behavior: initialize robot and advance to the initLoop*/
int Runner::init(){
    //Initialize all the thingies
    Runner::robot.start();
    Runner::initLoop();
    return 0;
}

/* This function will run after init until told to go to loop (light turning on)
Base behavior: Imediatly advance*/
int Runner::initLoop(){
    return Runner::loop();
}
/* Loops Forever, and handles the processes
Base behavior: Imediately die*/
int Runner::loop(){
    return 0;
}
