## Project6-Kidnapped-Vehicle-Project
![alt text](/Project6-Kidnapped-Vehicle-Project/results/Project6.png)
###  Run
- [origin project repo](https://github.com/udacity/CarND-Kidnapped-Vehicle-Project)  
- [Download simulator](https://github.com/udacity/self-driving-car-sim/releases)  
```
./run.sh
# Open the simulator and choose Kidnapped vehicle
# Click start
```
### Intro 
Implement a particle filter to find the location of the vehicle
- Overview  
![alt text](/Project6-Kidnapped-Vehicle-Project/results/overview.png)
1. Init the particles (vehile posible location) by gps location.  
2. predict the particles position based on the prediction formula.  
![alt text](/Project6-Kidnapped-Vehicle-Project/results/prediction_formula.png)
3. Trasform sensors' observation to partilce corordicate
4. Calculate distance from landmarks to vehicles and distance from landmark to vehicles(by sensor). Those particles which have similar distance should be more likely be the vehicle location.
5. Update the likelihood of those particles with high probability using multivariate gaussian formula    
![alt text](/Project6-Kidnapped-Vehicle-Project/results/multivariate.png)  
(Since we are in 2d, only x and y is considered. We assume they are independent, therefore the covariance matrix will be diagonal).
6. Resample the particles based on the likelihood.
7. Repeat these steps
- Summary
After update, We can then obtain the vehicle positions based on the particles clustering (In this project we choose one particle with highest probability to be the vehicle position). The  resample process choose particles with higher probability to be the initialization for next step. 
