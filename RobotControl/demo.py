from Robot.Robot import Robot
import sys

robot = Robot()


try:
    while True:
        robot.intake.intake_marshmallow()
        input()

except KeyboardInterrupt:
    robot.clear()
    sys.exit(0)
    