from Robot.Robot import Robot
from Actions.runActions import runEachLoop
import numpy as np
import sys



robot = Robot()

robot.drivetrain.driveM(0, 0, 0, 0)

robot.drivetrain.position = [0.0, 0.0, 0.0]
robot.drivetrain.encoderHandler.reset()
try:
    while True:
        runEachLoop(robot)
        if robot.drivetrain.driveToPoint(np.array([0, 0, np.pi/2]), 1, 0.08):
            break
        print(robot.drivetrain.position)
except KeyboardInterrupt:
    robot.clear()
    sys.exit(0)
    