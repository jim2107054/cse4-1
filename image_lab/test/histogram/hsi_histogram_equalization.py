import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread('input.jpeg')
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)


#normalize 
rgb = img.astype(np.float32) / 255.0


R = rgb[:,:,0]
G = rgb[:,:,1]
B = rgb[:,:,2]

#intensity
I = (R+G+B)/3.0

#convert intensity to 0-255
I_unit = np.clip(I*255,0,255).astype(np.uint8)

hist = np.zeros(256, dtype=np.int32)

for pixel in I_unit.flatten():
    hist[pixel]+=1

pdf = hist / I_unit.size
cdf = np.cumsum(pdf)

mapping = np.round(cdf*255).astype(np.uint8)

equalized = mapping[I_unit]


equalized = equalized.astype(np.float32) /255.0

#reconstruct
scale = np.divide(
    equalized,
    I,
    out=np.zeros_like(I),
    where= I>0
)

R_new = np.clip(R*scale,0,1)
G_new = np.clip(G*scale,0,1)
B_new = np.clip(B*scale,0,1) 

result = np.stack([R_new,G_new,B_new],axis=2)
result = (result*255).astype(np.uint8)

plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.imshow(img)
plt.title("Original")
plt.axis("off")

plt.subplot(1, 2, 2)
plt.imshow(result)
plt.title("HSI Intensity Equalization")
plt.axis("off")

plt.show()

