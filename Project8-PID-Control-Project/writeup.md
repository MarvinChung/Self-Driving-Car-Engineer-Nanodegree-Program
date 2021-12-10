# CarND-Controls-PID

## Describe the effect each of the P, I, D components had in your implementation.
In this project, First, I manual tune the P, I component and set D to zero. I find out that I component is very sensitive since the value is easy to become too big. Therefore, I set I to a small value. P component is the value to encorage the car to stay in the middle line. D component is effective when turing left or right, it encourages the car to turn faster. The I component seems to encourage the car to be close to middle of the lane when the car stays in left or right for a long time. 

## Describe how the final hyperparameters were chosen.
I have tried to use twiddle and update paramters while the car driving but the paramter changes too frequently. Therfore, I fix P value and I value to 0.05 and 1e-4. Then I manually tune the D value to 2.0 and find that the car performs well when turning left and left. However, after the car turns round, the car does not stay in the middle perfectly. Therefore, I increase the I value.  At last I choose P = 0.05, I = 5e-3, D = 3.5.