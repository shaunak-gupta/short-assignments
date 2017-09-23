import numpy as np
import skimage.io
import matplotlib.pyplot as plt

def contrast_stretch(image):
	minval = min(image.flatten())
	maxval = max(image.flatten())
	for i in range(image.shape[0]):
		image[i] = [(x-minval)*255/(maxval-minval) for x in image[i]]
	return image

b1 = skimage.io.imread('band1.gif', as_grey=True)

b1 = contrast_stretch(b1)

ax = plt.gca()
fig = plt.gcf()
implot = ax.imshow(b1, cmap='gray')

river = []
nonriver = []

def onclick(event):
    if event.xdata != None and event.ydata != None:
    	xval = int(event.xdata)
    	yval = int(event.ydata)
    	if len(river)<50:
    		river.append((xval,yval))
    	else:
    		nonriver.append((xval,yval))
        print "Current pixel: ", (xval, yval)
        print "River: ", len(river), " Non-river: ", len(nonriver)
        if (len(nonriver) == 100):
            rivernp = np.array(river)
            nonrivernp = np.array(nonriver)
            np.save('river.npy', rivernp)
            np.save('nonriver.npy', nonrivernp)
            np.savetxt("river.txt", rivernp, delimiter="\n")
            np.savetxt("nonriver.txt", nonrivernp, delimiter="\n")

cid = fig.canvas.mpl_connect('button_press_event', onclick)

plt.show()