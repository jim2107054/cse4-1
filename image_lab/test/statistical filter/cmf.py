import cv2
import numpy as np 
import matplotlib.pyplot as plt 

img = cv2.imread(r'Y:\4-1\cse4-1\image_lab\test\input.jpeg',0)

q = 1.5
k = 3
pad = k//2

rows = img.shape[0]
cols = img.shape[1]

padded = np.pad(img, pad, mode='wrap')

result = np.zeros_like(img, dtype=np.float32)

for i in range(rows):
    for j in range(cols):
        window = padded[i:i+k,j:j+k].astype(np.float32)

        numerator = np.sum(window**(q+1))
        denominator = np.sum(window**q)

        if denominator != 0:
            result[i,j] = numerator/denominator
        else:
            result[i,j] = 0

result = np.clip(result, 0,255).astype(np.uint8)

plt.figure(figsize=(12,6))
plt.subplot(1,2,1)
plt.imshow(img,cmap='gray')
plt.title("Original Image")
plt.axis('off')

plt.subplot(1,2,2)
plt.imshow(result,cmap='gray')
plt.title("Filtered Image")
plt.axis('off')
plt.show()

