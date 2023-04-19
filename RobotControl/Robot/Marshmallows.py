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
import smbus

import busio


class MarshmallowColors(Enum):
    EMPTY     = 0
    WHITE     = 1
    RED       = 2
    GREEN     = 3
    CLOSE   = 4
    FAR = 5
    
    @staticmethod   
    def getMarshFromRGB(color : tuple[int, int, int, int]) -> MarshmallowColors:
        if color[3] < 50:
            return MarshmallowColors.EMPTY
        if color[3] > 300 :
            return MarshmallowColors.WHITE
        elif color[0] > 200:
            return MarshmallowColors.RED
        elif color[1] > 200:
            return MarshmallowColors.GREEN


class PringleStates(Enum):
    OPEN = 90
    TIGHT = 35
    LOAD = 65
    

LOADER_UP = 130
LOADER_DOWN = 55

PLACER_UP = 40_600
PLACER_DOWN = 18_000

PRINGLE_OPEN = 120
PRINGLE_CLOSED = 42

AGITATOR_SPEED = 0.5

REVOLVER_ENCODER_PORT = 7

COUNTS_IN_RANGE = 5

# Tick Positions of Marshmallow Chambers in Relation to Pringle
ENCODER_POS = [1140, 1569, 2015, 2470, 2918]

# Mod to add to ENCODER_POS to go to Agitator
AGITATOR_MOD = -1125

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
    
    counter: int
    
    def __init__(self, pca: PCA9685, i2c: type[board.I2C]) -> None:
        self.loader = Servo(pca, 0, 0, 180)
        self.placer = Servo(pca, 5, 0, 0xFFFF)
        self.pringle_can = Servo(pca, 6, 0, 180)
        self.revolver_enc = Encoder()
        self.revolver = PWMMotor(13, Pin(16), pca)
        self.agitator = PWMMotor(12, Pin(17), pca)
        self.agitator.reverse(True)
        
        self.revolver_PID = PID(0.004, .01, .00008, .6, -.6)
        self.counter = 0
        
        i2c4 = busio.I2C(Pin(1), Pin(0))

        
        self.color_sensor = ColorSensor.TCS34725(i2c4, 0x29)
        self.color_sensor.integration_time = 5
        self.stored_in_revolver = [MarshmallowColors.CLOSE, MarshmallowColors.FAR, MarshmallowColors.EMPTY, MarshmallowColors.EMPTY, MarshmallowColors.EMPTY]
        print(self.stored_in_revolver)
        self.curr_chamber = 0
        
        """[ MarshmallowColors.GREENFOOD, 
                                    MarshmallowColors.REDFOOD, 
                                    MarshmallowColors.EMPTY,
                                    MarshmallowColors.EMPTY,
                                    MarshmallowColors.EMPTY,]"""
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
        goal = ENCODER_POS[index] + (AGITATOR_MOD if to_agitator else 0)

        print(goal, curr_count)

        correction =  self.revolver_PID.calculate(goal, curr_count)
        print(correction)

        self.revolver.run(np.sign(correction)*  max(abs(correction), .08))
        if np.isclose(goal, curr_count, atol=2, rtol=0.01):
            self.counter += 1
            if(self.counter >= COUNTS_IN_RANGE):
                self.revolver.run(0.0)
                self.curr_chamber = index
                return True
        else:
            self.counter = 0
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
            self.placer.runPWM(PLACER_DOWN)
        else:
            self.placer.runPWM(PLACER_UP)

#Three tall Statue: base level – white, second level – green, third level – red
#Two Tall Statue: base level – white, second level – green
#with a yellow duck on top on the outside pond locations
