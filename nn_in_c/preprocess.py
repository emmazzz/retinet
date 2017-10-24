import cv2
import numpy as np
import math
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



# cap = cv2.VideoCapture(0)
# i = 0
# while(True):
#     ret, frame = cap.read()
#     rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2BGRA)
#     i+=1
#     cv2.imshow('frame', rgb)
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         out = cv2.imwrite('capture%d.jpg'%(i), frame)
#     if cv2.waitKey(1) & 0xFF == ord('l'):
#         break

# cap.release()


# # cv2.destroyAllWindows()

# ##############################
# from matplotlib import pyplot as plt

def crop_image(img,tol=1):
    # img is image data
    # tol  is tolerance
    # mask = img<tol
    mask = img>127
    return img[np.ix_(mask.any(1),mask.any(0))]

def recenter(cropped, s = 28):
    w, h = np.shape(cropped)
    ml = max(w, h)
    pad = ml//16
    l = ml + 2*pad
    l = math.ceil(l//28) *28
    final = np.zeros((l,l), dtype=np.int)
    final.fill(0)
    sw = (l-w)//2
    sh = (l-h)//2
    final[sw:sw+w,sh:sh+h] = cropped
    return final

def resize(input, s = 28):
    w, h = np.shape(input)
    assert(w == h)
    assert(w//s*s == w)
    final = np.zeros((s,s), dtype=np.int)

    d = w//s
    for r in range(s):
        for c in range(s):
            total = 0
            for dr in range(d):
                for dc in range(d):
                    total += input[r*d+dr,c*d+dc]
            total //= (d**2)
            final[r,c] = total
    return final


                    



# read image as breyscale
img = cv2.imread("m3.jpg", 0);

# turn into binary
ret,thresh = cv2.threshold(img,127,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C)

# crop out white bg
cropped = crop_image(thresh)

cv2.imwrite('pre1.jpg', cropped)
# recenter it
result = recenter(cropped)
cv2.imwrite('pre2.jpg', result)

#resize to 26 by 26
img = cv2.imread("pre2.jpg", 0);

resized_image = resize(result)
# resized_image = cv2.resize(img, (28, 28)) 

cv2.imwrite('pre33.jpg',resized_image)
byte_array = np.ndarray.flatten(resized_image)
print(byte_array)

with open("processed", 'wb') as output:
    output.write(byte_array)
    # output.write(bytearray([(255-x) for x in byte_array]))




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


