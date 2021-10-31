/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include <unistd.h>

#include "helper_functions.h"

using std::string;
using std::vector;

static std::default_random_engine gen;
 
void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */
  num_particles = 100;  // TODO: Set the number of particles

  std::normal_distribution<double> dist_x(x, std[0]);
  std::normal_distribution<double> dist_y(y, std[1]);
  std::normal_distribution<double> dist_theta(theta, std[2]);

  for (int i = 0; i < num_particles; i++)
  {
    Particle p;
    p.id = i;
    p.x  = dist_x(gen);
    p.y  = dist_y(gen);
    p.theta = dist_theta(gen);
    p.weight = 1.0/num_particles;
    particles.push_back(p);
  }
  is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */

  for (int i = 0; i < num_particles; i++)
  { 

    double x, y, theta;
    if(abs(yaw_rate) > 1e-5)
    {
      //x_f = x_0 + (v/\dot{theta})[sin(theta_0)+\dot{theta}*dt - sin(theta_0)]
      x = particles[i].x + velocity/yaw_rate*(sin(particles[i].theta + yaw_rate*delta_t) - sin(particles[i].theta));
      y = particles[i].y + velocity/yaw_rate*(cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*delta_t));
      theta = particles[i].theta + yaw_rate*delta_t;
    }
    else
    {
      // straight
      x = particles[i].x + velocity * cos(particles[i].theta) * delta_t;
      y = particles[i].y + velocity * sin(particles[i].theta) * delta_t ;
      theta = particles[i].theta;
    }

    std::normal_distribution<double> dist_x(x, std_pos[0]);
    std::normal_distribution<double> dist_y(y, std_pos[1]);
    std::normal_distribution<double> dist_theta(theta, std_pos[2]);

    particles[i].x = dist_x(gen);
    particles[i].y = dist_y(gen);
    particles[i].theta = dist_theta(gen);

    
  } 

}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  for (auto& obs : observations) {
        double min_dist = std::numeric_limits<double>::max();

        for (const auto& pred_obs : predicted) {
            double d = dist(obs.x, obs.y, pred_obs.x, pred_obs.y);
            if (d < min_dist) {
                obs.id   = pred_obs.id;
                min_dist = d;
            }
        }
    }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */
  for (int i = 0; i < num_particles ; i++)
  {
    vector<LandmarkObs> mark_candidates; 
    // 1. find the landmark candidates in the sensor range
    int ct = 0;
    for (const auto& landmark : map_landmarks.landmark_list)
    {
      // double distance = sqrt(pow(landmark.x_f - particles[i].x, 2) + pow(landmark.y_f - particles[i].y, 2));
      // use functions in helper_functions.h
      double distance = dist(landmark.x_f, landmark.y_f, particles[i].x, particles[i].y);
      if (distance < sensor_range)
      {
        LandmarkObs mark_obs;
        mark_obs.id = ct++;
        mark_obs.x  = landmark.x_f;
        mark_obs.y  = landmark.y_f;

        mark_candidates.push_back(mark_obs);  
      }
    }
    
    // 2. rotate and translate the observations   
    vector<LandmarkObs> updated_obs;
    double p_theta = particles[i].theta;
    double p_x     = particles[i].x;
    double p_y     = particles[i].y;

    for ( const auto& obs : observations)
    {
      LandmarkObs new_obs;
      new_obs.x = cos(p_theta)*obs.x - sin(p_theta)*obs.y + p_x;
      new_obs.y = sin(p_theta)*obs.x + cos(p_theta)*obs.y + p_y;
      updated_obs.push_back(new_obs);

    }

    // 3. match the observations from sensor and landmark candidates
    dataAssociation(mark_candidates, updated_obs);

    // 4. calculate multivariate gaussian 
    // Add multiv_prob to helper funcionts
    double likelihood = 1.0;
    for (const auto& obs : updated_obs)
    {
      double prob = multiv_prob(std_landmark[0], std_landmark[1], obs.x, obs.y, mark_candidates[obs.id].x, mark_candidates[obs.id].y);
      likelihood *= prob;
    }
    particles[i].weight = likelihood;
    //printf("particles[%d].weight:%lf\n",i, particles[i].weight);
  }


  // Compute norm factor
  double norm_factor = 0.0;
  for (const auto& p : particles)
    norm_factor += p.weight;

  for (auto& p : particles){
    //std::cout << "unnorm weight" << p.weight << std::endl;
    //printf("unnorm p.weight:%lf\n", p.weight);
    p.weight /= (norm_factor + std::numeric_limits<double>::epsilon());
    //std::cout << "norm weight" << p.weight << std::endl;
    //printf("norm p.weight:%lf\n", p.weight);


  }
}

void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */
  vector<double> p_weights;
  for (const auto& p : particles)
    p_weights.push_back(p.weight);

  std::discrete_distribution<int> weighted_distribution(p_weights.begin(), p_weights.end());

  vector<Particle> resampled_paritcles;
  for (int i = 0; i < num_particles; i++)
  {
    int idx = weighted_distribution(gen);
    resampled_paritcles.push_back(particles[idx]);
    // std::cout << "particles weight:" << particles[idx].weight << std::endl;
  }

  particles = resampled_paritcles;

}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}