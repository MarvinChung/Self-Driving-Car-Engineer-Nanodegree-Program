## Project9-System-Integration
![alt text](/Project9-System-Integration/stop.png)
###  Run
- [origin project repo](https://github.com/udacity/CarND-Capstone)  
- [Download simulator](https://github.com/udacity/self-driving-car-sim/releases/tag/T3_v1.2)  
```
docker build . -t capstone
docker run -p 4567:4567 -v $PWD:/capstone -v /tmp/log:/root/.ros/ --rm -it capstone
cd ros
catkin_make
source devel/setup.sh
roslaunch launch/styx.launch
```

### Intro 
This project learns to use ROS and we need to let the car stop when the traffic light is red.
