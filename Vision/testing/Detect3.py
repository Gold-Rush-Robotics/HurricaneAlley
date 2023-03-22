import cv2
import numpy as np
import time
import argparse


cam = cv2.VideoCapture(1)
return_value,img = cam.read()




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



img = cv2.imread("green.png")
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
img2 = cv2.medianBlur(gray,5)
#for marshmellows
cimg = cv2.cvtColor(img2,cv2.COLOR_GRAY2BGR)
#circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,120,param1=100,param2=45,minRadius=30,maxRadius=60)
#For ducks
circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,150,param1=100,param2=23,minRadius=40,maxRadius=60)


# draw circles
img_circle = img.copy()
mask = np.zeros_like(gray)
for circle in circles[0]:
    # draw the circle in the output image, then draw a rectangle
    # corresponding to the center of the circle
    (x,y,r) = circle
    x = int(x)
    y = int(y)
    r = int(r)
    cv2.circle(img_circle, (x, y), r, (0, 0, 255), 2)
    cv2.circle(mask, (x, y), r, 255, -1)

# get average color with mask  
ave_color = cv2.mean(img, mask=mask)[:3]
print("average circle color:", ave_color)

# save results
cv2.imwrite('green_circle_circle.jpg', img_circle)
cv2.imwrite('green_circle_mask.jpg', mask)

# show images
cv2.imshow('circle', img_circle)
cv2.imshow('mask', mask)
cv2.waitKey(0)
cv2.destroyAllWindows()