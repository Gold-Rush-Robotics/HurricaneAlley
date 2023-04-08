import math


FIELD_X = 235
FIELD_Y = 119
START_X = 121.92
START_Y = 106.679
# (x, y, width, height)
FOOD1 = (0, 0, 22.86, 30.48)
FOOD2 = (0, 121.919, 22.86, 30.48)
RECYCLE1 = (FIELD_X-30.48, 0, 30.48, 22.86)
RECYCLE2 = (FIELD_X-30.48, FIELD_Y-22.86, 30.48, 22.86)
# (x, y, radius)
POND_INNER = (121.92, FIELD_Y-86.36, 2)
POND_OUTER = (121.92, FIELD_Y-86.36, 7)
PLACE1 = (152.4, FIELD_Y - 15.24, 2)
PLACE2 = (76.2, FIELD_Y - 15.24, 2)

LIGHT_SWITCH = (FIELD_X, FIELD_Y/2)

PI = math.pi

# Vision Constants
MAX_X = 400
MAX_Y = 800
CAMERA_X_CENTER = MAX_X / 2
# Where Cow Catcher is 
COW_CATCHER_Y = 600
# Pixel per cm constant
PIXEL_PER_CM = 20