import cv2
import numpy as np
import json
from enum import Enum


#Tunables to increase speed and decrease accuracy

MERGE_PASSES = 5 
"""this is how many itterations of bounding boxes for each color detection will merge to become bigger
Higher = Slower? more work for each frame but less objects for the main loop to deal with"""
CAMERA_NUM = 1
'''Which camera to use (0 for pi, 1 if running with an external and internal webcam)''' 

def detectColisions(rects: list[tuple[int, int, int, int]], depth: int) -> list[tuple[int, int, int, int]]:
    print(f"Detect: {rects}, {depth}")
    if(depth == 0):
        return rects
    old_rects = [x for x in rects]
    new_rects = []
    last = None
    #do a collision pass
    for i in range(len(rects)):
        cur_rect = rects.pop()
        if(last != None):
            new_rects.extend(colisionDetection(cur_rect, last))
        else:
            new_rects.append(cur_rect)
        last = cur_rect
    #if we didnt merge any dont bother continuing the recursion
    if new_rects == old_rects:
        return old_rects
    return detectColisions(new_rects, depth-1)


def colisionDetection(rect1: tuple[int, int, int, int], rect2: tuple[int, int, int, int]) -> list[tuple[int, int, int, int]]:
    '''
    Collision detection:
        takes two rectangles, and returns a list containing:
            no colision: both
            a collision: a rect that contains both rectangles
        params:
            rect1: (x, y, width, height)
            rect2: (x, y, width, height)
    '''
    
    colision =  rect1[0] < rect2[0] + rect2[2] and\
                rect1[0] + rect1[2] > rect2[0] and\
                rect1[1] < rect2[1] + rect2[3] and\
                rect1[1] + rect1[3] > rect2[1]
    if(colision):
        '''convert to top left, bottom right '''
        r1 = (rect1[0], rect1[1], rect1[0] + rect1[2], rect1[1] + rect1[3])
        r2 = (rect2[0], rect2[1], rect2[0] + rect2[2], rect2[1] + rect2[3])
        overlap = (min(r1[0], r2[0]), min(r1[1], r2[1]), max(r1[2], r2[2]), max(r1[3], r2[3]))
        '''convert back into x, y, w, h'''
        overlap = (overlap[0], overlap[1], overlap[2] - overlap[0], overlap[3] - overlap[1])
        return [overlap]
    print(f"colid {rect1}, {rect2}")
    return [rect1, rect2]



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
    def blob(self, frame:cv2.Mat, color_space:ColorSpace) -> list[tuple[int, int, int]]:
        '''
        return: (center x, center y, area)
        '''
        if(not color_space == self.color_space):
            frame = cv2.cvtColor(frame, self.color_space)
        mask = cv2.inRange(frame, self.lower, self.upper)
        cv2.imshow("Hello", cv2.bitwise_and(frame, frame, mask=mask))
        contours, hierarchy = cv2.findContours(mask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
        boundingBoxes = []
        print(len(contours))
        if(len(contours) != 0):
            for contour in contours:
                if(cv2.contourArea(contour) >= 300):
                    boundingBoxes.append(cv2.boundingRect(contour))
        print(f"pre {boundingBoxes}")
        boundingBoxes = detectColisions(boundingBoxes, MERGE_PASSES)
        print(f"post: {boundingBoxes}")
        a = list([(x+(w/2), y+(h/2), w*h) for x, y, w, h in boundingBoxes])
        print(a)
        return a
        
        


class VisionBuilder:
    def __init__(self) -> None:
        self.green = ColorType(ColorSpace.BGR)
        self.white = ColorType(ColorSpace.BGR)
        self.red = ColorType(ColorSpace.BGR)
        self.yellow = ColorType(ColorSpace.BGR)
        self.pink = ColorType(ColorSpace.BGR)
        self.video = cv2.VideoCapture(CAMERA_NUM)
        self.video.set(cv2.CAP_PROP_EXPOSURE,-6.5) #-6.5

    def loadFromFile(self) -> None:
        ranges ={}
        with open("colorVals.json", "r") as f:
            ranges = json.loads(f.read())
        self.green.updateLower(ranges["greenL"])
        self.green.updateUpper(ranges["greenU"])

        self.white.updateLower(ranges["whiteL"])
        self.white.updateUpper(ranges["whiteU"])

        self.red.updateLower(ranges["redL"])
        self.red.updateUpper(ranges["redU"])

        self.yellow.updateLower(ranges["yellowL"])
        self.yellow.updateUpper(ranges["yellowU"])

        self.pink.updateLower(ranges["pinkL"])
        self.pink.updateUpper(ranges["pinkU"])

    def detectMarshmellows(self) -> list[tuple[int, int, int, int]]:
        '''given a frame, will return a list of detected marshmellows 

        Type:
        * 0 - Green
        * 1 - White
        * 2 - Red

        return: (type, center x, center y, area)
        '''
        ret, frame = self.video.read()
        objList = []
        objList.extend([(b[0], b[1], b[2], 0) for b in self.green.blob(frame, ColorSpace.BGR)])
        objList.extend([(b[0], b[1], b[2], 1) for b in self.white.blob(frame, ColorSpace.BGR)])
        objList.extend([(b[0], b[1], b[2], 2) for b in self.red.blob(frame, ColorSpace.BGR)])
        return objList
    
    def detectDucks(self) -> list[tuple[int, int, int, int]]:
        '''given a frame, will return a list of detected Ducks 

        Type:
        * 3 - Yellow
        * 4 - Pink

        return: (type, center x, center y, area)
        '''
        ret, frame = self.video.read()
        objList = []
        objList.extend([(b[0], b[1], b[2], 3) for b in self.yellow.blob(frame, ColorSpace.BGR)])
        objList.extend([(b[0], b[1], b[2], 4) for b in self.pink.blob(frame, ColorSpace.BGR)])
        return objList

    def detectRedFood(self) -> bool:
        """will figure out what food area is under the camera

        Returns:
            bool: true if red
        """
        ret, frame = self.video.read()
        r, g, b = cv2.mean(frame)
        return r >= g
        


if __name__ == "__main__":
    vb = VisionBuilder()
    vb.loadFromFile()
    while True:
        ret, frame = vb.video.read()
        cv2.imshow("test",frame)
        marsh = vb.detectMarshmellows()
        print(marsh)
        if cv2.waitKey(1) == ord('q'):
            break

        
        

    