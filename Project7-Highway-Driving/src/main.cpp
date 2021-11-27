#include <uWS/uWS.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "helpers.h"
#include "json.hpp"
#include "spline.h"

#include <cfloat>
#include <ctime>

// for convenience
using nlohmann::json;
using std::string;
using std::vector;


#define KEEP_LANE 0
#define LEFT_CHANGE 1
#define RIGHT_CHANGE 2

clock_t a,b;

// this mean the middle lane, left:0 ,right:2
int lane = 1;

// reference velocity
double target_vel = 49.5;//mph
double ref_vel = 0.0; //mph

vector<int> valid_lanes[3] = {{1},{0,2},{1}};
double cost_function(int intended_speed, int current_speed)
{
  return (current_speed - intended_speed)/current_speed; 
}

int find_state(double prev_size, double car_s, vector<vector<double>> sensor_fusion, int current_lane, int current_vel)
{
  double best_cost = DBL_MAX;
  int best_state = KEEP_LANE;
  vector< std::pair <double, double> > lane_info[3];
  for(int j = 0; j < valid_lanes[current_lane].size(); j++)
  {
    int target_lane = valid_lanes[current_lane][j];
    for (int i = 0; i < sensor_fusion.size(); i++)
    {
      float d = sensor_fusion[i][6];
    
      if (d < (2+4*target_lane+2) && d > (2+4*target_lane-2))
      {
        double vx = sensor_fusion[i][3];
        double vy = sensor_fusion[i][4];
        double check_speed = sqrt(vx*vx+vy*vy);
        double check_car_s = sensor_fusion[i][5];

        check_car_s = sensor_fusion[i][5];

        check_car_s += ((double)prev_size*0.02*check_speed);
        if (check_car_s > car_s)
          lane_info[target_lane].push_back( std::make_pair(check_car_s, check_speed));
      }
    }

    std::sort(lane_info[target_lane].begin(), lane_info[target_lane].end());
    //printf("lane_info[%d].size():%d\n",target_lane,lane_info[target_lane].size());
    if (lane_info[target_lane].size() == 0)
    {
      // if no car in target_lane then intended_speed is the max speed
      double cost = cost_function(target_vel, current_vel);
      //printf("cost:%lf, best_cost:%lf\n", cost, best_cost);
      if (cost < best_cost)
      {
        best_cost = cost;
        if (current_lane == target_lane)
        {
          best_state = KEEP_LANE;
        }
        else if(current_lane - 1 == target_lane)
        {
          best_state = LEFT_CHANGE;
        }
        else if(current_lane + 1 == target_lane)
        {
          best_state = RIGHT_CHANGE;
        }
        else
        {
          // Should not go here;
          std::cout << "Invalid lane number" << std::endl;
          best_state = KEEP_LANE;
        }
      }
    }
    else
    {
      for(int i = 0; i < lane_info[target_lane].size(); i++)
      {
        double check_car_s = lane_info[target_lane][i].first;
        double intended_speed = lane_info[target_lane][i].second;
        std::cout << "check_car_s:" << check_car_s << " car_s:" << car_s << std::endl;
        if(std::abs(check_car_s-car_s) <= 50)
        {
          //invalid
          break;
        }
        else 
        {
          // valid, my car should follow the speed of the closest car in the target_lane
          // since most car will be fast in highway speed
          double cost = cost_function(intended_speed, current_vel);
          if (cost < best_cost)
          {
            best_cost = cost;
            if (current_lane == target_lane)
            {
              best_state = KEEP_LANE;
            }
            else if(current_lane - 1 == target_lane)
            {
              best_state = LEFT_CHANGE;
            }
            else if(current_lane + 1 == target_lane)
            {
              best_state = RIGHT_CHANGE;
            }
            else
            {
              // Should not go here;
              std::cout << "Invalid lane number" << std::endl;
              best_state = KEEP_LANE;
            }
          }
          break;
        }
      } 
    }
    

  }
  return best_state;
}

