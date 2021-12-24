# **Extended Kalman Filter** 
### Sensor type
Lidar data provides: position  
<img src="imgs/lidar.png" width="480" alt="Combined Image" />	  
Radar data provides: range, bearing and raidal velocity  
<img src="imgs/radar.png" width="480" alt="Combined Image" />

### State
The state for Kalman Filter is [px, py, vx, vy].  
The transformation of lidar data to state is linear. 
However, the transformation of radar data to state is non linear. Therefore, we need to use Extendd Kalman Filter.   
The Extended Kalman Filter uses a method called first order Taylor expansion. Details are shown in [sensor-fusion-ekf-reference](sensor-fusion-ekf-reference.pdf)

### Overview image
<img src="imgs/overview.png" width="480" alt="Combined Image" />


### Extended Kalman Filter algorithm
<img src="imgs/algorithm-generalization.jpg" width="480" alt="Combined Image" />

### General flow 
<img src="imgs/general-flow.png" width="480" alt="Combined Image" />
