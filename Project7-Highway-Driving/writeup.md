## Project7-HighWay Driving
![alt text](/Project7-Kidnapped-Vehicle-Project/results/Project7.png)
###  Run
- [origin project repo](https://github.com/udacity/CarND-Path-Planning-Project)  
- [Download simulator](https://github.com/udacity/self-driving-car-sim/releases/tag/T3_v1.2)  
```
./run.sh
# Open the simulator and choose Kidnapped vehicle
# Click start
```
### Intro 
1. Waypoints (Spline)
Use waypoints to update the car trajectory.
2. Finite State machine (If else)
Use finite state machine to decide when to change lanes. If the car in front of is too slow and change lane is safe, then we need to change lane.
![alt text](/Project7-Kidnapped-Vehicle-Project/results/change_lane.png) 

### Result
<img src="./Project7-Highway-Driving/results/result.jpg" alt="Overview" width="60%" height="60%"></a>