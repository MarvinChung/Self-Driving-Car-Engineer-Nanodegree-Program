# **Traffic Sign Recognition** 

## Writeup

### You can use this file as a template for your writeup if you want to submit it as a markdown file, but feel free to use some other method and submit a pdf if you prefer.

---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./examples/visualization.jpg "Visualization"
[image2]: ./examples/grayscale.jpg "Grayscaling"
[image3]: ./examples/random_noise.jpg "Random Noise"
[image4]: ./examples/placeholder.png "Traffic Sign 1"
[image5]: ./examples/placeholder.png "Traffic Sign 2"
[image6]: ./examples/placeholder.png "Traffic Sign 3"
[image7]: ./examples/placeholder.png "Traffic Sign 4"
[image8]: ./examples/placeholder.png "Traffic Sign 5"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

You're reading it! and here is a link to my [project code](https://github.com/MarvinChung/Self-Driving-Car-Engineer-Nanodegree-Program/tree/main/Project3-Traffic-Sign-Classifier/Traffic_Sign_Classifier.ipynb)

### Data Set Summary & Exploration

#### 1. Provide a basic summary of the data set. In the code, the analysis should be done using python, numpy and/or pandas methods rather than hardcoding results manually.

I used the pandas library to calculate summary statistics of the traffic
signs data set:

* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
* The shape of a traffic sign image is (32, 32, 3)
* The number of unique classes/labels in the data set is 43

#### 2. Include an exploratory visualization of the dataset.

Here is an exploratory visualization of the data set.   

<img src="visualization.png" width="480" alt="Combined Image" />
This is the bar chart showing the histogram of labels in training data   

<img src="bar.png" width="480" alt="Combined Image" />
### Design and Test a Model Architecture

#### 1. Describe how you preprocessed the image data. What techniques were chosen and why did you choose these techniques? Consider including images showing the output of each preprocessing technique. Pre-processing refers to techniques such as converting to grayscale, normalization, etc. (OPTIONAL: As described in the "Stand Out Suggestions" part of the rubric, if you generated additional data for training, describe why you decided to generate additional data, how you generated the data, and provide example images of the additional data. Then describe the characteristics of the augmented training set like number of images in the set, number of images for each class, etc.)

As a first step, I decided to convert the images to grayscale because ...

Here is an example of a traffic sign image before and after grayscaling.

<img src="normal.png" width="480" alt="Combined Image" />

<img src="grayscale.png" width="480" alt="Combined Image" />

As a last step, I normalized the image data using "Min-max normalization". The reason I use this is because I hope my images are not highly influenced by the pixel values. The "Min-max normalization" makes sure all the image has maximum value at 1.0 and minimum value at 0.0.


#### 2. Describe what your final model architecture looks like including model type, layers, layer sizes, connectivity, etc.) Consider including a diagram and/or table describing the final model.

My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x1 Grayscale normalized image   							| 
| Convolution 5x5     	| 1x1 stride, VALID padding, outputs 28x28x6 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride, VALID padding, outputs  14x14x6 	|
| Dropout 	      	    | keep prob = 0.8 				                |
| Convolution 5x5	    | 1x1 stride, VALID padding, outputs 10x10x16  	|
| RELU					|												|
| Max pooling	      	| 2x2 stride, VALID padding, outputs  5x5x16 	|
| Dropout 	      	    | keep prob = 0.8 				                |
| Flatten               | outputs = 400                                 |
| Fully connected		| outputs = 120         						|
| RELU					|												|
| Dropout 	      	    | keep prob = 0.8 				                |
| Fully connected		| outputs = 84           						|
| RELU					|												|
| Dropout 	      	    | keep prob = 0.8 				                |
| Fully connected		| outputs = 43           						|
 


#### 3. Describe how you trained your model. The discussion can include the type of optimizer, the batch size, number of epochs and any hyperparameters such as learning rate.

The hyperparamters for training my model is shown as below:


| hyperparameter | value |
| -------- | -------- | 
| Optimizer     | Adam     | 
| Batch size     | 128     | 
| learning rate     | 1e-3     |
| max epochs     |  30     |
| keep prob     |  0.8     |
| Early stop condition | valid acc >= 0.93 |

I use "tf.nn.softmax_cross_entropy_with_logits" to calucalte the loss of my netowkrs and use the Adam optimizer to update the parameters. Although I set the max epochs to 30, the model satisfies the early stop condition in 7 epochs.


#### 4. Describe the approach taken for finding a solution and getting the validation set accuracy to be at least 0.93. Include in the discussion the results on the training, validation and test sets and where in the code these were calculated. Your approach may have been an iterative process, in which case, outline the steps you took to get to the final solution and why you chose those steps. Perhaps your solution involved an already well known implementation or architecture. In this case, discuss why you think the architecture is suitable for the current problem.

My final model results were:
* training set accuracy of 0.980
* validation set accuracy of 0.937
* test set accuracy of 0.913

