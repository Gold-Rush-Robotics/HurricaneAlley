from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors
from Robot.Marshmallows import PringleStates
from Actions.marshmallowActions import LoadStack
from Actions.foodActions import FoodAction 
from main import runEachLoop
import busio
from board import SCL, SDA
import board
import adafruit_tcs34725 as ColorSensor
import numpy as np


robot = Robot()

should_start : bool = False
check_for_light : bool = True

robot.marshmallow.revolver.run(0.0)

i2c = busio.I2C(SCL, SDA)
color_sensor = ColorSensor.TCS34725(i2c, 0x29)

START_LIGHT = [20, 0, 0, 0]
NUM_SAMPLES = 250

samples : list = [color_sensor.lux]

std : float = 0
mean : float = 0
value = 0

def collect_samples():
    for i in range(NUM_SAMPLES):
        samples.append(color_sensor.lux)
    data = np.array(samples)
    std = np.std(data)
    mean = np.average(data)

while check_for_light:
    print(color_sensor.lux)
    if robot.duck.home_sensor.value:
        should_start = True
        print("Should Start")
        for i in range(NUM_SAMPLES):
            samples.append(color_sensor.lux)
            data = np.array(samples)
            std = np.std(data)
            mean = np.average(data)
    if should_start:
        value = color_sensor.lux
        if not np.isclose(value, mean, atol=2*std):
            check_for_light = False

print("std: ", std)
print("Value: ", value)
print("mean: ", mean)