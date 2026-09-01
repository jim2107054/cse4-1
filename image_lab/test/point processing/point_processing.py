from cv2 import imread
import cv2
import numpy as np 
import matplotlib.pyplot as plt 

img = imread("input.jpeg",cv2.IMREAD_GRAYSCALE)

L = 256

#Negative Transform
neg_img = (L-1)-img

#Log transform
c = (L-1)/np.log(L)
log_img = c*(np.log(1+img.astype(np.float32))).astype(np.uint8)

#Gamma Transform
gamma = 0.5
normalized = img.astype(np.float32)/255.0
gamma_img = c*(normalized**gamma)*255
gamma_img = gamma_img.astype(np.uint8)

#Thresholding
T=150
thre_img = np.where(img>=T,255,0).astype(np.uint8)


plt.subplot(2,3,1)
plt.imshow(img,cmap='gray')
plt.title("Original Image")
plt.axis('off')

plt.subplot(2,3,2)
plt.imshow(neg_img,cmap='gray')
plt.title("Negative Image")
plt.axis('off')

plt.subplot(2,3,3)
plt.imshow(log_img, cmap='gray')
plt.title("Log Transform")
plt.axis('off')

plt.subplot(2,3,4)
plt.imshow(gamma_img, cmap='gray')
plt.title("Gamma Transform")
plt.axis('off')

plt.subplot(2,3,5)
plt.imshow(thre_img, cmap = 'gray')
plt.title("Thresholding")
plt.axis('off')

plt.show()


