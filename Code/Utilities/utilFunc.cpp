#include "utilFunc.h"
#include <algorithm> 

double constrain(double minN, double maxN, double val){
    return std::min(maxN, std::max(minN, val));
}