
import cv2
import numpy as np
import time



#def fixHSVRange(h, s, v):
#    # Normal H,S,V: (0-360,0-100%,0-100%)
#    # OpenCV H,S,V: (0-180,0-255 ,0-255)
#    return (180 * h / 360, 255 * s / 100, 255 * v / 100)


yl = np.array([15,123,153])
yu = np.array([33,255,255])

gl = np.array([35,86,110],np.uint8)
gu = np.array([100,255,255],np.uint8)

rl = np.array([0,130,70],np.uint8)
ru = np.array([3,255,255],np.uint8)

pl = np.array([143,104,124],np.uint8)
pu = np.array([174,255,255],np.uint8)



video = cv2.VideoCapture(1)

prev_frame_time = 0
new_frame_time = 0



while True:
    success, img = video.read()


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
    ymask = cv2.inRange(image, yl, yu)
    pmask = cv2.inRange(image,pl,pu)
    rmask = cv2.inRange(image,rl,ru)
    gmask = cv2.inRange(image,gl,gu)

    ycontours, yhier = cv2.findContours(ymask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    pcontours, phier = cv2.findContours(pmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)

    rcontours, rhier = cv2.findContours(rmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    gcontours, ghier = cv2.findContours(gmask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)

    
    # yellow
    if len(ycontours) != 0:
        for contour in ycontours:
            if cv2.contourArea(contour) > 995:
                x,y,w,h = cv2.boundingRect(contour)
                
                cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,255,255,),3)
    #pink
    if len(pcontours) != 0:
        for contour2 in pcontours:
            if cv2.contourArea(contour2) > 995:
                x,y,w,h = cv2.boundingRect(contour2)
                cv2.rectangle(img, (x,y),(x + w, y+ h ),(204,153,255,),3)

    # red
    if len(rcontours) != 0:
        for contour in rcontours:
            if cv2.contourArea(contour) > 995:
                x,y,w,h = cv2.boundingRect(contour)
                
                cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,0,255,),3)
    #green
    if len(gcontours) != 0:
        for contour2 in gcontours:
            if cv2.contourArea(contour2) > 995:
                x,y,w,h = cv2.boundingRect(contour2)
                cv2.rectangle(img, (x,y),(x + w, y+ h ),(0,255,0,),3)


    #cv2.imshow("ymask",ymask)
    #cv2.imshow("pmask2",pmask)
    #cv2.imshow("rmask",rmask)
    #cv2.imshow("gmask2",gmask)
    cv2.imshow("webcam", img)

    cv2.waitKey(1)



