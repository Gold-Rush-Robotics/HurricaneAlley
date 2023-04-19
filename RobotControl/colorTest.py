from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors
from main import runEachLoop

robot = Robot()

robot.marshmallow.revolver.run(0)

while True:
    color = robot.marshmallow.color_sensor.color_raw
    print(color)
    print(MarshmallowColors.getMarshFromRGB(color))
    