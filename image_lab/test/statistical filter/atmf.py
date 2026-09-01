from PIL.PyAccess import mode_map
import cv2
import numpy as np 
import matplotlib.pyplot as plt 

img = cv2.imread(r'Y:\4-1\cse4-1\image_lab\test\input.jpeg',0)

k = 5
d = 8
rows, cols = img.shape

pad = k//2
padded = np.pad(img, pad, mode='wrap')

result = np.zeros_like(img).astype(np.float32)

for i in range(rows):
    for j in range(cols):
        window = padded[i:i+k, j:j+k]

        #convert 2D => 1D
        values = window.flatten()

        #sorting
        values = np.sort(values)
        
        # Remove Lowest d/2
        # Remove Highest d/2
        trimmed = values[d//2 : len(values)-d//2]

        #replace center pixel intensity
        result[i,j] = np.mean(trimmed)

result = np.clip(result, 0,255).astype(np.uint8)

plt.figure(figsize=(10,5))
plt.subplot(1,2,1)
plt.imshow(img, cmap='gray')
plt.title('Original')
plt.xticks([]), plt.yticks([])
plt.subplot(1,2,2)
plt.imshow(result, cmap='gray')
plt.title('ATMF')
plt.show()