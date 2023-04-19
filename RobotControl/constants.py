import math


FIELD_X = 235
FIELD_Y = 119
START_X = 122
START_Y = 103

# Field Entities
# (x, y, width(x), height(y))
FOOD1 = (0, 0, 22.86, 30.48)
FOOD2 = (0, FIELD_Y-30.48, 22.86, 30.48)
RECYCLE1 = (FIELD_X-53.5, 0, 53.5, 22.86)
RECYCLE2 = (FIELD_X-53.5, FIELD_Y-22.86, 53.5, 22.86)
# (x, y, radius)
POND_INNER = (121.92, FIELD_Y-86.36, 2)
POND_OUTER = (121.92, FIELD_Y-86.36, 7)
PLACE1 = (152.4, FIELD_Y - 15.24, 2)
PLACE2 = (76.2, FIELD_Y - 15.24, 2)

LIGHT_SWITCH = (FIELD_X, FIELD_Y/2)

PI = math.pi

# SWEEPS Calculation
X_SEGMENTS = 2
Y_SEGMENTS = 3
X_SWEEP_START = 23.25
# Y_SWEEP_START = FIELD_Y - 30.48
Y_SWEEP_START = FIELD_Y - 45.0
X_END = FIELD_X - 53.5
Y_END = 23.5
HEADING = -PI/4

X_POSITIONS : list = [0]
X_SEG_DISTANCE = (X_END - X_SWEEP_START) / X_SEGMENTS
for x in range(X_SEGMENTS):
	X_POSITIONS.append(X_SWEEP_START + X_SEG_DISTANCE * x)

Y_POSITIONS : list = [0]
Y_SEG_DISTANCE = (Y_SWEEP_START - Y_END) / Y_SEGMENTS
for y in range(Y_SEGMENTS):
	Y_POSITIONS.append(Y_SWEEP_START - Y_SEG_DISTANCE * y)

SWEEPS : list = [0]
for y in Y_POSITIONS:
	for x in X_POSITIONS:
		temp = (x, y, HEADING)
		SWEEPS.append(temp)

# Intake Motor Speeds
INTAKE_MARSH_SPEED = 0.66
INTAKE_DUCK_BAG_SPEED = 1
INTAKE_DUCK_AG_SPEED = 0.83

# For Camera Release
DM_SEPARATOR_MARSHMALLOW = 50 
DM_SEPARATOR_DUCK = 90
DM_RELEASE = 40

# Vision Constants
MAX_X = 400
MAX_Y = 800
CAMERA_X_CENTER = MAX_X / 2
# Where Cow Catcher is 
COW_CATCHER_Y = 600
# Pixel per cm constant
PIXEL_PER_CM = 1