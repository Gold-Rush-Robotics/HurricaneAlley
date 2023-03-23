import cv2
import numpy as np
import json
from enum import Enum

class ColorSpace(Enum):
    BGR = cv2.COLOR_BGR2BGRA
    HSV = cv2.COLOR_BGR2HSV
    LAB = cv2.COLOR_BGR2LAB

class ColorType:
    def __init__(self, color_space:ColorSpace) -> None:
        self.color_space = color_space
        self.upper = np.array([255, 255, 255], np.uint8)
        self.lower = np.array([  0,   0,   0], np.uint8)
    def updateUpper(self, vals: list[int]) -> None:
        self.upper = np.array(vals, np.uint8)
    def updateLower(self, vals: list[int]) -> None:
        self.lower = np.array(vals, np.uint8)
    def blob(self, frame:cv2.Mat, color_space:ColorSpace) -> list[tuple[int]]:
        if(not color_space == self.color_space):
            frame = cv2.cvtColor(frame, self.color_space)
        mask = cv2.inRange(frame, self.lower, self.upper)
        contours, hierarchy = cv2.findContours(mask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)



class VisionBuilder:
    def __init__(self) -> None:
        self.green = ColorType(ColorSpace.BGR)
        self.white = ColorType(ColorSpace.BGR)
        self.red = ColorType(ColorSpace.BGR)
        self.yellow = ColorType(ColorSpace.BGR)
        self.pink = ColorType(ColorSpace.BGR)
        

    