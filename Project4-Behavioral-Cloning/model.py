import os
import csv
import cv2
import numpy as np
import sklearn
import tensorflow as tf
from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Convolution2D, Cropping2D, Dropout
import math
from sklearn.utils import shuffle
from sklearn.model_selection import train_test_split


samples = []
with open('./data/driving_log.csv') as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        samples.append(line)
        
train_samples, validation_samples = train_test_split(samples[1:], test_size=0.2)

def generator(samples, batch_size=32):
    num_samples = len(samples)
    while 1: # Loop forever so the generator never terminates
        #shuffle(samples)
        for offset in range(0, num_samples, batch_size):
            batch_samples = samples[offset:offset+batch_size]

            images = []
            angles = []
            for batch_sample in batch_samples:
                name = './data/IMG/'+batch_sample[0].split('/')[-1]
                center_image = cv2.imread(name)
                center_angle = float(batch_sample[3])

                # debug
                # cv2.imshow('center_image',center_image)
                # cv2.waitKey()

                # original image
                images.append(center_image)
                angles.append(center_angle)

                # flip image
                images.append(cv2.flip(center_image, 1))
                angles.append(-center_angle)

            X_train = np.array(images).astype('float32')
            y_train = np.array(angles).astype('float32')
            
            yield (X_train, y_train)

# Set our batch size
batch_size = 32 # Will cause batch_size * 2, because including flip

# compile and train the model using the generator function
train_generator = generator(train_samples, batch_size=batch_size)
validation_generator = generator(validation_samples, batch_size=batch_size)

row, col, ch = 160, 320, 3 

model = Sequential()
# Preprocess incoming data, centered around zero with small standard deviation 
model.add(Cropping2D(cropping=((50,20), (0,0)), input_shape=(row, col, ch)))
model.add(Lambda(lambda x: x / 127.5 - 1.))
model.add(Convolution2D(24, 5, strides=(2,2), activation='relu'))
model.add(Dropout(0.2))
model.add(Convolution2D(36, 5, strides=(2,2), activation='relu'))
model.add(Dropout(0.2))
model.add(Convolution2D(48, 5, strides=(2,2), activation='relu'))
model.add(Dropout(0.2))
model.add(Convolution2D(64, 3, strides=(2,2), activation='relu'))
model.add(Dropout(0.2))
model.add(Convolution2D(64, 3, strides=(2,2), activation='relu'))
model.add(Dropout(0.2))
model.add(Flatten())
model.add(Dense(100, activation='relu'))
model.add(Dense(50, activation='relu'))
model.add(Dense(10, activation='relu'))
model.add(Dense(1))

model.summary()
model.compile(loss='mse', optimizer='adam')
model.fit_generator(train_generator, 
            steps_per_epoch=math.ceil(len(train_samples)/batch_size), 
            validation_data=validation_generator, 
            validation_steps=math.ceil(len(validation_samples)/batch_size), 
            epochs=10, verbose=1)
model.save('model.h5')
