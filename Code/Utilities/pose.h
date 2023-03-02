#ifndef PoseH
#define PoseH

class Pose{
    public:
        Pose(double x, double y, double theta);
        Pose();
        double getDistance(Pose p);
        double getDirection(Pose p);
        double getHeadingOffset(Pose p);
        void normalizeHeading();
        void update(double x, double y, double theta);
        double x, y, theta;   
};

#endif