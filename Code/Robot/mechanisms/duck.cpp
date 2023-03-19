#include "duck.h"
#include <memory>
#include <iostream>

Duck::Duck(std::shared_ptr<PCA9685> pca){

    // Need to update pins for servos
    gripper = std::make_shared<Servo>(-1, pca);
    elbow = std::make_shared<Servo>(-1, pca);
    shoulder = std::make_shared<Servo>(-1, pca);
    waist = std::make_shared<Servo>(-1, pca);

    agitator = std::make_shared<Motor>(17, 6, pca); // Need to update these pins (same as duck now)

    hall_effect = new DigitalSensor(-1, true);

    stored_ducks = 0;
}

void Duck::grip_duck()
{
    // Need to update gripper positions
    gripper->setPosition(1800, 300);
}

void Duck::release_duck()
{
    // Need to update gripper positions
    gripper->setPosition(1800, 300);
}

// Look at the name
void Duck::turn_on_agitator()
{
    Duck::agitator->setPower(1);
}

// Look at the name
void Duck::turn_off_agitator()
{
    Duck::agitator->setPower(0.0);
}

bool Duck::home_agitator()
{
    if (hall_effect->read_sensor())
    {
        Duck::agitator->setPower(0);
        return true;
    }
    else
    {
        Duck::agitator->setPower(0.1);
    }
    return false;
}

void Duck::elbow_servo(ELBOW_POSITIONS state)
{
    switch(state)
    {
        case ELBOW_POSITIONS::AGITATOR:
            elbow->setPosition(-1, 300);    // Need to tune values
            break;
        case ELBOW_POSITIONS::STORE_1:
            elbow->setPosition(-1, 300);
            break;
        case ELBOW_POSITIONS::STORE_2:
            elbow->setPosition(-1, 300);
            break;
        case ELBOW_POSITIONS::STACK:
            elbow->setPosition(-1, 300);
            break;
        case ELBOW_POSITIONS::RAISED:
            elbow->setPosition(-1, 300);
            break;
    }
}

void Duck::shoulder_servo(SHOULDER_POSITIONS state)
{
    switch(state)
    {
        case SHOULDER_POSITIONS::AGITATOR:
            shoulder->setPosition(-1, 300);    // Need to tune values
            break;
        case SHOULDER_POSITIONS::STORE_1:
            shoulder->setPosition(-1, 300);
            break;
        case SHOULDER_POSITIONS::STORE_2:
            shoulder->setPosition(-1, 300);
            break;
        case SHOULDER_POSITIONS::STACK:
            shoulder->setPosition(-1, 300);
            break;
    }
}

void Duck::waist_servo(WAIST_POSITIONS state)
{
    switch(state)
    {
        case WAIST_POSITIONS::AGITATOR:
            waist->setPosition(-1, 300);    // Need to tune values
            break;
        case WAIST_POSITIONS::STORE_1:
            waist->setPosition(-1, 300);
            break;
        case WAIST_POSITIONS::STORE_2:
            waist->setPosition(-1, 300);
            break;
        case WAIST_POSITIONS::STACK:
            waist->setPosition(-1, 300);
            break;
    }
}