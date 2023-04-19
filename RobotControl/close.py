from Robot.Robot import Robot
from Robot.Marshmallows import PringleStates
from constants import *

from adafruit_pca9685 import PCA9685
from board import SCL, SDA
from utils import reMap
import board
import busio
import time 


robot = Robot()

robot.intake.dm_separator_servo.run(DM_SEPARATOR_MARSHMALLOW)


input()

robot.intake.dm_separator_servo.run(DM_RELEASE)

