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
        cv2.imwrite("image.png", frame)
        print('screenshot taken')
        # the number of images automaticallly increases by 1
        img_counter += 1



img = cv2.imread("image.png")
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
img2 = cv2.medianBlur(gray,5)
cimg = cv2.cvtColor(img2,cv2.COLOR_GRAY2BGR)
# under not working
#circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,120,param1=100,param2=45,minRadius=30,maxRadius=60)
#For circles prev param 2 was 23
#circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,150,param1=100,param2=24,minRadius=40,maxRadius=60)
circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,200,param1=100,param2=23,minRadius=30,maxRadius=50)
                                                                    #200,param1=100,param2=19,minRadius=30,maxRadius=50               
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

hsvFrame = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
mean,stddev = cv2.meanStdDev(hsvFrame, mask=mask)[:3]

print("average circle color:",mean[0])
print("average circle color:",mean[1])
print("average circle color:",mean[2])

# 3 for white 2 for red and green i think pink and yellow is three
#ru = np.array([min(mean[0]+ 2*stddev[0], 180),min(mean[1]+ 2*stddev[1], 255), min(mean[2]+ 2*stddev[2], 255)],np.uint8)
#rl = np.array([max(mean[0]- 2*stddev[0],0),max(mean[1]- 2*stddev[1],0),max(mean[2]- 2*stddev[2],0)],np.uint8)
ru = np.array([min(mean[0]+ 3*stddev[0], 180),min(mean[1]+ 3*stddev[1], 255), min(mean[2]+ 3*stddev[2], 255)],np.uint8)
rl = np.array([max(mean[0]- 3*stddev[0],0),max(mean[1]- 3*stddev[1],0),max(mean[2]- 3*stddev[2],0)],np.uint8)

print("redL",rl)
print("redU",ru)

''''' For Red and Pink
# get average color with mask  
ave_color = cv2.meanStd(frame, mask=mask)[:3]
print("average circle color:", ave_color)
ru = np.array([min(ave_color[0]+ 10, 255),min(ave_color[1]+ 10, 255), min(ave_color[2]+ 10, 255)],np.uint8)
rl = np.array([max(ave_color[0]- 10,0),max(ave_color[1]- 10,0),max(ave_color[2]- 10,0)],np.uint8)
'''''

prev_frame_time = 0
new_frame_time = 0

cv2.imwrite('green_circle_circle.jpg', img_circle)
cv2.imwrite('green_circle_mask.jpg', mask)
# show images
cv2.imshow('circle', img_circle)
cv2.imshow('mask', mask)


while True:
    success, img = cam.read()


    if not success:
        break
 
    # Our operations on the frame come here
    gray = img
 
    # resizing the frame size according to our need
    gray = cv2.resize(gray, (500, 300))
 
    # font which we will be using to display FPS
    font = cv2.FONT_HERSHEY_SIMPLEX
    # time when we finish processing for this frame
    new_frame_time = time.time()
 
    # Calculating the fps
 
    # fps will be number of frame processed in given time frame
    # since their will be most of time error of 0.001 second
    # we will be subtracting it to get more accurate result
    fps = 1/(new_frame_time-prev_frame_time)
    prev_frame_time = new_frame_time
 
    # converting the fps into integer
    fps = int(fps)
 
    # converting the fps to string so that we can display it on frame
    # by using putText function
    fps = str(fps)
 
    # putting the FPS count on the frame
    cv2.putText(gray, fps, (7, 70), font, 3, (100, 255, 0), 3, cv2.LINE_AA)
 
    # displaying the frame with fps
    cv2.imshow('frame', gray)
    image = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    rmask = cv2.inRange(image,rl,ru)
    rcontours, rhier = cv2.findContours(rmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)

    
    # green - 150
    # white - 300 also 400
    # yellow - 400
    #pink- 500
    # red - 50
    if(img_counter == 0):
        if len(rcontours) != 0: 
            for contour in rcontours:
                if cv2.contourArea(contour) > 50:
                    x,y,w,h = cv2.boundingRect(contour)
                    
                    cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,0,255,),3)

    
    cv2.imshow("webcam", img)

    cv2.waitKey(1)

# save results

cv2.destroyAllWindows()