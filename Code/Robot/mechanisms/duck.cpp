#include "duck.h"
#include <memory>
#include <iostream>

Duck::Duck(std::shared_ptr<PCA9685> pca){

    // Need to update pins for servos
    gripper = std::make_shared<Servo>(-1, pca);
    j3 = std::make_shared<Servo>(-1, pca);
    j2 = std::make_shared<Servo>(-1, pca);
    j1 = std::make_shared<Servo>(-1, pca);

    agitator = std::make_shared<Motor>(17, 6, pca); // Need to update these pins (same as duck now)

    hall_effect = new DigitalSensor(-1, true);

    stored_ducks = 3;   // For Testing remove later
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

void Duck::j3_servo(JOINT_POSITIONS state)
{
    switch(state)
    {
        case JOINT_POSITIONS::AGITATOR:
            j3->setPosition(-1, 300);    // Need to tune values
            break;
        case JOINT_POSITIONS::STORE_1:
            j3->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::STORE_2:
            j3->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::STACK:
            j3->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::RAISED:
            j3->setPosition(-1, 300);
            break;
    }
}

void Duck::j2_servo(JOINT_POSITIONS state)
{
    switch(state)
    {
        case JOINT_POSITIONS::AGITATOR:
            j2->setPosition(-1, 300);    // Need to tune values
            break;
        case JOINT_POSITIONS::STORE_1:
            j2->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::STORE_2:
            j2->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::STACK:
            j2->setPosition(-1, 300);
            break;
    }
}

void Duck::j1_servo(JOINT_POSITIONS state)
{
    switch(state)
    {
        case JOINT_POSITIONS::AGITATOR:
            j1->setPosition(-1, 300);    // Need to tune values
            break;
        case JOINT_POSITIONS::STORE_1:
            j1->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::STORE_2:
            j1->setPosition(-1, 300);
            break;
        case JOINT_POSITIONS::STACK:
            j1->setPosition(-1, 300);
            break;
    }
}