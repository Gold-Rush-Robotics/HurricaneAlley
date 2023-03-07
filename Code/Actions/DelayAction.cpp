#include "DelayAction.h"

DelayAction::DelayAction(double seconds){
    DelayAction::seconds = seconds;
    flag = true;
    name = "Delay Action (" + std::to_string(seconds) + " seconds)";
}
GoldRushAction* DelayAction::run(Robot* robot){
    if(flag){
        begin = std::chrono::steady_clock::now();
        flag = false;
    }

    double diff = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count())*0.000001;

    return diff > seconds ? nextAction : this;

}




