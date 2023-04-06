from adafruit_pca9685 import PCA9685
from board import SCL, SDA
import board
import busio

from Drivebase import Drivetrain


class Robot:
    drivetrain: Drivetrain
    def __init__(self) -> None:
        i2c = busio.I2C(SCL, SDA)
        pca = PCA9685(i2c)
        self.drivetrain = Drivetrain(pca)
        
