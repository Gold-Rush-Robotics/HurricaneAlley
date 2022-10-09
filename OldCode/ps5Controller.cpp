#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "ps5Controller.h"

#define AXIS_MAX 32767.0    

ps5Controller::ps5Controller() {
    
}

void ps5Controller::printStates() {
    printf("lx: %.2f ly: %.2f rx: %.2f ry: %.2f dx: %.1f dy: %.1f lBump: %d rBump: %d lTrigBool: %d rTrigBool: %d " , 
        lx, ly, rx, ry, dx, dy, lBump ? 1 : 0, rBump ? 1 : 0, lTrigBool ? 1 : 0, rTrigBool ? 1 : 0);
    printf("lTrig: %.2f rTrig %.2f ", lTrig, rTrig);
    printf("x: %d c: %d t: %d s: %d\n", x ? 1 : 0, c ? 1 : 0, t ? 1 : 0, s ? 1 : 0);
}

void ps5Controller::eventHandler(struct js_event *event) {
    if(event->type == JS_EVENT_BUTTON) {
        //Button Handler
        switch (event->number) {
            case 0:
                //square
                s = event->value;
                break;
            case 1:
                //X
                x = event->value;
                break;
            case 2:
                //circle
                c = event->value;
                break;
            case 3:
                //triangle
                t = event->value;
                break;
            case 4: 
                //left bumper
                lBump = event->value;
                break;
            case 5:
                //right bumper
                rBump = event->value;
                break;
            case 6: 
                //left trigger
                lTrigBool = event->value;
                break;
            case 7:
                //right trigger
                rTrigBool = event->value;
                break;
        }

    } else if (event->type == JS_EVENT_AXIS) {
        //Joystick, Trigger, and D-Pad Handler
        switch (event->number) {
            case 0:
                //left x
                lx = event->value/AXIS_MAX;
                break;
            case 1:
                //left y
                ly = event->value/-AXIS_MAX;
                break;
            case 2:
                //right x
                rx = event->value/AXIS_MAX;
                break;
            case 5:
                //right y
                ry = event->value/-AXIS_MAX;
                break;
            case 3:
                //left trigger
                lTrig = event->value/AXIS_MAX;
                break;
            case 4:
                //right trigger
                rTrig = event->value/AXIS_MAX;
                break;
            case 6:
                //d-pad x
                dx = event->value/AXIS_MAX;
                break;
            case 7:
                //d-pad y
                dy = event->value/-AXIS_MAX;
                break;
        }
    }
};