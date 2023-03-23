#include "digitalsensor.h"
#include <iostream>
#include "bcm2835.h"

DigitalSensor::DigitalSensor(int pin, bool pull_up)
{
    DigitalSensor::pin = pin;
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    if (pull_up)
        bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_UP);
    else
        bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_DOWN);
}

bool DigitalSensor::read_sensor()
{
    return bcm2835_gpio_lev(pin);
}