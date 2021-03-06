#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;
// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void NormalizeAngle(double& phi)
{
  phi = atan2(sin(phi), cos(phi));
}


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
  TODO:
    * predict the state
  */
   x_ = F_ * x_;
   MatrixXd Ft = F_.transpose();
   P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */

   VectorXd y;
   y = z - H_*x_;
   
   MatrixXd S;
   S = H_*P_*H_.transpose() +R_;
   
   MatrixXd K;
   K = P_*H_.transpose()*S.inverse();
   
   //new estimate
   x_ += (K * y);
   long x_size = x_.size();
   MatrixXd I = MatrixXd::Identity(x_size, x_size);
   P_ -= K * H_ * P_;
   
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
   
   float rho = sqrt(x_(0)*x_(0)+x_(1)*x_(1));
   float theta = atan2(x_(1),x_(0));//my error: atan -pi/2~pi/2; atan2 -pi~pi
   float rho_dot = (x_(0)*x_(2)+x_(1)*x_(3))/rho ;
   VectorXd z_pred =  VectorXd(3);  
   z_pred<< rho, theta, rho_dot;
  
   VectorXd y;
   y = z - z_pred;
   NormalizeAngle(y(1));//my error : ensure Radian Measure
  
   
   MatrixXd S;
   S = H_*P_*H_.transpose() +R_;
   
   MatrixXd K;
   K = P_*H_.transpose()*S.inverse();
   
   //new estimate
   x_ += (K * y);
   long x_size = x_.size();
   MatrixXd I = MatrixXd::Identity(x_size, x_size);
   P_ -= K * H_ * P_;
}
