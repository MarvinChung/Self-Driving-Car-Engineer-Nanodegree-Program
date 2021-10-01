#include "FusionEKF.h"
#include <iostream>
#include "Eigen/Dense"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::vector;

/**
 * Constructor.
 */
FusionEKF::FusionEKF() {
  is_initialized_ = false;

  previous_timestamp_ = 0;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  Hj_ = MatrixXd(3, 4);

  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
              0, 0.0225;

  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0,
              0, 0.0009, 0,
              0, 0, 0.09;

  /**
   * TODO: Finish initializing the FusionEKF.
   * TODO: Set the process and measurement noises
   */

  H_laser_  <<  1, 0, 0, 0,
                0, 1, 0, 0;

  //initialize P
  ekf_.P_ = MatrixXd(4, 4);
  ekf_.P_ <<  1000,0, 0, 0,
              0,1000, 0, 0,
              0,0,1000, 0,
              0,0,0,1000; 

  noise_ax_ = 9; //sigmax_ax^2
  noise_ay_ = 9; //sigmax_ay^2
}

/**
 * Destructor.
 */
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {
  /**
   * Initialization
   */
  if (!is_initialized_) {
    /**
     * TODO: Initialize the state ekf_.x_ with the first measurement.
     * TODO: Create the covariance matrix.
     * You'll need to convert radar from polar to cartesian coordinates.
     */

    // first measurement
    cout << "EKF: " << endl;
    ekf_.x_ = VectorXd(4);
    ekf_.x_ << 1, 1, 1, 1;

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      // TODO: Convert radar from polar to cartesian coordinates 
      //         and initialize state.

      // process radar measurement (main.cpp 81~91)
      float rho       = measurement_pack.raw_measurements_(0); 
      float phi       = measurement_pack.raw_measurements_(1);
      float delta_rho = measurement_pack.raw_measurements_(2);

      // change RADAR measurement to state space
      float px = rho * cos(phi);
      float py = rho * sin(phi);
      float vx = delta_rho * cos(phi);
      float vy = delta_rho * sin(phi);

      ekf_.x_ << px, py, vx, vy;
    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      // TODO: Initialize state.

      // process lidar measurement (main.cpp 71~79)
      float px       = measurement_pack.raw_measurements_(0); 
      float py       = measurement_pack.raw_measurements_(1);

      // lidar has no velocity info
      ekf_.x_ << px, py, 0.0, 0.0;

    }

    previous_timestamp_ = measurement_pack.timestamp_;

    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  /**
   * Prediction
   */

  /**
   * TODO: Update the state transition matrix F according to the new elapsed time.
   * Time is measured in seconds.
   * TODO: Update the process noise covariance matrix.
   * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
   */
  float dt       =  (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;
  float dt_2     = dt * dt;
  float dt_3     = dt_2 * dt;
  float dt_4     = dt_3 * dt;
  float dt_3div2 = dt_3 * 0.5;
  float dt_4div4 = dt_4 * 0.25;


  previous_timestamp_ = measurement_pack.timestamp_;

  // state transition matrix
  ekf_.F_ = MatrixXd(4, 4);
  ekf_.F_ << 1, 0, dt, 0,
             0, 1, 0, dt,
             0, 0, 1, 0,
             0, 0, 0, 1;

  // process covariance matrix
  ekf_.Q_ = MatrixXd(4, 4);
  ekf_.Q_ << dt_4div4 * noise_ax_, 0, dt_3div2 * noise_ax_, 0,
             0, dt_4div4 * noise_ay_, 0, dt_3div2 * noise_ay_,
             dt_3div2 * noise_ax_, 0, dt_2 * noise_ax_,     0,
             0, dt_3div2 * noise_ay_, 0,     dt_2 * noise_ay_;

  ekf_.Predict();

  /**
   * Update
   */

  /**
   * TODO:
   * - Use the sensor type to perform the update step.
   * - Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // TODO: Radar updates
    ekf_.R_ = R_radar_;
    ekf_.H_ = Hj_ = tools.CalculateJacobian(ekf_.x_); 
    ekf_.UpdateEKF(measurement_pack.raw_measurements_);

  } else {
    // TODO: Laser updates
    ekf_.R_ = R_laser_;
    ekf_.H_ = H_laser_;
    ekf_.Update(measurement_pack.raw_measurements_);
  }

  // print the output
  cout << "x_ = " << ekf_.x_ << endl;
  cout << "P_ = " << ekf_.P_ << endl;
}
