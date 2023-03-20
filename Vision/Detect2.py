import cv2
import numpy as np
import time
import argparse


cam = cv2.VideoCapture(0)
return_value,img = cam.read()

cam = cv2.VideoCapture(0)


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
    cv2.imshow('test', frame)
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
        cv2.imwrite("green.png", frame)
        print('screenshot taken')
        # the number of images automaticallly increases by 1
        img_counter += 1



docircle = cv2.imread("green.png")
gray = cv2.cvtColor(docircle,cv2.COLOR_BGR2GRAY)
img2 = cv2.medianBlur(gray,5)
#for marshmellows
cimg = cv2.cvtColor(img2,cv2.COLOR_GRAY2BGR)
circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,120,param1=100,param2=45,minRadius=30,maxRadius=60)
#For ducks
#circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,150,param1=100,param2=23,minRadius=40,maxRadius=60)
circles = np.uint16(np.around(circles))
for i in circles[0,:]:
    cv2.circle(docircle,(i[0],i[1]),i[2],(255,255,0),2)
    cv2.circle(docircle,(i[0],i[1]),2,(255,255,0),3)
    
#Circle colors
#https://stackoverflow.com/questions/56907198/what-is-the-best-way-to-detect-the-color-of-circles-detected-using-cv2-houghcirc





cv2.imshow("circle", docircle)
cv2.waitKey(0)
cv2.destroyAllWindows()