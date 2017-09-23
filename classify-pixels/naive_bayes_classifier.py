import numpy as np
import skimage.io
import matplotlib.pyplot as plt
import sys
from scipy.stats import norm

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

river_std = np.std(river_points, axis=0)
nonriver_std = np.std(nonriver_points, axis=0)

prob_river = prior_river*np.multiply(
    np.multiply(norm.pdf(b1, river_mean[0], river_std[0]),norm.pdf(b2, river_mean[1], river_std[1])),
    np.multiply(norm.pdf(b3, river_mean[2], river_std[2]),norm.pdf(b4, river_mean[3], river_std[3]))
    )
prob_nonriver = prior_nonriver*np.multiply(
    np.multiply(norm.pdf(b1, nonriver_mean[0], nonriver_std[0]),norm.pdf(b2, nonriver_mean[1], nonriver_std[1])),
    np.multiply(norm.pdf(b3, nonriver_mean[2], nonriver_std[2]),norm.pdf(b4, nonriver_mean[3], nonriver_std[3]))
    )
binary_img = (prob_river>prob_nonriver)*255

ax = plt.gca()
fig = plt.gcf()
implot = ax.imshow(binary_img, cmap='gray')
plt.show()