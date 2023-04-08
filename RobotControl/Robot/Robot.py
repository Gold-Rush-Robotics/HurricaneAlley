from adafruit_pca9685 import PCA9685
from board import SCL, SDA
import board
import busio

from Drivebase import Drivetrain
from Duck import Duck
from Marshmallows import Marshmallows
from Intake import Intake
from Vision.mainVision import VisionBuilder


class Robot:
    drivetrain: Drivetrain
    duck: Duck
    marhmallow:  Marshmallows
    intake: Intake
    vision: VisionBuilder
    def __init__(self) -> None:
        i2c = busio.I2C(SCL, SDA)
        pca = PCA9685(i2c)
        self.drivetrain = Drivetrain(pca)
        self.duck = Duck(pca)
        self.marhmallow = Marshmallows(pca, i2c)
        self.intake = Intake(pca)
        self.vision = VisionBuilder()
        
        
