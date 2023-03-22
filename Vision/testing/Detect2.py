import cv2
import numpy as np
import time
#import argparse


def getROI(frame, x, y, r):
    return frame[int(y-r/2):int(y+r/2), int(x-r/2):int(x+r/2)]


cam = cv2.VideoCapture(1)
return_value,img = cam.read()

#cam = cv2.VideoCapture(1)


# let's assume the number of images gotten is 0
img_counter = 0

# while loop
while img_counter < 1:
    # intializing the frame, ret
    ret, frame = cam.read()
    # if statement
    if not ret:
        print('failed to grab frame')
        break
    # the frame will show with the title of test
    
    #to get continuous live video feed from my laptops webcam
    k  = cv2.waitKey(1)
    # if the escape key is been pressed, the app will stop
    if k%256 == 27:
        print('escape hit, closing the app')
        break
    # if the spacebar key is been pressed
    # screenshots will be taken
    elif k%256  == 32:
        # the format for storing the images scrreenshotted
       
        # saves the image as a png file
        docircle = frame
        gray = cv2.cvtColor(docircle,cv2.COLOR_BGR2GRAY)
        img2 = cv2.medianBlur(gray,5)
        #for marshmellows
        cimg = cv2.cvtColor(img2,cv2.COLOR_GRAY2BGR)
        #circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,120,param1=100,param2=45,minRadius=30,maxRadius=60)
        #For ducks
        
        try:
            circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,150,param1=100,param2=23,minRadius=40,maxRadius=60)
            circles = np.uint16(np.around(circles))
            for i in circles[0,:]:
                cv2.circle(frame,(i[0],i[1]),i[2],(255,0,0),2)
                cv2.circle(frame,(i[0],i[1]),2,(255,0,0),3)
        except:
            print("No circle")


    cv2.imshow('test', frame)





    
#Circle colors
#https://stackoverflow.com/questions/56907198/what-is-the-best-way-to-detect-the-color-of-circles-detected-using-cv2-houghcirc


cv2.destroyAllWindows()