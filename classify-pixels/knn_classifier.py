import numpy as np
import skimage.io
import matplotlib.pyplot as plt
import sys
from sklearn.neighbors import KNeighborsClassifier

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
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " k\n"
    quit()

k = int(sys.argv[1])

b1 = get_image('band1.gif')
b2 = get_image('band2.gif')
b3 = get_image('band3.gif')
b4 = get_image('band4.gif')

river_coordinates = np.load('river.npy')
nonriver_coordinates = np.load('nonriver.npy')

river_points = [[b1[x[1]][x[0]], b2[x[1]][x[0]], b3[x[1]][x[0]], b4[x[1]][x[0]]] for x in river_coordinates]
nonriver_points = [[b1[x[1]][x[0]], b2[x[1]][x[0]], b3[x[1]][x[0]], b4[x[1]][x[0]]] for x in nonriver_coordinates]

input_points = np.zeros((b1.shape[0],b1.shape[1],4))

for i in range(b1.shape[0]):
    for j in range(b1.shape[1]):
        input_points[i][j] = [b1[i][j], b2[i][j], b3[i][j], b4[i][j]]

neigh = KNeighborsClassifier(n_neighbors=k)
neigh.fit(np.concatenate((river_points,nonriver_points)), np.concatenate((np.ones(len(river_points)), np.zeros(len(nonriver_points)))))

binary_img = np.zeros((b1.shape[0],b1.shape[1]))
for i in range(b1.shape[0]):
    binary_img[i] = neigh.predict(input_points[i])*255

ax = plt.gca()
fig = plt.gcf()
implot = ax.imshow(binary_img, cmap='gray')
plt.show()