# **Finding Lane Lines on the Road** 

## Writeup Template

### You can use this file as a template for your writeup if you want to submit it as a markdown file. But feel free to use some other method and submit a pdf if you prefer.

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


[//]: # (Image References)

[image1]: ./examples/grayscale.jpg "Grayscale"

---

### Reflection

### 1. Describe your pipeline. As part of the description, explain how you modified the draw_lines() function.

My pipeline consisted of 5 steps. 
1. I converted the images to grayscale.
2. I apply gaussian blur to the images
3. I use the canny algorithm to find the edges of the images and use the tool function region_of_interest() to get the edges where I am interested in.
4. I use the draw_lines() function to get the lane lines.
5. I call weighted_img() function to draw the lane lines on the images.

In order to draw a single line on the left and right lanes, I modified the draw_lines() function by checking the slope for all candidate lines from the HoughLinesP() function. The lines with negative slope between - $\pi$/6 and -$\pi$/3 are chosen to be the group of left lane and those with positive slope between $\pi$/6 and $\pi$/3 are chosen to be the group of right lane. After all, average both groups to get two lines ( each line is defined by two points). These lines are then be defined as left lane and right lane. 


### 2. Identify potential shortcomings with your current pipeline

There is some hard-coding part in my code and that cause my code can't pass the opitonal challenge. The region of interest is fixed in my code and is not generalizable.


### 3. Suggest possible improvements to your pipeline

Both lanes in the videos are not robust enough, I think the reason is that there are some line outliers affecting the average. I think an improvement is to use some algorithm to remove the outliers.