If an iterative approach was chosen:
* What was the first architecture that was tried and why was it chosen? 
The Lenet structure is used first.
* What were some problems with the initial architecture?
The model looks like overfit since validation accuracy is decreasing but training accuaracy is increasing.
* How was the architecture adjusted and why was it adjusted? Typical adjustments could include choosing a different model architecture, adding or taking away layers (pooling, dropout, convolution, etc), using an activation function or changing the activation function. One common justification for adjusting an architecture would be due to overfitting or underfitting. A high accuracy on the training set but low accuracy on the validation set indicates over fitting; a low accuracy on both sets indicates under fitting.
I add dropout after some layers.
* Which parameters were tuned? How were they adjusted and why?
I spend time on tuning "keep_prob" parameters of the dropout. I set it to 0.5 at first and the training accuracy increases slowly. Therefore, I choose a higher probability and it produce better results. 
* What are some of the important design choices and why were they chosen? For example, why might a convolution layer work well with this problem? How might a dropout layer help with creating a successful model?
Convolution layers considers spatial information of the image. Therefore, I think this is the reason why it performs better than using fully connected layers. The dropout layer prevents the model from overfitting.

If a well known architecture was chosen:
* What architecture was chosen?
I choose to use Lenet with some modifications.
* Why did you believe it would be relevant to the traffic sign application?
I think it may be a good start using a well-known structure.
* How does the final model's accuracy on the training, validation and test set provide evidence that the model is working well?
 The original Lenet structure performs well on training accuracy. However it does not achieve well in validation and test set. Therefore, I add dropout into it.

### Test a Model on New Images

#### 1. Choose five German traffic signs found on the web and provide them in the report. For each image, discuss what quality or qualities might be difficult to classify.

Here are five German traffic signs that I found on the web:

<img src="./web_data/00001.png" width="480" alt="Combined Image" />
<img src="./web_data/00002.png" width="480" alt="Combined Image" />
<img src="./web_data/00003.png" width="480" alt="Combined Image" />
<img src="./web_data/00004.png" width="480" alt="Combined Image" />
<img src="./web_data/00005.png" width="480" alt="Combined Image" />

The first image might be difficult to classify because it is quite bended.
The second image and the third image are a little fuzzy.
The forth image is darker than the others.
The fifth image has a strong light spot in its right.

#### 2. Discuss the model's predictions on these new traffic signs and compare the results to predicting on the test set. At a minimum, discuss what the predictions were, the accuracy on these new predictions, and compare the accuracy to the accuracy on the test set (OPTIONAL: Discuss the results in more detail as described in the "Stand Out Suggestions" part of the rubric).

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| Speed limit (30km/h)  | Speed limit (30km/h)             				| 
| Keep right     		| Keep right 								    |
| Turn right ahead	    | Turn right ahead								|
| Right-of-way at the next intersection	      		| Right-of-way at the next intersection					 				|
| Keep right			| Keep right      							    |


The model was able to correctly guess 5 of the 5 traffic signs, which gives an accuracy of 100%. This compares favorably to the accuracy on the test set of 91.3%

#### 3. Describe how certain the model is when predicting on each of the five new images by looking at the softmax probabilities for each prediction. Provide the top 5 softmax probabilities for each image along with the sign type of each probability. (OPTIONAL: as described in the "Stand Out Suggestions" part of the rubric, visualizations can also be provided such as bar charts)

The code for making predictions on my final model is located in the 11th cell of the Ipython notebook.

For the first image, the model is relatively sure that this is a "Speed limit (30km/h)" sign (probability of 0.976). The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .976        			| Speed limit (30km/h)   						| 
| .012     				| Speed limit (70km/h) 							|
| .008					| Speed limit (50km/h)							|
| .003	      			| Speed limit (20km/h)			 				|
| .000				    | Speed limit (80km/h)							|

For the second image, the model is relatively sure that this is a "Keep right" sign (probability of 1.0). The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| Keep right               						| 
| .00    				| Turn left ahead    							|
| .00					| Speed limit (60km/h)							|
| .00	      			| No vehicles             		 				|
| .00				    | Yield             							|


For the third image, the model is relatively sure that this is a "Turn right ahead" sign (probability of 1.0). The top five soft max probabilities were
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| Turn right ahead         						| 
| .00    				| No entry          							|
| .00					| Stop                							|
| .00	      			| Keep left             		 				|
| .00				    | Go straight or left  							| 

For the forth image, the model is relatively sure that this is a "Right-of-way at the next intersection" sign (probability of 1.0). The top five soft max probabilities were
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| Right-of-way at the next intersection			| 
| .00    				| Beware of ice/snow       						|
| .00					| Double curve         							|
| .00	      			| Pedestrians            		 				|
| .00				    | Slippery road                					|

For the fifth image, the model is relatively sure that this is a "Keep right" sign (probability of 1.0). The top five soft max probabilities were
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| Keep right               						| 
| .00    				| Turn left ahead    							|
| .00					| Speed limit (60km/h)							|
| .00	      			| Yield              		 				    |
| .00				    | No vehicles            				        |

### (Optional) Visualizing the Neural Network (See Step 4 of the Ipython notebook for more details)
#### 1. Discuss the visual output of your trained network's feature maps. What characteristics did the neural network use to make classifications?
The first image is the output of the first convolution layer and the second image is the output of the second convolution layer. The first output looks seems like the model is finding the contour of each symbol. The second output is hard to described, I guess the model is finding some pattern inside the sign.
<img src="conv1.png" width="480" alt="Combined Image" />
<img src="conv2.png" width="480" alt="Combined Image" />

