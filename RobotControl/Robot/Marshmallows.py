from __future__ import annotations
from enum import Enum
from Hardware.actuators import Servo, PWMMotor
from Hardware.encoders import Encoder
from adafruit_pca9685 import PCA9685
import adafruit_tcs34725 as ColorSensor
from utils import PID
from microcontroller import Pin
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

class PringleStates(Enum):
    OPEN = 90
    TIGHT = 30
    LOAD = 39
    

LOADER_UP = 162
LOADER_DOWN = 0

PLACER_UP = 149
PLACER_DOWN = 10

PRINGLE_OPEN = 90
PRINGLE_CLOSED = 39

AGITATOR_SPEED = 0.5

REVOLVER_ENCODER_PORT = 3

# Tick Positions of Marshmallow Chambers in Relation to Pringle
ENCODER_POS = [1792, 2534, 3254, 320, 1024]

# Mod to add to ENCODER_POS to go to Agitator
AGITATOR_MOD = -1792

class Marshmallows:
    
    loader : Servo
    placer : Servo
    pringle_can : Servo
    
    revolver_enc : Encoder
    
    revolver : PWMMotor
    agitator : PWMMotor
    
    revolver_PID : PID
    
    color_sensor : ColorSensor.TCS34725
    
    stored_in_revolver : list[MarshmallowColors, MarshmallowColors, MarshmallowColors, MarshmallowColors, MarshmallowColors]
    state : int 
    curr_chamber : int
    
    def __init__(self, pca: PCA9685, i2c: type[board.I2C]) -> None:
        self.loader = Servo(pca, 0, 0, 180)
        self.placer = Servo(pca, 5, 0, 180)
        self.pringle_can = Servo(pca, 6, 0, 180)
        self.revolver_enc = Encoder()
        self.revolver = PWMMotor(14, Pin(0), pca)
        self.agitator = PWMMotor(13, Pin(0), pca)
        
        self.revolver_PID = PID(0.01, 0, 0, 1.0, -1.0)
        
        self.color_sensor = ColorSensor.TCS34725(i2c, 0x29)
        self.stored_in_revolver = [ MarshmallowColors.GREENFOOD, 
                                    MarshmallowColors.REDFOOD, 
                                    MarshmallowColors.EMPTY,
                                    MarshmallowColors.EMPTY,
                                    MarshmallowColors.EMPTY,]
        self.state = 0

    def agitate(self, marsh_color : MarshmallowColors) -> bool:
        """Agitates the marshmallows until they settle

        Args:
            MarshmallowColors: color read from storeMarsh
        Returns:
            bool: returns true if the marshmallow has settled
        """
        if marsh_color is MarshmallowColors.EMPTY:
            self.agitator.run(AGITATOR_SPEED)
            return False
        else:
            self.agitator.run(0)
            return True

    def storeMarsh(self) -> bool:
        """Prep to recieve and agitate
        
        Returns:
            bool: true when the marshmallow has settled
        """
        color = self.color_sensor.color_rgb_bytes
        marsh_color : MarshmallowColors = MarshmallowColors.getMarshFromRGB(color)
        match(self.state):
            case 0:
                # Rotate to Empty Chamber
                if self.rotate_revolver(MarshmallowColors.EMPTY, True):
                    self.state = 1
            case 1:
                # Dump Marshmallow in the Chamber
                if self.agitate(marsh_color):
                    self.stored_in_revolver[self.curr_chamber] = marsh_color
                    self.state = 0
                    return True
        return False
        
    
    def rotate_revolver(self, color: MarshmallowColors, to_agitator: bool) -> bool:
        """Rotates revolver

        Args:
            color (MarshmallowColors): Which color it should rotate to
            to_agitator (bool): if it should rotate to the agitator

        Returns:
            bool: true when it is finished
        """
        index = self.stored_in_revolver.index(color)
        curr_count = self.revolver_enc.getCounts()[REVOLVER_ENCODER_PORT]
        self.revolver.run( self.revolver_PID.calculate(ENCODER_POS[index] + AGITATOR_MOD, curr_count) )
        if np.isclose(ENCODER_POS[index] + AGITATOR_MOD if to_agitator else 0, curr_count):
            self.revolver.run(0)
            self.curr_chamber = index
            return True
        return False
    
    def set_loader(self, load: bool) -> None:
        """Toggles the Loader Servo

        Args:
            load (bool): when true moves servo up
        """
        if load:
            self.loader.run(LOADER_UP)
            self.stored_in_revolver[self.curr_chamber] = MarshmallowColors.EMPTY
        else:
            self.loader.run(LOADER_DOWN)
            
    def set_pringle(self, degree: PringleStates) -> None:
        """Controls pringle servo

        Args:
            degree (PringleStates): which state the pringle servo should be in
        """
        self.pringle_can.run(degree.value)
                
    
    def place_pringle(self, lower:bool) -> None:
        if lower:
            self.placer.run(PLACER_DOWN)
        else:
            self.placer.run(PLACER_UP)

#Three tall Statue: base level – white, second level – green, third level – red
#Two Tall Statue: base level – white, second level – green
#with a yellow duck on top on the outside pond locations
