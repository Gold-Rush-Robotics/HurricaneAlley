from __future__ import annotations
from enum import Enum
from Hardware.actuators import Servo, PWMMotor
from adafruit_pca9685 import PCA9685
import adafruit_tcs34725 as ColorSensor
import numpy as np
import board

class MarshmallowColors(Enum):
    EMPTY = (0, 0, 0)
    WHITE = (0, 0, 0)
    RED =   (0, 0, 0)
    GREEN = (0, 0, 0)
    REDFOOD =   (0, 0, 0)
    GREENFOOD = (0, 0, 0)
    
    @staticmethod   
    def getMarshFromRGB(color : list[int, int, int]) -> MarshmallowColors:
        if color[0] > 200 and color[1] > 200 and color[2] > 200 :
            return MarshmallowColors.WHITE
        elif color[0] > 200:
            return MarshmallowColors.RED
        elif color[1] > 200:
            return MarshmallowColors.GREEN
        else:
            return MarshmallowColors.EMPTY

LOADER_UP = 0
LOADER_DOWN = 0

PLACER_UP = 0
PLACER_DOWN = 0

PRINGLE_OPEN = 0
PRINGLE_CLOSED = 0

class Marshmallows:
    
    loader : Servo
    placer : Servo
    pringle_can : Servo
    
    revolver : PWMMotor
    
    color_sensor : ColorSensor
    
    def __init__(self, pca: PCA9685, i2c: type[board.I2C]) -> None:
        self.loader = Servo(pca, 0, 0, 180)
        self.placer = Servo(pca, 0, 0, 180)
        self.pringle_can  = Servo(pca, 0, 0, 180)
        self.color_sensor = ColorSensor(i2c, 0x29)
        

    def agitate(self) -> bool:
        """Agitates the marshmallows until they settle

        Returns:
            bool: returns true if the marshmallow has settled
        """
        pass

    def storeMarsh(self) -> bool:
        """Prep to recieve and agitate
        
        Returns:
            bool: true when the marshmallow has settled
        """
    
    def insertToPringle(self, color: MarshmallowColors) -> None:
        """puts the selected color into the pringle can

        Args:
            color (MarshmallowColors): which color to load
        """
        pass

    def pringleOpen(self, open:bool) -> None:
        """open the pringle can

        Args:
            open (bool): open or close
        """
        pass
    
    def lowerPringle(self, lower:bool) -> None:
        pass


#Three tall Statue: base level – white, second level – green, third level – red
#Two Tall Statue: base level – white, second level – green
#with a yellow duck on top on the outside pond locations
