#ifndef __COMP417_PID_H__
#define __COMP417_PID_H__


class PID {
 public:
  PID(double Kp, double Td, double Ti, double dt) {
    // todo: write this
    this->Kp = Kp;
    this->Td = Td;
    this->Ti = Ti;
    this->dt = dt;
  };
  
  void update_control(double current_error) {
      // integral to previous current integral time inteval
      sum_error += current_error*dt;
      // derivative calculation
      current_deriv_error = (current_error - previous_error)/dt;
      // PID control calcualtion
      control = Kp * (current_error + Td * current_deriv_error + (1 / Ti) * sum_error);
      previous_deriv_error = current_deriv_error;
      previous_error = current_error;
  };

  double get_control() {
    return control;
  };

 private:
  double Kp;
  double Td;
  double Ti;

  double current_error;
  double previous_error;

  double sum_error;
  
  double current_deriv_error;
  double previous_deriv_error;
  double control;
  double dt;
};

#endif
