#include "pose.h"
#include <algorithm> 
#include <math.h>
#include <iostream>


Pose::Pose(double x, double y, double theta){
    Pose::x = x;
    Pose::y = y;
    Pose::theta = theta;
}

Pose::Pose(){
    x = 0;
    y = 0;
    theta = 0;
}

void Pose::update(double x, double y, double theta){
    Pose::x = x;
    Pose::y = y;
    Pose::theta = theta;
}

int radtoDeg(double rad){
    return rad * (180.0/M_PI);
}

double degToRad(int deg){
    return deg * (M_PI/180.0);
}

void Pose::printPosition(){
    std::cout << "X:" << x << " - Y:" << y << " - Theta:" << theta << std::endl;
}

void Pose::normalizeHeading(){
    theta = degToRad(radtoDeg(theta) % 360);
}


double Pose::getDirection(Pose* p){
    double xLength = p->x - x;
    double yLength = p->y - y;
    return std::atan2(yLength, xLength);
}

double Pose::getDistance(Pose* p){
    return std::sqrt(std::pow(x-p->x, 2) + std::pow(y-p->y, 2));
}

double Pose::getHeadingOffset(Pose* p){
    return p->theta - theta;
}