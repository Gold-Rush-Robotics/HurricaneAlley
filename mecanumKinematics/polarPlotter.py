import matplotlib.pyplot as plt
import numpy as np

# Drivebase Constants

wheel_r = 0.0375 # wheel radius in meters

mecanum_offset_x = 0.104 # offset of mecanum center in meters
mecanum_offset_y = 0.104725 # offset of mecanum center in meters

K = abs(mecanum_offset_x) + abs(mecanum_offset_y)

# Mix robot relative to mecanum powers
def drivePow(forward: float, strafe: float, rotate:float) -> None:
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

    return powerFL, powerBL, powerBR, powerFR

# Calculate the output of the mecanum drivebase given inputs
def calcMecanumOutput(magnitude:float, angle:float) -> np.matrix:

    robot_x = magnitude * np.cos(angle)
    robot_y = magnitude * np.sin(angle)

    powers = drivePow(robot_x, robot_y, 0.0)

    print(f'{angle:.{2}f}: {powers[0]:.{2}f}, {powers[1]:.{2}f}, {powers[2]:.{2}f}, {powers[3]:.{2}f}')

    omega = np.array(powers)

    # des_velo = magnitude * np.array([np.cos(angle), np.sin(angle)])

    forward_matrix = np.matrix([[1/4, 1/4, 1/4, 1/4],
                            [-1/4, 1/4, -1/4, 1/4],
                            [-1/(4*K), -1/(4*K), 1/(4*K), 1/(4*K)]])


    # cosTerm = magnitude * np.cos(angle)
    # sinTerm = magnitude * np.sin(angle)

    V = forward_matrix.dot(omega) * wheel_r

    print(V)

    return V

direction_magnitude = 1.0

list = []
fig, ax, = plt.subplots(subplot_kw={'projection': 'polar'})

for i in np.linspace(-np.pi, np.pi, 9):
    list.append([i, calcMecanumOutput(direction_magnitude, i)])

with np.printoptions(precision=3, suppress=True):
    #print(list)
    #ax.plot(list[0], 1)
    ...

plt.show()