int main() {
  uWS::Hub h;

  // Load up map values for waypoint's x,y,s and d normalized normal vectors
  vector<double> map_waypoints_x;
  vector<double> map_waypoints_y;
  vector<double> map_waypoints_s;
  vector<double> map_waypoints_dx;
  vector<double> map_waypoints_dy;

  // Waypoint map to read from
  string map_file_ = "../data/highway_map.csv";
  // The max s value before wrapping around the track back to 0
  double max_s = 6945.554;

  std::ifstream in_map_(map_file_.c_str(), std::ifstream::in);

  string line;
  while (getline(in_map_, line)) {
    std::istringstream iss(line);
    double x;
    double y;
    float s;
    float d_x;
    float d_y;
    iss >> x;
    iss >> y;
    iss >> s;
    iss >> d_x;
    iss >> d_y;
    map_waypoints_x.push_back(x);
    map_waypoints_y.push_back(y);
    map_waypoints_s.push_back(s);
    map_waypoints_dx.push_back(d_x);
    map_waypoints_dy.push_back(d_y);
  }



  h.onMessage([&map_waypoints_x,&map_waypoints_y,&map_waypoints_s,
               &map_waypoints_dx,&map_waypoints_dy]
              (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
               uWS::OpCode opCode) {
    
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {

      auto s = hasData(data);

      if (s != "") {
        auto j = json::parse(s);
        
        string event = j[0].get<string>();
        
        if (event == "telemetry") {
          // j[1] is the data JSON object
          
          // Main car's localization Data
          double car_x = j[1]["x"];
          double car_y = j[1]["y"];
          double car_s = j[1]["s"];
          double car_d = j[1]["d"];
          double car_yaw = j[1]["yaw"];
          double car_speed = j[1]["speed"];

          // Previous path data given to the Planner
          auto previous_path_x = j[1]["previous_path_x"];
          auto previous_path_y = j[1]["previous_path_y"];
          // Previous path's end s and d values 
          double end_path_s = j[1]["end_path_s"];
          double end_path_d = j[1]["end_path_d"];

          // Sensor Fusion Data, a list of all other cars on the same side 
          //   of the road.
          auto sensor_fusion = j[1]["sensor_fusion"];

          int prev_size = previous_path_x.size();


          /**
           * TODO: define a path made up of (x,y) points that the car will visit
           *   sequentially every .02 seconds
           */

          // sensor fusion : prevent from collision
          if (prev_size > 0)
          {
            car_s = end_path_s;
          }

          bool too_close = false;
          bool changing_lane = false;

          for (int i = 0; i < sensor_fusion.size(); i++)
          {
            float d = sensor_fusion[i][6];
            if (d < (2+4*lane+2) && d > (2+4*lane-2))
            {
              double vx = sensor_fusion[i][3];
              double vy = sensor_fusion[i][4];
              double check_speed = sqrt(vx*vx+vy*vy);
              double check_car_s = sensor_fusion[i][5];

              check_car_s = sensor_fusion[i][5];

              check_car_s += ((double)prev_size*0.02*check_speed);

              if ((check_car_s > car_s) && ((check_car_s-car_s) < 30))
              {
                // if too close than we need to take action
                too_close = true;
              }
            }
          }
          
          if( car_d == 2+4*lane && changing_lane){
            if ((clock()-a)/CLOCKS_PER_SEC >= 2.0)
              changing_lane = false;
          }

          if (too_close && !changing_lane)
          {
            // too_close we may want to change lane 
            int best_state = find_state(prev_size, car_s, sensor_fusion, lane, ref_vel);
            std::cout << "BEST_STATE:" << best_state << std::endl;
            switch(best_state)
            {
              case KEEP_LANE:
                ref_vel -= 0.224;
                break;
              case LEFT_CHANGE:
                lane -= 1;
                changing_lane = true;
                a=clock();
                break;
              case RIGHT_CHANGE:
                lane += 1;
                changing_lane = true;
                a=clock();
                break;
            } 
          }
          else if (ref_vel < target_vel)
          {
            ref_vel += 0.224;
          }

          // car planning

          vector<double> ptsx, ptsy;
          double ref_x = car_x;
          double ref_y = car_y;
          double ref_yaw = deg2rad(car_yaw);

          // if size is not enough
          if( prev_size < 2) 
          {

            // use the heading direction to guess the previous point
            double prev_car_x = car_x - cos(car_yaw);
            double prev_car_y = car_y - sin(car_yaw);

            ptsx.push_back(prev_car_x);
            ptsx.push_back(car_x);

            ptsy.push_back(prev_car_y);
            ptsy.push_back(car_y);
          }
          else // push two points
          {
            ref_x = previous_path_x[prev_size-1];
            ref_y = previous_path_y[prev_size-1];

            double ref_x_prev = previous_path_x[prev_size-2];
            double ref_y_prev = previous_path_y[prev_size-2];

            ref_yaw = atan2(ref_y-ref_y_prev, ref_x-ref_x_prev);

            ptsx.push_back(ref_x_prev);
            ptsx.push_back(ref_x);

            ptsy.push_back(ref_y_prev);
            ptsy.push_back(ref_y);

          }


          // push three more points

          vector<double> next_wp[3];
          // In Frenet add evenly 30m spaced points ahead of the starting reference
          for (int i = 0; i < 3; i ++)
          {
            // (2+4*lane) is the middle point of the lane. For middle lane, the middle point is 6
            next_wp[i] = getXY(car_s+30*(i+1),(2+4*lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);

            ptsx.push_back(next_wp[i][0]);
            ptsy.push_back(next_wp[i][1]);
          }

          for (int i = 0; i < ptsx.size(); i++)
          {

            // shift car reference angle to 0 degree
            double shift_x = ptsx[i]-ref_x;
            double shift_y = ptsy[i]-ref_y;

            ptsx[i] = (shift_x*cos(0-ref_yaw)-shift_y*sin(0-ref_yaw));
            ptsy[i] = (shift_x*sin(0-ref_yaw)+shift_y*cos(0-ref_yaw));
          }

          
          vector<double> next_x_vals;
          vector<double> next_y_vals;

          for (int i = 0; i < previous_path_x.size(); i++)
          {
            next_x_vals.push_back(previous_path_x[i]);
            next_y_vals.push_back(previous_path_y[i]);

          }
          
          // Calculate how to break up spline points so that we travel at our desired reference velocity
          tk::spline s;
          s.set_points(ptsx, ptsy);
          double target_x = 30.0;
          double target_y = s(target_x);
          double target_dist = sqrt(target_x*target_x+target_y*target_y);

          double x_add_on = 0;
          // Fill up the rest of our path planner after filling it with previous points, here we will always output 50 points
          for (int i = 1; i < 50 - previous_path_x.size(); i++)
          {
            double N = (target_dist/(0.02*ref_vel/2.24));
            double x_point = x_add_on+target_x/N;
            double y_point = s(x_point);

            x_add_on = x_point;

            double x_ref = x_point; 
            double y_ref = y_point;

            // rotate back to normal after rotating it earilier
            x_point = (x_ref*cos(ref_yaw)-y_ref*sin(ref_yaw));
            y_point = (x_ref*sin(ref_yaw)+y_ref*cos(ref_yaw));

            x_point += ref_x;
            y_point += ref_y;

            next_x_vals.push_back(x_point);
            next_y_vals.push_back(y_point);

          }

          
          
          
          json msgJson;
          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;

          auto msg = "42[\"control\","+ msgJson.dump()+"]";

          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}