import cv2
import numpy as np
import matplotlib.pyplot as plt

def log_kernel(size,sigma):
    kernel = np.zeros((size,size), dtype=np.float32)
    center = size//2
    for i in range(size):
        for j in range(size):
            x = i-center
            y = j-center
            exponent = -(x**2 + y**2)/(2*sigma**2)
            coeff = -1/(np.pi*sigma**4)
            kernel[i,j] = coeff * (1 + exponent) * np.exp(exponent) 
            kernel-=np.mean(kernel)
    return kernel 

def manual_convolution(img, kernel):
    rows, cols = img.shape
    k = kernel.shape[0]
    pad = k//2
    padded = np.pad(img,pad,mode='wrap')

    output = np.zeros((rows,cols), dtype=np.float32)

    for i in range(rows):
        for j in range(cols):
            #window 
            region = padded[i:i+k,j:j+k]
            output[i,j] = np.sum(region*kernel)
    
    return output

img = cv2.imread("input.jpeg",cv2.IMREAD_GRAYSCALE)
kernel = log_kernel(5,1.2)
output = manual_convolution(img,kernel)
output=cv2.normalize(output,None,0,255,cv2.NORM_MINMAX)
output = np.uint8(output)

plt.figure(figsize=(12,6))
plt.subplot(1,2,1)
plt.imshow(img, cmap="gray")
plt.title("Original")
plt.axis("off")
plt.subplot(1,2,2)
plt.imshow(output, cmap="gray")
plt.title("LoG")
plt.axis("off")
plt.show()
            