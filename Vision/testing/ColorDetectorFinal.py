import cv2
import numpy as np
import time
import argparse


cam = cv2.VideoCapture(1)
cam.set(cv2.CAP_PROP_EXPOSURE,-4.0) #-6.5
return_value,img = cam.read()

# let's assume the number of images gotten is 0
img_counter = 0

# while loop
while img_counter < 3:
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
        a = "image["+str(img_counter)+"].png" 
        cv2.imwrite(a, frame)
        print('screenshot taken')
        # the number of images automaticallly increases by 1
        img_counter += 1

imgArr = ["image[0].png","image[1].png","image[2].png","image[3].png","image[4].png"]


img_counter = 0
while img_counter < 3:
    img = cv2.imread(imgArr[img_counter])
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    img2 = cv2.medianBlur(gray,5)
    cimg = cv2.cvtColor(img2,cv2.COLOR_GRAY2BGR)
    
    if(img_counter != 3 or 4):
        circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,120,param1=100,param2=45,minRadius=30,maxRadius=60)
    else:
        circles = cv2.HoughCircles(img2, cv2.HOUGH_GRADIENT, 1,200,param1=100,param2=23,minRadius=30,maxRadius=50)     

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
    hsvFrame = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mean,stddev = cv2.meanStdDev(hsvFrame, mask=mask)[:3]
    
    b = "circle["+str(img_counter)+"].jpg" 
    cv2.imwrite(b, img_circle)
    if(img_counter == 0):
        wu = np.array([min(mean[0]+ 4*stddev[0], 180),min(mean[1]+ 4*stddev[1], 255), min(mean[2]+ 4*stddev[2], 255)],np.uint8)
        wl = np.array([max(mean[0]- 4*stddev[0],0),max(mean[1]- 4*stddev[1],0),max(mean[2]- 4*stddev[2],0)],np.uint8)
    if(img_counter == 1):
        gu = np.array([min(mean[0]+ 4*stddev[0], 180),min(mean[1]+ 4*stddev[1], 255), min(mean[2]+ 4*stddev[2], 255)],np.uint8)
        gl = np.array([max(mean[0]- 4*stddev[0],0),max(mean[1]- 4*stddev[1],0),max(mean[2]- 4*stddev[2],0)],np.uint8)
    if(img_counter == 2):
        ru = np.array([min(mean[0]+ 4*stddev[0], 180),min(mean[1]+ 4*stddev[1], 255), min(mean[2]+ 4*stddev[2], 255)],np.uint8)
        rl = np.array([max(mean[0]- 4*stddev[0],0),max(mean[1]- 4*stddev[1],0),max(mean[2]- 4*stddev[2],0)],np.uint8)
    if(img_counter == 3):
        yu = np.array([min(mean[0]+ 4*stddev[0], 180),min(mean[1]+ 4*stddev[1], 255), min(mean[2]+ 4*stddev[2], 255)],np.uint8)
        yl = np.array([max(mean[0]- 4*stddev[0],0),max(mean[1]- 4*stddev[1],0),max(mean[2]- 4*stddev[2],0)],np.uint8)
    if(img_counter == 4):
        pu = np.array([min(mean[0]+ 4*stddev[0], 180),min(mean[1]+ 4*stddev[1], 255), min(mean[2]+ 4*stddev[2], 255)],np.uint8)
        pl = np.array([max(mean[0]- 4*stddev[0],0),max(mean[1]- 4*stddev[1],0),max(mean[2]- 4*stddev[2],0)],np.uint8)
    img_counter+=1
    
prev_frame_time = 0
new_frame_time = 0


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
    wmask = cv2.inRange(image,wl,wu)
    gmask = cv2.inRange(image,gl,gu)
    rmask = cv2.inRange(image,rl,ru)
    #ymask = cv2.inRange(image,yl,yu)
    #pmask = cv2.inRange(image,pl,pu)

    wcontours, whier = cv2.findContours(wmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    gcontours, ghier = cv2.findContours(gmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    rcontours, rhier = cv2.findContours(rmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    #ycontours, yhier = cv2.findContours(ymask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    #pcontours, phier = cv2.findContours(pmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    
    
    # green - 150
    # white - 300 also 400
    # yellow - 400
    #pink- 500
    # red - 50
    
    if len(wcontours) != 0: 
            for contour in wcontours:
                if cv2.contourArea(contour) > 350:
                    x,y,w,h = cv2.boundingRect(contour)
                    
                    cv2.rectangle(img, (x,y),(x + w, y+ h ),(255,255,255,),3)
    
    if len(gcontours) != 0: 
            for contour in gcontours:
                if cv2.contourArea(contour) > 350:
                    x,y,w,h = cv2.boundingRect(contour)
                    
                    cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,255,0,),3)
    
    if len(rcontours) != 0: 
            for contour in rcontours:
                if cv2.contourArea(contour) > 350:
                    x,y,w,h = cv2.boundingRect(contour)
                    
                    cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,0,255,),3)

#    if len(ycontours) != 0: 
#            for contour in ycontours:
#                if cv2.contourArea(contour) > 350:
#                    x,y,w,h = cv2.boundingRect(contour)
#                    
#                    cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,255,255,),3)
    
#    if len(pcontours) != 0: 
#            for contour in pcontours:
#                if cv2.contourArea(contour) > 350:
#                    x,y,w,h = cv2.boundingRect(contour)
#                    
#                    cv2.rectangle(img, (x,y),(x + w, y+ h ),(204,153,255,),3)

    
    cv2.imshow("webcam", img)

    cv2.waitKey(1)

# save results

cv2.destroyAllWindows()