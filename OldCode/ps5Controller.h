#ifndef ps5Controller_H
#define ps5Controller_H

class ps5Controller {
    public: 
            bool x = false, c = false, t = false, s = false;
            bool lBump = false, rBump = false;
            bool lTrigBool = false, rTrigBool = false;
            double lx = 0.0, ly = 0.0, rx = 0.0, ry = 0.0;
            double dx = 0.0, dy = 0.0;
            double lTrig = -1.0, rTrig = -1.0;

            ps5Controller();
            void printStates();
            void eventHandler(struct js_event *event);
};

#endif