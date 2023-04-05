from adafruit_pca9685 import PCA9685
from adafruit_motor import servo
from microcontroller import Pin
import digitalio
import numpy as np

from utils import clampRange

PWM_MAX = 0xFFFF

class Actuator:
    pca: PCA9685 = None
    def __init__(self, pca: PCA9685) -> None:
        self.pca = pca
    def run(self, power:int) -> None:
        '''run the actuator
        power is speed for motors
              is location for servos
        '''
        pass
    def coast() -> None:
        pass
    

class PWMMotor(Actuator):
    pwmPIN = 0
    dirPIN: digitalio.DigitalInOut = 0
    reversed: bool = False
    def __init__(self, pwm: int, dir: Pin, pca:PCA9685) -> None:
        super.__init__(pca)
        self.pwmPIN = pwm
        self.dirPIN = digitalio.DigitalInOut(dir)
        self.dirPIN.switch_to_output(True)
    
    def reverse(self, reverse:bool) -> None:
        self.reversed = reverse

    def run(self, power: int) -> None:
        power = clampRange(-1.0, 1.0, power)
        self.pca.channels[self.pwmPIN] = abs(power) * PWM_MAX
        self.dirPIN.value = (np.sign(power) < 0) and not self.reversed
        
        