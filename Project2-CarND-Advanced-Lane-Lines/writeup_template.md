## Writeup 

### You can use this file as a template for your writeup if you want to submit it as a markdown file, but feel free to use some other method and submit a pdf if you prefer.

---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image1]: ./examples/undistort_output.png "Undistorted"
[image2]: ./test_images/test1.jpg "Road Transformed"
[image3]: ./examples/binary_combo_example.jpg "Binary Example"
[image4]: ./examples/warped_straight_lines.jpg "Warp Example"
[image5]: ./examples/color_fit_lines.jpg "Fit Visual"
[image6]: ./examples/example_output.jpg "Output"
[video1]: ./project_video.mp4 "Video"

## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points

### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---

### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the first code cell of the IPython notebook located in `main.ipynb In [1]`.

I assume the object points is fixed on a planar therefore its z should equal to 0. The (x,y) of object points are share with all calibration images. Then I use `cv2.findChessboardCorners` to get the corners and save them as the image points. Finally, I pass the object points and image points to `cv2.calibrateCamera` to obtain distortion coefficients and camera matrix.

The result is shown as below:
##### Original image
![alt text][./output_images/calibration1.jpg]
##### Undistort image
![alt text][./output_images/calibration1_undistort.jpg]

### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.
The code is locate at `main.ipynb In [2]`
I pass the the source image, distortion coefficients and camera matrix to `cv2.undistort` to obtain this result:  
![alt text][./output_images/test1_undistort.jpg]

#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.

The code is locate at `main.ipynb In [5]`.
I used a combination of color and gradient thresholds to generate a binary image. First, I applying the sobel filter with direction 'x' on the image then thresholding the image to generate a mask. Then, I generate another mask by thresholding the saturation channel in HSV. I then combined these mask using OR operation to generate the final mask. The final mask is equialvalent to the binary image. 
Here's an example of my output for this step.  

![alt text][./output_images/test1_binary.jpg]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.
The code is locate at `main.ipynb In [7]`
I choose to hardcode the source and destination points in the following manner. 
```python
src_points = np.float32(
    [[585, img.shape[0]//2+100],
    [200, img.shape[0]],
    [1190, img.shape[0]],
    [730, img.shape[0]//2+100]])
dst_points = np.float32([[32
    0, 0], 
    [320, 720], 
    [960, 720], 
    [960, 0]])
```
I then pass the source and destination points to `cv2.getPerspectiveTransform` to get the transform matrix. Then I pass the transform matrix and my source image to `cv2.warpPerspective` to obtain a bird-view image.
Here’s an example of my output for this step.

![alt text][output_images/test1_bird_view_comparison.jpg]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

The code is locate at `main.ipynb In [11] and In [12]`

First I pass the bird-view binary image to `find_lane_pixels` function at `In[11]`. To fine the two lines in the road, I divide the image into left half and right half and calculate the histogram. Since the lower part of the image means closer to the car, we also only consider the lower half pixels for the histogram. We then find the peak value in the histogram for both parts. The location of the peak value will be consider as the start point of the line. Then I use windows to identify the line pixels from bottom to top. The mean x value of the line pixels in current window will be the middle point of the next window. I keep doing these steps until the termination. I have modified the margin parameter from the example in the course. I think a lower margin is more stable.
The parameters in this step is shown as follows:


| nwindows |  margin | minpix |
| -------- | -------- | -------- |
| 9     | 50     | 50     |

After I get the x,y values of pixels in both lines. I fit the x and y values using `np.polyfit` and assume the polynomial is in second order. The line is then draw on the binary image which is shown below:



![alt text][./output_images/test1_line_on_warp.jpg]

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

The code is locate at `main.ipynb In [14] or In[24]`
```
def calc_radius_of_curvature(poly_fit, y_closest_to_vehicle):
    return ((1 + (2*poly_fit[0]*y_closest_to_vehicle*ym_per_pix + poly_fit[1])**2)**1.5) / np.absolute(2*poly_fit[0])
```
I use the above function to calucalte the radius of carvature. The poly_fit is the polynomial obtained from the previous step. I assume the vehicle is located at the center in the image. Therefore, I count the `(left_line_x_value + right_line_x_value)/2 - midpoint of the image`
will get the position of the vehicle respect to center.

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

The code is locate at `main.ipynb In [19]`
The result is shown as belows:


![alt text][./output_images/test1_line_image.jpg]

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

Here's a [link to my video result](./test_videos_output/project_video.mp4)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

Here I'll talk about the approach I took, what techniques I used, what worked and why, where the pipeline might fail and how I might improve it if I were going to pursue this project further. 
The line doesn't work well in both challenge tasks. When the input image has a great color difference on the ground. My algorithm doesn't detect well. I assume this is because the gradient mask doesn't apply well, therefore I assume applying a gaussian to the image or some bluring technique may make it better.
