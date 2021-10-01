#include "kalman_filter.h"
#include <iostream>
using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
   * TODO: predict the state
   */
  // Assume noise is zero (x = Fx + 0)
  x_ = F_ * x_;
  P_ = F_ * P_ * F_.transpose() + Q_;

}

void KalmanFilter::MeasurementUpdate(const VectorXd& y) {
  // Share function for Update and UpdateEKF

  MatrixXd H_t =  H_.transpose();
  MatrixXd S   = H_ * P_ * H_t + R_;
  MatrixXd K   = P_ * H_t * S.inverse();
  x_           = x_ + K * y;
  MatrixXd I = MatrixXd::Identity(x_.size(), x_.size());
  P_ = (I - (K * H_)) * P_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
   * TODO: update the state by using Kalman Filter equations
   */
  
  VectorXd z_pred = H_ * x_;
  VectorXd y = z - z_pred;

  MeasurementUpdate(y);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
   * TODO: update the state by using Extended Kalman Filter equations
   */
  // H_j only updates S, K, P. Still use h(x) here.

  float px = x_(0);
  float py = x_(1);
  float vx = x_(2);
  float vy = x_(3);

  double rho       = sqrt(px * px + py * py);
  double phi       = atan2(py, px);
  double delta_rho = (px * vx + py * vy) / std::max(rho, 0.0001);

  VectorXd z_pred(3);
  z_pred << rho, phi, delta_rho;

  VectorXd y = z - z_pred;

  while (y(1) > M_PI)  y(1) -= 2 * M_PI;
  while (y(1) < -M_PI) y(1) += 2 * M_PI;

  MeasurementUpdate(y);
}
