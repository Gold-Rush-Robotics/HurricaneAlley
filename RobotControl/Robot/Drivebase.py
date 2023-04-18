from Hardware.actuators import GRRRoboClaw
from Hardware.encoders import Encoder
from adafruit_pca9685 import PCA9685
from utils import PID, clampRange, normalizeAngle, unitVectorinator
from constants import *
import numpy as np
import math

ODO_R = 1.903190

ODO_N = 8192

CM_PER_TICK = 2.0 * math.pi * ODO_R / ODO_N


ODO_L = 23.6
ODO_B = 11.5

LEFT_ODO_PORT = 1
RIGHT_ODO_PORT = 2
MIDDLE_ODO_PORT = 0

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
    translatePid: PID = PID(1, 0, 0, 1.0, -1.0, 0.3)
    roatatePid: PID = PID(1, 0, 0, 1.0, -1.0, 0.3)
    position: list[float, float, float]
    encoderHandler: Encoder = Encoder()
    oldH = 0
    oldL = 0
    oldR = 0
    
    def __init__(self, pca: PCA9685) -> None:
        self.fl = GRRRoboClaw(pca, 0x81, False)
        self.fl.reverse(True)
        self.fr = GRRRoboClaw(pca, 0x82, True)
        self.bl = GRRRoboClaw(pca, 0x81, True)
        self.bl.reverse(True)
        self.br = GRRRoboClaw(pca, 0x82, False)
        self.position = [START_X, START_Y, 0]
    
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
        """Updates robot odometry position
           Using X as robot forward and Y as TODO
        Returns:
            np.array: returns robot global position
        """

        # Get updated odometer positions
        enc = self.encoderHandler.getCounts()
        posL = enc[LEFT_ODO_PORT]
        posR = -enc[RIGHT_ODO_PORT]
        posH = -enc[MIDDLE_ODO_PORT]

        # Get changes in odometer positions
        dR = posR - self.oldR
        dL = posL - self.oldL
        dH = posH - self.oldH

        # Calculate Pose Exponential odomerty variables
        phi = (dL - dR) / ODO_L # robot delta heading
        dPoseC = (dL + dR) / 2 # robot delta x
        dPoseP = (dH - (ODO_B * phi)) # robot delta y

        # Convert tick amounts into cm amounts
        phi = phi * CM_PER_TICK
        dPoseC = dPoseC * CM_PER_TICK
        dPoseP = dPoseP * CM_PER_TICK

        # Generate robot relative delta matrix
        dPoseRobot = np.array([dPoseC, dPoseP, phi])

        # Build robot to global rotation matrix
        robotToGlobalMatrix = np.matrix([
            [np.cos(self.position[2]), -np.sin(self.position[2]), 0],
            [np.sin(self.position[2]),  np.cos(self.position[2]), 0],
            [0, 0, 1]])

        sineTerm = 0
        cosTerm = 0

        # Check to see if theta is close to zero which would make the trig terms indeterminate
        if(np.isclose(phi, 0)):
            # Approximate terms with taylor series
            sineTerm = 1.0 - ( ( phi * phi ) / 6.0 )
            cosTerm = phi / 2.0
        else:
            # Calculate full terms
            sineTerm = np.sin(phi) / phi
            cosTerm = (np.cos(phi) - 1) / phi

        # Build arc following estimation matrix
        robotDeltaEstimationMatrix = np.matrix([
            [sineTerm, cosTerm, 0],
            [-cosTerm, sineTerm, 0],
            [0, 0, 1]
        ])

        # Dot product all the matricies together to convert encoder delta movement to global delta pose
        # dPoseGlobal = robotToGlobalMatrix.dot(robotDeltaEstimationMatrix).dot(dPoseRobot)
        dPoseGlobal = np.dot(robotToGlobalMatrix, np.dot(robotDeltaEstimationMatrix, dPoseRobot))

        # Update robot position with new changes
        self.position[0] += dPoseGlobal[0]
        self.position[1] += dPoseGlobal[1]
        self.position[2] += dPoseGlobal[2]

        # Save old encoder values for next iteration
        self.oldL = posL
        self.oldR = posR
        self.oldH = posH

        return self.position
    def driveToPoint(self, goalPose:np.array, distTol:float, angTol:float) -> bool:
        """Drives to point along line connecting current point to goal point

        Args:
            goalPose (np.array): vector for goal pose
            distTol (float): goal translational proximity exit condition tolerance in cm
            angTol (float): goal angle proximity exit condition tolerance in radians

        Returns:
            bool: True if exit condition proximities are met
        """

        # Get current position and goal position
        currGlobalX, currGlobalY, currGlobalH = self.position
        goalPoseX, goalPoseY, goalPoseH = goalPose

        # Calculate magnitude of distance from goal
        distDif = np.linalg.norm(np.array([goalPoseX, goalPoseY]) - np.array([currGlobalX, currGlobalY]))

        # Calculate angle error
        headingError = normalizeAngle(goalPoseH - currGlobalH)

        xComp = 0
        yComp = 0
        tComp = 0

        # Check to see if in exit condition proximity
        inDist = distDif <= distTol
        inAng = headingError <= angTol

        # Run translational corrections
        if(not inDist):
            correctionMagnitude = self.translatePid.calculate(0, distDif)

            translationalError = np.array([goalPoseX - currGlobalX, goalPoseY - currGlobalY])

            angleToDrive = np.arctan2(translationalError(1), translationalError(0))

            xComp = correctionMagnitude * np.cos(angleToDrive)
            yComp = correctionMagnitude * np.sin(angleToDrive) # TODO there may or may not need to be a negatie here depending upon y-axis definition
        
        # Run rotational corrections
        if(not inAng):
            correctionMagnitude = self.roatatePid.calculate(0, headingError)
            tComp = correctionMagnitude

        self.drivePow(xComp, yComp, tComp)
        return (inDist and inAng)


    