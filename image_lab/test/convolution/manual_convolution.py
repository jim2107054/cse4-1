import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread(r"Y:\4-1\cse4-1\image_lab\test\input.jpeg", cv2.IMREAD_GRAYSCALE)

def gaussian_kernel(size, sigma):
    kernel = np.zeros((size, size))
    center = size // 2
    for i in range(size):
        for j in range(size):
            x = i - center
            y = j - center

            kernel[i, j] = np.exp(
                -(x*x + y*y) / (2*sigma*sigma)
            )
    kernel = kernel / np.sum(kernel)
    return kernel

kernel = gaussian_kernel(3,5)

def manual_convolution(img, kernel):
    rows, cols = img.shape
    k = kernel.shape[0]

    pad = k//2

    img_pad = np.pad(img, pad, mode='wrap')
    result = np.zeros((rows,cols), dtype=np.float32)

    #Flip kernel for conv
    # kernel = np.flip(kernel)
    # Manually flipped kernel
    flipped_kernel = np.zeros_like(kernel)
    for i in range(k):
        for j in range(k):
            flipped_kernel[i,j] = kernel[k-1-i, k-1-j]

    # Sliding window
    for i in range(rows):
        for j in range(cols):
            # Element wise multiplication
            region = img_pad[i:i+k, j:j+k]
            result[i,j] = np.sum(region * flipped_kernel)

    return np.clip(result,0,255).astype(np.uint8)

out_img = manual_convolution(img,kernel)

plt.figure(figsize=(10,5))

plt.subplot(1,2,1)
plt.title("Original Image")
plt.imshow(img, cmap='gray')
plt.axis('off')

plt.subplot(1,2,2)
plt.title("Gaussian Blurred Image")
plt.imshow(out_img, cmap='gray')
plt.axis('off')

plt.show()
