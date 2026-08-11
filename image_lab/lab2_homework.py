"""
CSE 4102 - Lab 02 - Homework (A2, B2)
Histogram Matching: Equalize an RGB image by matching only the L channel
(in Lab color space) to the L-channel histogram of a reference image.

Author: <your name>
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt

# ---------------------------------------------------------
# Load images
# ---------------------------------------------------------
boat_bgr = cv2.imread('image_lab/lab2_boat.jpg')
boat_rgb = cv2.cvtColor(boat_bgr, cv2.COLOR_BGR2RGB)
boat_lab = cv2.cvtColor(boat_bgr, cv2.COLOR_BGR2LAB)

power_bgr = cv2.imread('image_lab/lab2_power_plant.jpg')
power_rgb = cv2.cvtColor(power_bgr, cv2.COLOR_BGR2RGB)
power_lab = cv2.cvtColor(power_bgr, cv2.COLOR_BGR2LAB)

L_src = boat_lab[:, :, 0]
L_ref = power_lab[:, :, 0]

print("L_src min/max:", L_src.min(), L_src.max(), "shape:", L_src.shape)
print("L_ref min/max:", L_ref.min(), L_ref.max(), "shape:", L_ref.shape)

# ---------------------------------------------------------
# Compute PDFs and CDFs (normalized histograms)
# ---------------------------------------------------------
hist_src, _ = np.histogram(L_src.flatten(), bins=256, range=[0, 256])
pdf_src = hist_src / hist_src.sum()
cdf_src = pdf_src.cumsum()

hist_ref, _ = np.histogram(L_ref.flatten(), bins=256, range=[0, 256])
pdf_ref = hist_ref / hist_ref.sum()
cdf_ref = pdf_ref.cumsum()

print("PDF src max:", pdf_src.max())
print("PDF ref max:", pdf_ref.max())

# ---------------------------------------------------------
# Perform Histogram Matching on the L channel
# For each value r in 0..255, find z in 0..255 such that cdf_ref[z] ~ cdf_src[r]
# ---------------------------------------------------------
lookup_table = np.zeros(256, dtype=np.uint8)
for r in range(256):
    diff = np.abs(cdf_src[r] - cdf_ref)
    lookup_table[r] = np.argmin(diff)

L_out = lookup_table[L_src]

hist_out, _ = np.histogram(L_out.flatten(), bins=256, range=[0, 256])
pdf_out = hist_out / hist_out.sum()
cdf_out = pdf_out.cumsum()

out_lab = boat_lab.copy()
out_lab[:, :, 0] = L_out
out_bgr = cv2.cvtColor(out_lab, cv2.COLOR_LAB2BGR)
out_rgb = cv2.cvtColor(out_bgr, cv2.COLOR_BGR2RGB)

# ---------------------------------------------------------
# Plot to verify appearance
# ---------------------------------------------------------
fig, axes = plt.subplots(3, 3, figsize=(15, 8))

# Row 1
axes[0, 0].imshow(boat_rgb)
axes[0, 0].set_title("Input Image")
axes[0, 0].axis('off')
axes[0, 1].plot(pdf_src, color='red')
axes[0, 1].set_title("Source PDF")
axes[0, 1].set_ylim(0, 0.04)
axes[0, 2].plot(cdf_src, color='black')
axes[0, 2].set_title("Source CDF - S(r)")
axes[0, 2].set_ylim(0, 1.05)

# Row 2
axes[1, 0].imshow(power_rgb)
axes[1, 0].set_title("Reference Image")
axes[1, 0].axis('off')
axes[1, 1].plot(pdf_ref, color='green')
axes[1, 1].set_title("Reference PDF")
axes[1, 1].set_ylim(0, 0.04)
axes[1, 2].plot(cdf_ref, color='green')
axes[1, 2].set_title("Reference CDF - G(z)")
axes[1, 2].set_ylim(0, 1.05)

# Row 3
axes[2, 0].imshow(out_rgb)
axes[2, 0].set_title("Output Image")
axes[2, 0].axis('off')
axes[2, 1].plot(pdf_out, color='blue')
axes[2, 1].set_title("Output PDF")
axes[2, 1].set_ylim(0, 0.04)
axes[2, 2].plot(cdf_out, color='blue')
axes[2, 2].set_title("Output CDF")
axes[2, 2].set_ylim(0, 1.05)

plt.tight_layout()
plt.savefig('image_lab/lab2_histogram_matching_result.png', dpi=150)
print("Saved lab2_histogram_matching_result.png")

cv2.imwrite('image_lab/lab2_output_image.png', cv2.cvtColor(out_rgb, cv2.COLOR_RGB2BGR))
print("Saved lab2_output_image.png")