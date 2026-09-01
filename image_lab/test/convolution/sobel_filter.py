import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread("input.jpeg", 0)

Gx = np.array([
    [-1, 0, 1],
    [-2, 0, 2],
    [-1, 0, 1]
], dtype=np.float32)

Gy = np.array([
    [-1, -2, -1],
    [0, 0, 0],
    [1, 2, 1]
], dtype=np.float32)


def manual_convolution(image, kernel):
    rows, cols = image.shape
    k = kernel.shape[0]
    pad = k//2

    padded = np.pad(image,pad,mode='wrap')
    output = np.zeros_like(image, dtype=np.float32)

    for i in range(rows):
        for j in range(cols):
            window = padded[i:i+k, j:j+k]
            output[i,j] = np.sum(window * kernel)
    
    return output

gx = manual_convolution(img,Gx)
gy = manual_convolution(img,Gy)

magnitude = np.sqrt(gx**2 + gy**2)

g = np.clip(magnitude,0,255).astype(np.uint8)

plt.figure(figsize=(12,6))
plt.subplot(1,3,1)
plt.title("Original")
plt.imshow(img,cmap='gray')
plt.axis("off")

plt.subplot(1,3,2)
plt.title("Gx")
plt.imshow(gx,cmap='gray')
plt.axis("off")

plt.show()