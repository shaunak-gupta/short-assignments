import numpy as np
from matplotlib.pylab import imread, imshow, subplot, title, show, get_cmap, imsave
import sys

def median(lst):
    sortedLst = sorted(lst)
    lstLen = len(lst)
    index = (lstLen - 1) / 2
    return sortedLst[index]

def get_median(arr, row, col, window):
	red = []
	green = []
	blue = []
	for r in range(max(0,row-window), min(arr.shape[0], row+window)):
		for c in range(max(0,col-window), min(arr.shape[1], col+window)):
			red.append(arr[r][c][0])
			green.append(arr[r][c][1])
			blue.append(arr[r][c][2])
	return([median(red), median(green), median(blue)])

def add_noise(image, mean, var):
	row,col,ch= image.shape
	if var == 0:
		return image + (mean + np.zeros(image.shape))
	sigma = var**0.5
	gauss = np.random.normal(mean,sigma,(row,col,ch))
	gauss = gauss.reshape(row,col,ch)
	noisy = image + gauss
	noisy[noisy > 1] = 1
	noisy[noisy < 0] = 0
 	return noisy

def median_filter(image, window):
	new = []
	for r in range(image.shape[0]):
		arr = []
		for c in range(image.shape[1]):
			arr.append(get_median(image, r, c, window))
		new.append(arr)
	return np.array(new)

if len(sys.argv) != 5:
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " <IMAGE_PATH> <WINDOW_SIZE> <NOISE_MEAN> <NOISE_VARIANCE>\n"
    quit()

img = imread(sys.argv[1])
window = int(sys.argv[2])
noise_mean = float(sys.argv[3])
noise_var = float(sys.argv[4])
if (noise_var < 0 or window < 0):
	print "Variance and window size cannot be negative!\n"
	quit()
noisy = add_noise(img, noise_mean, noise_var)
filtered = median_filter(noisy, window)
subplot(131)
imshow(img)
title('Original image')
subplot(132)
imshow(noisy)
title('With gaussian noise')
subplot(133)
imshow(filtered)
title('Median filtered')
show()