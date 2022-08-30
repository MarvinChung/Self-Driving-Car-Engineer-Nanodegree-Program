# **Extended Kalman Filter** 
### Sensor type
Lidar data provides: position  
<img src="imgs/lidar.png" width="480" alt="Combined Image" />	  
Radar data provides: range, bearing and raidal velocity  
<img src="imgs/radar.png" width="480" alt="Combined Image" />

### State
The state for Kalman Filter is [px, py, vx, vy].  
The transformation of lidar data to state is linear.   
<img src="imgs/lidar_h1.png" width="120" alt="Combined Image" />
<img src="imgs/lidar_h2.png" width="120" alt="Combined Image" />  
However, the transformation of radar data to state is non linear. Therefore, we need to use Extended Kalman Filter.  
<img src="imgs/radar_h.png" width="320" alt="Combined Image" />  
The Extended Kalman Filter uses a method called first order Taylor expansion. Details are shown in [sensor-fusion-ekf-reference](sensor-fusion-ekf-reference.pdf)

### Overview image
<img src="imgs/overview.png" width="480" alt="Combined Image" />


### Extended Kalman Filter algorithm
<img src="imgs/algorithm-generalization.jpg" width="480" alt="Combined Image" />

### General flow 
<img src="imgs/general-flow.png" width="480" alt="Combined Image" />

### Result (gif)
Lidar measurements are red circles, radar measurements are blue circles with an arrow pointing in the direction of the observed angle, and estimation markers are green triangles.
