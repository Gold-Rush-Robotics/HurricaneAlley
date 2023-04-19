from adafruit_pca9685 import PCA9685
from board import SCL, SDA
import board
import busio
import adafruit_tcs34725 as ColorSensor


from Robot.Drivebase import Drivetrain
from Robot.Duck import Duck
from Robot.Marshmallows import Marshmallows
from Robot.Intake import Intake


class Robot:
    drivetrain: Drivetrain
    duck: Duck
    marshmallow:  Marshmallows
    intake: Intake
    start_color_sensor: ColorSensor.TCS34725
    def __init__(self) -> None:
        i2c = busio.I2C(SCL, SDA)
        self.pca = PCA9685(i2c)
        self.pca.frequency = 300
        self.drivetrain = Drivetrain(self.pca)
        self.duck = Duck(self.pca)
        self.marshmallow = Marshmallows(self.pca, i2c)
        self.intake = Intake(self.pca)
        self.start_color_sensor = ColorSensor.TCS34725(i2c, 0x29)
    
    def clear(self):
        for channel in self.pca.channels:
            channel.duty_cycle = 0
        self.pca.deinit()
        
        
