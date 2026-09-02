import cv2
import numpy as np
import matplotlib.pyplot as plt 

img = cv2.imread(r"image_lab/test/input.jpeg")

#BGR format e thake
channels = cv2.split(img)

result_channels = []

for channel in channels:
    hist = np.zeros(256, dtype=np.uint64)

    for pixel in channel.flatten():
        hist[pixel] +=1

    pdf = hist/channel.size

    cdf = np.cumsum(pdf)
    mapping = (cdf*255).astype(np.uint8)

    equalized = mapping[channel]
    result_channels.append(equalized)

result_img = cv2.merge(result_channels)

#BGR -> RGB

img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
result_rgb = cv2.cvtColor(result_img, cv2.COLOR_BGR2RGB)

plt.figure(figsize=(12,6))

plt.subplot(2,2,1)
plt.imshow(img)
plt.title("Original")
plt.axis('off')

plt.subplot(2,2,2)
plt.imshow(result_img)
plt.title("BGR Image")
plt.axis('off')

plt.subplot(2,2,3)
plt.imshow(img_rgb)
plt.title("RGB Image")
plt.axis('off')

plt.subplot(2,2,4)
plt.imshow(result_rgb)
plt.title("RGB Equalized Image")
plt.axis('off')

plt.show()
