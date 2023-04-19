from Actions.GoldRushAction import GoldRushAction
from Actions.marshmallowActions import LoadStack
from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors, PringleStates
from Actions.placementActions import PlaceStack
import numpy as np
from constants import *

import time

robot = Robot()
check : bool = True
place_stack : PlaceStack = PlaceStack("Place food Chip")
state = 0
samples = []
NUM_SAMPLES = 150

robot.drivetrain.drivePow(0,0,0)
robot.clear()

input()

while check:
    if robot.drivetrain.driveToPoint((122, 80, 0), 2, 0.1):
        break
    robot.drivetrain.updatePosition()

    # if robot.drivetrain.drivePow(-0.2, 0, 0):
    #     break
