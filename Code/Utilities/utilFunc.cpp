#include "utilFunc.h"
#include <algorithm> 

double constrain(double minN, double maxN, double val){
    return std::min(maxN, std::max(minN, val));
}

bool essentiallyEqual(float a, float b, float epsilon)
{
    return std::abs(a - b) <= ( (std::abs(a) > std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
}
