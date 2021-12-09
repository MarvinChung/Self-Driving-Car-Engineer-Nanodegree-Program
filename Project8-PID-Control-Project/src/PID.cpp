#include "PID.h"
#include <float.h>
#include <iostream>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_, bool use_twiddle) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  best_err = DBL_MAX;
  p_error = 0;
  d_error = 0;
  i_error = 0;
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_; 

  dp[0] = 1;
  dp[1] = 1;
  dp[2] = 1;

  use_twiddle = use_twiddle;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  d_error = cte - p_error;
  p_error = cte; 
  i_error += cte;

}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  double error = -Kp * p_error - Ki * i_error - Kd * d_error;
  printf("p_error:%lf, i_error:%lf, d_error:%lf\n", p_error, i_error, d_error);
  return error;  // TODO: Add your total error calc here!
}

// void PID::twiddle() {
//   double steer_value = pid.TotalError();
//   double best_err = DBL_MAX;
//   while(dp[0] + dp[1] + dp[2] > 1e-5)
//   {
//     double p[3] = {Kp, Ki, Kd};
//     for(int i = 0; i < 3; i++)
//     {
//       p[i] += dp[i];
//       double err = pid.TotalError();
//       if (err < best_err)
//       {
//         best_err = err;
//       }
//       else{
//         p[i] -= 2*dp[i];
//         double err = pid.TotalError();
//         if (err < best_err)
//         {
//           best_err = err;
//           dp[i] *= 1.1;
//         }
//         else{
//           dp[i] *= 0.9;
//         }
//       }
//     }
//   }
// }