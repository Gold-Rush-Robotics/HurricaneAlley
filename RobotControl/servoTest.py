from Robot.Robot import Robot
from Robot.Marshmallows import PringleStates

from adafruit_pca9685 import PCA9685
from board import SCL, SDA
from utils import reMap
import board
import busio


robot = Robot()


# robot.marshmallow.set_loader(True)
# robot.marshmallow.set_pringle(PringleStates.LOAD)

# input()

# robot.marshmallow.set_pringle(PringleStates.TIGHT)

# input()

# robot.marshmallow.set_loader(False)

# input()

# robot.marshmallow.place_pringle(True)


# input()

# robot.marshmallow.set_pringle(PringleStates.OPEN)

# input()

# robot.marshmallow.set_pringle(PringleStates.TIGHT)

# input()

robot.marshmallow.place_pringle(False)