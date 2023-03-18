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

void Duck::move_elbow(int pos)
{
    elbow->setPosition(pos, 300);
}

void Duck::move_shoulder(int pos)
{
    shoulder->setPosition(pos, 300);
}

void Duck::turn_to_agitator()
{
    // Need to update shoulder positions
    waist->setPosition(1800, 300);
}

void Duck::turn_to_space_1()
{
    // Need to update shoulder positions
    waist->setPosition(1800, 300);
}

void Duck::turn_to_space_2()
{
    // Need to update shoulder positions
    waist->setPosition(1800, 300);
}

void Duck::turn_to_stack()
{
    // Need to update shoulder positions
    waist->setPosition(1800, 300);
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