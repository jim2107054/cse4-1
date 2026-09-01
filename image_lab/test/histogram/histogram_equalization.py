from numpy import int64
import cv2
import numpy as np 
import matplotlib.pyplot as plt 

img = cv2.imread('input.jpeg', cv2.IMREAD_GRAYSCALE)

#Image Dimensions
row,col = img.shape
total_pixels = row*col 

hist = np.zeros(256, dtype=int64)

#Histogram Calculation
for i in range(row):
    for j in range(col):
        intensity = img[i,j]
        hist[intensity] += 1

#PDF
pdf = hist / total_pixels

#CDF
cdf = np.zeros(256)
cdf[0] = pdf[0]

for i in range(1,256):
    cdf[i] = cdf[i-1] + pdf[i]

#Transformation
mapping = np.round(255*cdf).astype(np.uint8)


#equalization
equalized = np.zeros_like(img)

for i in range(row):
    for j in range(col):
        equalized[i,j] = mapping[img[i,j]]

plt.figure(figsize=(12, 8))

plt.subplot(2, 2, 1)
plt.imshow(img, cmap="gray")
plt.title("Original")
plt.axis("off")

plt.subplot(2, 2, 2)
plt.imshow(equalized, cmap="gray")
plt.title("Equalized")
plt.axis("off")

plt.show()
