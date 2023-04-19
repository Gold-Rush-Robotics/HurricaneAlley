from Robot.Robot import Robot
from Actions.runActions import runEachLoop
from Actions.DelayAction import DelayAction
from constants import *
robot = Robot()

robot.drivetrain.encoderHandler.reset()

robot.drivetrain.driveM(0, 0, 0, 0)

input()

delay = DelayAction(3.0)

for pose in SWEEPS:
    if robot.drivetrain.driveToPoint(pose, 2, .1):
        # if not delay.run(robot):
        #     delay.endTime = -1
        #     continue
        input()
    