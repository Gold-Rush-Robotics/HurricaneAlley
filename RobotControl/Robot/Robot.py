from adafruit_pca9685 import PCA9685
from board import SCL, SDA
import board
import busio

from Robot.Drivebase import Drivetrain
from Robot.Duck import Duck
from Robot.Marshmallows import Marshmallows
from Robot.Intake import Intake
from Vision.mainVision import VisionBuilder


class Robot:
    drivetrain: Drivetrain
    duck: Duck
    marshmallow:  Marshmallows
    intake: Intake
    vision: VisionBuilder
    def __init__(self) -> None:
        i2c = busio.I2C(SCL, SDA)
        self.pca = PCA9685(i2c)
        self.pca.frequency = 300
        self.drivetrain = Drivetrain(self.pca)
        self.duck = Duck(self.pca)
        self.marshmallow = Marshmallows(self.pca, i2c)
        self.intake = Intake(self.pca)
        self.vision = VisionBuilder()
    
    def clear(self):
        for channel in self.pca.channels:
            channel.duty_cycle = 0
        self.pca.deinit()
        self.drivetrain.driveM(0, 0, 0, 0)
        self.intake.intake_motor.run(0)
        self.intake.cow_catcher_motor.run(0)
        
        
