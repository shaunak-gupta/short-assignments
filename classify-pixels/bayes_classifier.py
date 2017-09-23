import numpy as np
import skimage.io
import matplotlib.pyplot as plt
import sys
from scipy.stats import multivariate_normal

def contrast_stretch(image):
	minval = min(image.flatten())
	maxval = max(image.flatten())
	for i in range(image.shape[0]):
		image[i] = [(x-minval)*255/(maxval-minval) for x in image[i]]
	return image

def get_image(filename):
    img = skimage.io.imread(filename, as_grey=True)
    img = contrast_stretch(img)
    return img

if len(sys.argv) != 2:
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " PROB_RIVER\n"
    quit()

prior_river = float(sys.argv[1])
prior_nonriver = 1-prior_river

b1 = get_image('band1.gif')
b2 = get_image('band2.gif')
b3 = get_image('band3.gif')
b4 = get_image('band4.gif')

river_coordinates = np.load('river.npy')
nonriver_coordinates = np.load('nonriver.npy')

river_points = [[b1[x[1]][x[0]], b2[x[1]][x[0]], b3[x[1]][x[0]], b4[x[1]][x[0]]] for x in river_coordinates]
nonriver_points = [[b1[x[1]][x[0]], b2[x[1]][x[0]], b3[x[1]][x[0]], b4[x[1]][x[0]]] for x in nonriver_coordinates]

river_mean = np.mean(river_points, axis=0)
nonriver_mean = np.mean(nonriver_points, axis=0)

river_cov = np.cov(river_points, rowvar=False)
nonriver_cov = np.cov(nonriver_points, rowvar=False)

input_points = np.zeros((b1.shape[0],b1.shape[1],4))

for i in range(b1.shape[0]):
    for j in range(b1.shape[1]):
        input_points[i][j] = [b1[i][j], b2[i][j], b3[i][j], b4[i][j]]

prob_river = prior_river*multivariate_normal.pdf(input_points, mean=river_mean, cov=river_cov)
prob_nonriver = prior_nonriver*multivariate_normal.pdf(input_points, mean=nonriver_mean, cov=nonriver_cov)

binary_img = (prob_river>prob_nonriver)*255

ax = plt.gca()
fig = plt.gcf()
implot = ax.imshow(binary_img, cmap='gray')
plt.show()