import os
import csv
import cv2
import numpy as np
import sklearn
import tensorflow as tf
import math
from sklearn.utils import shuffle
from sklearn.model_selection import train_test_split

samples = []
with open('./data/driving_log.csv') as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        samples.append(line)
# for sample in samples[1:]:
# 	name = './data/IMG/'+sample[0].split('/')[-1]
# 	center_image = cv2.imread(name)
# 	center_angle = float(sample[3])

# 	# debug
# 	cv2.imshow('center_image',center_image)
# 	cv2.imshow('flip_image', cv2.flip(center_image, 1))
# 	cv2.waitKey()
name = './data/IMG/'+samples[200][0].split('/')[-1]
center_image = cv2.imread(name)
cv2.imshow('center_image', center_image)
cv2.imshow('flip_image', cv2.flip(center_image, 1))
cv2.waitKey()