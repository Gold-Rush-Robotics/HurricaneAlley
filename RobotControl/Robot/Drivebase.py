from Hardware.actuators import GRRRoboClaw
from Hardware.encoders import Encoder
from adafruit_pca9685 import PCA9685
from utils import PID, clampRange
import numpy as np
import math

ODO_R = 1.903190
ODO_N = 8192
CM_PER_TICK = 2.0 * math.pi * ODO_R / ODO_N
ODO_L = 23.6
ODO_B = 11.5

LEFT_ODO_PORT = 0
RIGHT_ODO_PORT = 1
MIDDLE_ODO_PORT = 2

posL = 0
posR = 0
posH = 0
oldL = 0
oldR = 0
oldH = 0

class Drivetrain:
    fl : GRRRoboClaw
    fr : GRRRoboClaw
    bl : GRRRoboClaw
    br : GRRRoboClaw
    pca: PCA9685
    translatePid: PID = PID(1, 0, 0, 1.0, -1.0)
    roatatePid: PID = PID(1, 0, 0, 1.0, -1.0)
    position: np.array = np.array([0, 0, 0])
    encoderHandler: Encoder = Encoder()
    
    def __init__(self, pca: PCA9685) -> None:
        fl = GRRRoboClaw(pca, 0x00, True)
        fr = GRRRoboClaw(pca, 0x00, True)
        bl = GRRRoboClaw(pca, 0x00, True)
        br = GRRRoboClaw(pca, 0x00, True)
    
    def drivePow(self, forward: float, strafe: float, rotate:float) -> None:
        powerFL = forward + strafe - rotate
        powerFR = forward - strafe + rotate
        powerBL = forward - strafe - rotate
        powerBR = forward + strafe + rotate
        powerMax = max(abs(powerBL), abs(powerFR), abs(powerFL), abs(powerBR))
        if powerMax >= 1.0:
            powerFL /= powerMax
            powerFR /= powerMax
            powerBL /= powerMax
            powerBR /= powerMax

        self.driveM(powerFL, powerFR, powerBL, powerBR)
        
    def driveM(self, fl: float, fr: float, bl: float, br: float) -> None:
        self.fl.run(fl)
        self.fr.run(fr)
        self.bl.run(bl)
        self.br.run(br)
    
    def updatePosition(self) -> np.array:
        oldL = posL
        oldR = posR
        oldH = posH
        enc = self.encoderHandler.getCounts()
        posL = enc[LEFT_ODO_PORT]
        posR = enc[RIGHT_ODO_PORT]
        posH = enc[MIDDLE_ODO_PORT]

        dR = posR - oldR
        dL = posL - oldL
        dH = posH - oldH


        fi = CM_PER_TICK * (dR - dL) / ODO_L
        dXC = CM_PER_TICK * (dR + dL) / 2
        dXh = CM_PER_TICK * (dH + (fi/CM_PER_TICK * ODO_B))

        v = np.array([dXC, dXh, fi])

        m1 = np.matrix([
            [math.cos(self.position[2]), -math.sin(self.position[2]), 0],
            [math.sin(self.position[2]),  math.cos(self.position[2]), 0],
            [0, 0, 1]])

        sineTerm = 0
        cosTerm = 0

        if(np.isclose(fi, 0)):
            sineTerm = 1.0 - ( fi * fi ) / 6.0
            cosTerm = fi / 2.0
        else:
            sineTerm = math.sin(fi)/fi
            cosTerm = (1-math.cos(fi))/fi

        m2 = np.matrix([
            [sineTerm, -cosTerm, 0],
            [cosTerm, sineTerm, 0],
            [0, 0, 1]
        ])

        v2 = m1 @ m2 @ v

        self.position[0] += v2[0]
        self.position[1] += v2[1]
        self.position[2] += v2[2]

        return self.position
    