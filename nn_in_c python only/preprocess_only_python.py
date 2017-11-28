import cv2
import numpy as np
import math
import os
import serial
import time
import testLeNet
import copy
###############################
def crop_image(img,tol=1):
    # img is image data
    # tol  is tolerance
    # mask = img<tol
    mask = img>127
    return img[np.ix_(mask.any(1),mask.any(0))]

def recenter(cropped, s = 28):
    w, h = np.shape(cropped)
    if w == 0 or h == 0: return None
    ml = max(w, h)
    pad = ml//12
    l = ml + 2*pad
    l = math.ceil(l//28) *28
    final = np.zeros((l,l), dtype=np.int)
    final.fill(0)
    sw = (l-w)//2
    sh = (l-h)//2
    if sw <= 0 or sh <=0: return None
    final[sw:sw+w,sh:sh+h] = cropped #there an error here dunno why
    return final

def resize(input, s = 28):
    w, h = np.shape(input)
    assert(w == h)
    assert(w//s*s == w)
    final = np.zeros((s,s), dtype=np.int)

    d = w//s
    if d ==0: return final
    for r in range(s):
        for c in range(s):
            total = 0
            for dr in range(d):
                for dc in range(d):
                    total += input[r*d+dr,c*d+dc]

            total //= (d**2)
            final[r,c] = total
    return final

def getOnePic(cap):
    i = 1000
    while(True):
        ret, frame = cap.read()
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2BGRA)
        i+=1
        # cv2.namedWindow("frame", cv2.WINDOW_NORMAL)
        # cv2.resizeWindow('frame', 600,600)

        cv2.imshow('frame', rgb)
        key = cv2.waitKey(100)
        # print(key)
        if key & 0xFF == ord('t'):
            out = cv2.imwrite('capture.jpg', frame)
            break
        if key & 0xFF == ord('q'):
            break

def clip_on(img, img2):

    h1 = len(img)
    w1 = len(img[0])
    h2 = len(img2)   
    w2 = len(img2[0])


    m = 4
    img_c = np.zeros((h2*m, w2*m))
    for r in range(h2*m):
        for c in range(w2*m):
            img_c[r][c] = img2[r//m][c//m]
    h2 = h2*m
    w2 = w2*m
    res = copy.deepcopy(img)
   

    res[h1-h2:h1,w1-w2:w1] = img_c
    return res


WIDTH = 1920//2
HEIGHT = 1080//2
cap = cv2.VideoCapture(0)
cv2.namedWindow("frame", cv2.WINDOW_NORMAL)
cv2.resizeWindow('frame', WIDTH,HEIGHT)

t = 0
# sd = serial.Serial("/dev/tty.usbmodem1411",115200)
# sd = serial.Serial("/dev/tty.SLAB_USBtoUART",115200)
refreshTime = 1.0
time_stamp = time.time()

# Write some Text

font                   = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10,500)
fontScale              = 1
fontColor              = (50,50,50)
lineType               = 2

timetook = 0
img_save = [[]]
result = "nothing"
while t < 300:
    # took picture named "capture.jpg"
    
    if (time.time() - time_stamp) < refreshTime:
        # print("time", str(time.time() - time_stamp))
        ret, frame = cap.read()
        img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        #Display the image
        # Create a black image
        img = cv2.resize(img, (WIDTH, HEIGHT))
        img = np.flipud(np.fliplr(img))
        img = np.array(img)

        cv2.putText(img,'Retinet sees a %s at time %f'%(result, timetook), 
        bottomLeftCornerOfText, 
        font, 
        fontScale,
        fontColor,
        lineType)
        img = clip_on(img, img_save)
        cv2.imshow("frame",img)
        cv2.waitKey(1)
        continue
    print("Iteration: ", str(t))
    time_stamp = time.time()
    # means time to take another picture for analyzing:






    start = time.time()
    # time.sleep(1) 
    ret, frame = cap.read()
    # cv2.imwrite('capturelala%d.jpg'%t, frame)
    # getOnePic(cap)
    
    # preprocess image
    # img = cv2.imread("capture1.jpg", 0);
    img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # print(type(img))
    # print(np.shape(img))
    # print(img2)
    


    ret,thresh = cv2.threshold(img,127,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C)
    cropped = crop_image(thresh)
    result = recenter(cropped)
    if result is None: continue
    
    resized_image = resize(result)
    
    byte_array = np.ndarray.flatten(resized_image)
    byte_array = byte_array[::-1]
    img_save = np.reshape(byte_array, (28, 28))

    cv2.imwrite('testing%d.jpg'%(t%10), img_save)

    # write to os
    # resultstr = " ".join(["%03d"%x for x in byte_array])+" "
    
    # result = testLeNet.get_result(np.reshape(byte_array, (28*28, 1)))

    result = testLeNet.get_500result(np.reshape(byte_array, (28*28, 1)))
    result = [ int(x[0]*10000) for x in result]
    resultstr = ",".join(["%08d"%x for x in result])+","
    
    ########################################################################

    file_handler = open("somenumbers.txt", "w")
    file_handler.write(resultstr)
    file_handler.close()
    os.system('gcc lenet_inf.c')
    os.system('./a.out')
    file_handler2 = open("result.txt", "r")
    result = file_handler2.read()
    # print("final out put is :", result)



    # print(resultstr)
    ########################################################################
    # Create a black image
    img = cv2.resize(img, (WIDTH, HEIGHT))
   
    img = np.flipud(np.fliplr(img))
    
    img = np.array(img)


    
    ret_result = result
    timetook = time.time()-start
    cv2.putText(img,'Retinet sees a %s at time %f'%(ret_result, timetook), 
        bottomLeftCornerOfText, 
        font, 
        fontScale,
        fontColor,
        lineType)

    #Display the image
    img = clip_on(img, img_save)
    cv2.imshow("frame",img)
    cv2.waitKey(1)

    # print(t)
    print()



    # with open("processed.txt", 'w') as output:
    #     output.write(result)
    # tmp = os.system("python2 sendToUart.py")

    # # print("hello")
    # with open("label.txt", "r") as f:
    #     result = f.read()
    #     print(result)

    t+=1




cap.release()
cv2.destroyAllWindows()






###############################
###############################
###############################
# Windows dependencies
# - Python 2.7.6: http://www.python.org/download/
# - OpenCV: http://opencv.org/
# - Numpy -- get numpy from here because the official builds don't support x64:
#   http://www.lfd.uci.edu/~gohlke/pythonlibs/#numpy

# Mac Dependencies
# - brew install python
# - pip install numpy
# - brew tap homebrew/science
# - brew install opencv


# cap = cv2.VideoCapture(1)
# i = 0
# while(True):
#     ret, frame = cap.read()
#     rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2BGRA)
#     i+=1
#     cv2.imshow('frame', rgb)
#     # if cv2.waitKey(1) & 0xFF == ord('t'):
#     #     out = cv2.imwrite('capture%d.jpg'%(i), frame)
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# cap.release()
# cv2.destroyAllWindows()

# ##############################
# from matplotlib import pyplot as plt




# for i in [0,1,2,3,4,5,6,7,8,9]:
#     # read image as breyscale
#     img = cv2.imread("capture%d.jpg"%i, 0);

#     # turn into binary
#     ret,thresh = cv2.threshold(img,127,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C)

#     # crop out white bg
#     cropped = crop_image(thresh)

#     # cv2.imwrite('pre1-%d.jpg'%i, cropped)
#     # recenter it
#     result = recenter(cropped)
#     # cv2.imwrite('pre2-%d.jpg'%i, result)

#     #resize to 26 by 26
#     img = cv2.imread("pre3-%d.jpg"%i, 0);

#     resized_image = resize(result)
#     # resized_image = cv2.resize(img, (28, 28)) 

#     cv2.imwrite('pre4-%d.jpg'%i,resized_image)
#     byte_array = np.ndarray.flatten(resized_image)


#     # option1 write to os
#     # result = " ".join(["%03d"%x for x in byte_array])
#     # os.system("")
#     # print(result)


#     #option2 write to file
#     # print(byte_array)
#     with open("processed%d"%i, 'wb') as output:
#         output.write(byte_array)







    # with output.write(bytearray([(255-x) for x in byte_array]))
    # with open("processed%d"%i, 'rb') as output:




# # # Now find contours in it. There will be only one object, so find bounding rectangle for it.
# p = cv2.findContours(thresh,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)

# print(p)
# # contours,hierarchy = cv2.findContours(thresh,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
# cnt = p[1]
# x,y,w,h = cv2.boundingRect(cnt)
# # # # Now crop the image, and save it into another file.

# crop = img[y:y+h,x:x+w]
# cv2.imwrite('sofwinres.png',crop)

# print(np.shape(img))
# for i in range(1920):
#   print(img[500][i])






# # Read image
# im = cv2.imread("capture.jpg", 0);
 
# # Set up the detector with default parameters.
# detector = cv2.SimpleBlobDetector_create()
 
# # Detect blobs.
# keypoints = detector.detect(im)
 
# # Draw detected blobs as red circles.
# # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
# im_with_keypoints = cv2.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
 
# # Show keypoints
# cv2.imshow("Keypoints", im_with_keypoints)
# cv2.waitKey(0)



############################
# maybe use adaptive threashold 
# import cv2
# import numpy as np
# from matplotlib import pyplot as plt
# img = cv2.imread('sudoku.png',0)
# img = cv2.medianBlur(img,5)
# ret,th1 = cv2.threshold(img,127,255,cv2.THRESH_BINARY)
# th2 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_MEAN_C,\
#             cv2.THRESH_BINARY,11,2)
# th3 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
#             cv2.THRESH_BINARY,11,2)
# titles = ['Original Image', 'Global Thresholding (v = 127)',
#             'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding']
# images = [img, th1, th2, th3]
# for i in xrange(4):
#     plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')
#     plt.title(titles[i])
#     plt.xticks([]),plt.yticks([])
# plt.show()


