from Hardware.actuators import GRRRoboClaw
from Hardware.encoders import Encoder
from adafruit_pca9685 import PCA9685
from utils import PID, clampRange, normalizeAngle
from constants import *
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
    position: np.array = np.array([START_X, START_Y, np.pi/2])
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
            [np.cos(self.position[2]), -np.sin(self.position[2]), 0],
            [np.sin(self.position[2]),  np.cos(self.position[2]), 0],
            [0, 0, 1]])

        sineTerm = 0
        cosTerm = 0

        if(np.isclose(fi, 0)):
            sineTerm = 1.0 - ( fi * fi ) / 6.0
            cosTerm = fi / 2.0
        else:
            sineTerm = np.sin(fi)/fi
            cosTerm = (1-np.cos(fi))/fi

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
    def driveToPoint(self, pose:np.array, distTol:float, angTol:float) -> bool:
        '''
        distTol in cm
        angTol in radians
        '''
        ax, ay, atheta = self.position
        bx, by, btheta = pose
        distDif = np.linalg.norm(np.array([bx, by])-np.array([ax, ay]))
        headingDif = normalizeAngle(btheta-atheta)

        xComp = 0
        yComp = 0
        tComp = 0

        inDist = distDif <= distTol
        inAng = headingDif <= angTol

        if(not inDist):
            correction = self.translatePid.calculate(0, distDif)
            unit_vector_1 = np.array([ax, ay]) / np.linalg.norm(np.array([ax, ay]))
            unit_vector_2 = np.array([bx, by]) / np.linalg.norm(np.array([bx, by]))
            angle = np.arccos(np.dot(unit_vector_1, unit_vector_2))
            xComp -= correction * np.cos(angle)
            yComp += correction * np.sin(angle)

        if(not inAng):
            correction = self.roatatePid.calculate(0, headingDif)
            tComp -= correction

        self.drivePow(xComp, yComp, tComp)
        return (inDist and inAng)


    