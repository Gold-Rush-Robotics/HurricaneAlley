from adafruit_pca9685 import PCA9685
from adafruit_motor import servo
from microcontroller import Pin
import digitalio
from board import SCL, SDA
import board
import busio

from Vendors.roboclaw_python.roboclaw_3 import Roboclaw



from utils import clampRange, reMap

PWM_MAX = 0xFFFF - 1

class Actuator:
    pca: PCA9685 = None

    def __init__(self, pca: PCA9685) -> None:
        self.pca = pca
        self.roboclaw = Roboclaw("/dev/ttyS0", 38400)
        self.roboclaw.Open()
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
        self.dirPIN.value = (power < 0) and not self.reversed

class Servo(Actuator):
    pwmPin: int = 0
    rangeMax: int = 0
    rangeMin: int = 360
    def __init__(self, pca: PCA9685, pwm:int, rangeMin: int, rangeMax: int) -> None:
        super().__init__(pca)
        self.pwmPin = pwm
        self.rangeMax = rangeMax
        self.rangeMin = rangeMin
    def run(self, position: int) -> None:
        self.pca.channels[self.pwmPin] = reMap(position, self.rangeMin, self.rangeMax, PWM_MAX, 0)

class GRRRoboClaw(Actuator):
    rcAddress: int = 0x00
    m1: bool = True
    reversed:bool = False
    def __init__(self, pca: PCA9685, rcAddress:int, m1:bool) -> None:
        super().__init__(pca)
        self.rcAddress = rcAddress
        self.m1 = m1

    def reverse(self, reverse:bool) -> None:
        self.reversed = reverse

    def run(self, power: int) -> None:
        power = clampRange(-1, 1, power)
        power = reMap(power, -1, 1, 127, -127)
        if(self.m1):
            if((power < 0) and not reversed):
                self.roboclaw.BackwardM1(self.rcAddress, power)
            else:
                self.roboclaw.ForwardM1(self.rcAddress, power)
        else:
            if((power < 0) and not reversed):
                self.roboclaw.BackwardM2(self.rcAddress, power)
            else:
                self.roboclaw.ForwardM2(self.rcAddress, power)
        

if __name__ == "__main__":
    i2c = busio.I2C(SCL, SDA)
    pca = PCA9685(i2c)
    motor = PWMMotor(0, board.GPIO0, pca)
    input("speed")
    motor.run(1)
    input("input to stop")
    motor.run(0)
    print("stopped")

