from pydualsense import *
import sys
from RobotControl.utils import reMap

# def cross_down(state):
#     print(f'cross {state}')


# def circle_down(state):
#     print(f'circle {state}')


# def dpad_down(state):
#     print(f'dpad down {state}')

# def gyro_changed(pitch, yaw, roll):
#     print(f'{pitch}, {yaw}, {roll}')

# Joystick position lists
lStickPos = [0,0]
rStickPos = [0,0]
mecanumVector = [0.0, 0.0, 0.0]

# Drivebase speed multiplier constants
TRIGGER_THRESH = 96
SLOW_SPEED = 0.3
NORM_SPEED = 0.5
FAST_SPEED = 1.0

speedMultiplier = NORM_SPEED

JOYSTICK_MAX = 128
JOYSTICK_MIN = -127


# Event Handlers
def lStick(stateX, stateY):
    global lStickPos
    lStickPos = [stateX, stateY]

def rStick(stateX, stateY):
    global rStickPos
    rStickPos = [stateX, stateY]

def drivebaseMultiplier(state):
    global speedMultiplier
    print(f"in multi {state}, {type(state)}")

    if(type(state) is int and dualsense.state.R2 > TRIGGER_THRESH):
        speedMultiplier = SLOW_SPEED
    elif(type(state) is bool and dualsense.state.R1):
        speedMultiplier = FAST_SPEED
    else:
        speedMultiplier = NORM_SPEED


# create dualsense
dualsense = pydualsense()
# find device and initialize
dualsense.init()

try:
    # add events handler functions
    # dualsense.cross_pressed += cross_down
    # dualsense.circle_pressed += circle_down
    # dualsense.dpad_down += dpad_down
    # dualsense.gyro_changed += gyro_changed
    dualsense.left_joystick_changed += lStick
    dualsense.right_joystick_changed += rStick
    dualsense.r1_changed += drivebaseMultiplier
    dualsense.r2_changed += drivebaseMultiplier

    # read controller state until R1 is pressed
    while not dualsense.state.L1:
        print(f"Joysticks: L: {lStickPos}, R: {rStickPos} Multiplier: {speedMultiplier}")

        mecanumVector = [0.0, 0.0, 0.0]

        print(f"hi")
        

    # close device
    dualsense.close()

except Exception as e:
    dualsense.close()
    sys.exit(e)