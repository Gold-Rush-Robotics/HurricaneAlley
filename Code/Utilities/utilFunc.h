#ifndef UtilityH
#define UtilityH

double constrain(double minN, double maxN, double val);
bool essentiallyEqual(float a, float b, float epsilon);

enum MARSHMALLOWS { 
    EMPTY,
    RED, 
    GREEN, 
    WHITE,
    RED_CHIPS,
    GREEN_CHIPS
};

#endif