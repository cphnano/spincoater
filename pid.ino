#include "configuration.h"

unsigned long last_pid_update;
unsigned long ts;
double Kp, Ki, Kd;
double input, output, setpoint;
double lastErr, lastInput;
double ITerm;
boolean enable_feedforward;

double smoothed_input[D_SMOOTHING];
double total_input;
double current_smooth_input;
double previous_smooth_input;
unsigned int ind_smooth;

double feedforward_gain = 1.0;

void init_pid() {
  for (int i = 0; i < D_SMOOTHING; i++) {
    smoothed_input[i] = 0;
  }
  total_input = 0;
  current_smooth_input = 0;
  previous_smooth_input = 0;
  ind_smooth = 0;

  last_pid_update = micros();
  ts = 5000; //5ms
  Kp = Ki = Kd = 0;
  input = output = setpoint = 0;
  lastErr = 0;
  lastInput = 0;
  ITerm = 0;
  enable_feedforward = true;
}

double compute_pid() {
  unsigned long now = micros();
  double delta_t = (double)(now - last_pid_update);

  if (delta_t >= ts) {
    double error = setpoint - input;
    ITerm += Ki * error;

    output = Kp * error;
    if (enable_feedforward) {
      output += get_feedforward(setpoint*feedforward_gain);
    }

    update_d_input(input);
    double dInput = (current_smooth_input - previous_smooth_input);
    output += Kd * dInput;

    if(setpoint == 0){
      ITerm = 0;
    } else {
      double IMax = THROTTLE_MAX - output;
      double IMin = THROTTLE_MIN - output;
      if(ITerm > IMax){
        ITerm = IMax;
      } else if(ITerm < IMin){
        ITerm = IMin;
      }
    }

    output += ITerm;

    //limit output
    if (setpoint > 0) {
      if (output > THROTTLE_MAX) {
        output = THROTTLE_MAX;
      } else if (output < THROTTLE_MIN) {
        output = THROTTLE_MIN;
      }
    } else {
      output = THROTTLE_STOP;
    }

    lastErr = error;
    lastInput = input;
    last_pid_update = now;
  }

  return output;
}

void set_input(double inp) {
  input = inp;
}

void set_setpoint(double s) {
  setpoint = s;
}

double get_setpoint() {
  return setpoint;
}

double get_output() {
  return output;
}

void set_pid_parameters(double p, double i, double d) {
  double sample_time = ts / 1000000.0;
  Kp = p;
  Ki = i * sample_time;
  Kd = d / sample_time;
}

void set_sample_time(unsigned long t) {
  ts = t;
  set_pid_parameters(Kp, Ki, Kd);
}

void update_d_input(double inp) {
  previous_smooth_input = current_smooth_input;
  total_input -= smoothed_input[ind_smooth];
  total_input += inp;
  current_smooth_input = total_input / D_SMOOTHING;
  smoothed_input[ind_smooth] = inp;
  ind_smooth++;
  if (ind_smooth >= D_SMOOTHING) {
    ind_smooth = 0;
  }
}

double get_feedforward(double r) {
  if (r == 0) {
    return (double)THROTTLE_STOP;
  }

  if (r <= throttletable[0][0]) {
    return (double)throttletable[0][1];
  } else if (r >= throttletable[NUM_THROTTLE - 1][0]) {
    return (double)throttletable[NUM_THROTTLE - 1][1];
  }

  int r_low, r_high;
  int t_low, t_high;

  for (int i = 1; i < NUM_THROTTLE; i++) {
    if (throttletable[i][0] > r) {
      r_low = throttletable[i - 1][0];
      r_high = throttletable[i][0];
      t_low = throttletable[i - 1][1];
      t_high = throttletable[i][1];
      break;
    }
  }

  double t = (r - r_low);
  t *= (t_high - t_low) * 1.0f;
  t /= ((r_high - r_low) * 1.0f);
  t += t_low;
  return t;
}

