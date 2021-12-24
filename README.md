<table style="width:100%">
  <tr>
    <th>
      <p align="center">
           <a href="./Project1-Finding-Lane-Lines-on-the-Road"><img src="./Project1-Finding-Lane-Lines-on-the-Road/test_videos_output/solidWhiteCurve.gif" alt="Overview" width="60%" height="60%"></a>
           <br>Project1: Finding Lane Lines on the Road 
        <br><a href="./Project1-Finding-Lane-Lines-on-the-Road/P1.ipynb" name="p1_code">(code notebook)</a>
        <br><a href="./Project1-Finding-Lane-Lines-on-the-Road/writeup_template.md" name="p1_writeup.md">(writeup)</a>
      </p>
    </th>
        <th><p align="center">
           <a href="./Project2-CarND-Advanced-Lane-Lines"><img src="./Project2-CarND-Advanced-Lane-Lines" alt="Overview" width="60%" height="60%"></a>
           <br>Project2: CarND Advanced Lane Lines
           <br><a href="./Project2-CarND-Advanced-Lane-Lines/main.ipynb" name="p2_code">(code notebook)</a>
          <br><a href="./Project2-CarND-Advanced-Lane-Lines/writeup.md" name="p2_writeup.md">(writeup)</a>
        </p>
    </th>
       <th><p align="center">
           <a href="https://www.youtube.com/watch?v=gXkMELjZmCc"><img src="./project_3_behavioral_cloning/img/overview.gif" alt="Overview" width="60%" height="60%"></a>
           <br>P3: Behavioral Cloning
           <br><a href="./project_3_behavioral_cloning" name="p3_code">(code)</a>
        </p>
    </th>
        <th><p align="center">
           <a href="https://www.youtube.com/watch?v=g5BhDtoheE4"><img src="./project_4_advanced_lane_finding/img/overview.gif"                         alt="Overview" width="60%" height="60%"></a>
           <br>P4: Adv. Lane Finding
           <br><a href="./project_4_advanced_lane_finding" name="p4_code">(code)</a>
        </p>
    </th>
  </tr>
  <tr>
    <th><p align="center">
           <a href="https://www.youtube.com/watch?v=Cd7p5pnP3e0"><img src="./project_5_vehicle_detection/img/overview.gif"                         alt="Overview" width="60%" height="60%"></a>
           <br>P5: Vehicle Detection
           <br><a href="./project_5_vehicle_detection" name="p5_code">(code)</a>
        </p>
    </th>
        <th><p align="center">
           <a href="./project_6_extended_kalman_filter"><img src="./project_6_extended_kalman_filter/img/overview.jpg"                         alt="Overview" width="60%" height="60%"></a>
           <br>P6: Ext. Kalman Filter
           <br><a href="./project_6_extended_kalman_filter" name="p6_code">(code)</a>
        </p>
    </th>
    <th><p align="center">
           <a href="./project_7_unscented_kalman_filter"><img src="./project_7_unscented_kalman_filter/img/overview.jpg"                         alt="Overview" width="60%" height="60%"></a>
           <br>P7: Unsc. Kalman Filter
           <br><a href="./project_7_unscented_kalman_filter" name="p7_code">(code)</a>
        </p>
    </th>
    <th><p align="center">
           <a href="./project_8_kidnapped_vehicle"><img src="./project_8_kidnapped_vehicle/img/overview.gif"                         alt="Overview" width="60%" height="60%"></a>
           <br>P8: Kidnapped Vehicle
           <br><a href="./project_8_kidnapped_vehicle" name="p8_code">(code)</a>
        </p>
    </th>
  </tr>
  <tr>
    <th><p align="center">
           <a href="https://www.youtube.com/watch?v=w9CETKuJcVM"><img src="./project_9_PID_control/img/overview.gif"                         alt="Overview" width="60%" height="60%"></a>
           <br>P9: PID Controller
           <br><a href="./project_9_PID_control" name="p9_code">(code)</a>
        </p>
    </th>
    <th><p align="center">
           <a href="./project_10_MPC_control"><img src="./project_10_MPC_control/img/overview.gif"                         alt="Overview" width="60%" height="60%"></a>
           <br>P10: MPC Controller
           <br><a href="./project_10_MPC_control" name="p10_code">(code)</a>
        </p>
    </th>
   <th><p align="center">
           <a href="./project_11_path_planning"><img src="./project_11_path_planning/img/overview.jpg"                         alt="Overview" width="60%" height="60%"></a>
           <br>P11: Path Planning
           <br><a href="./project_11_path_planning" name="p11_code">(code)</a>
        </p>
    </th>
    <th><p align="center">
          <a href="./project_12_road_segmentation"><img src="./project_12_road_segmentation/img/overview.jpg"                         alt="Overview" width="60%" height="60%"></a>
           <br>P12: Road Segmentation
           <br><a href="./project_12_road_segmentation" name="p12_code">(code)</a>
        </p>
    </th>
  </tr>
</table>

# Self-Driving-Car-Engineer-Nanodegree-Program
![alt text](/Project1-Finding-Lane-Lines-on-the-Road/test_images_output/solidWhiteCurve.jpg)
## Project 1

## Project 2

## Project 3

## Project 4

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
