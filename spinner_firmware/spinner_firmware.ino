#include "configuration.h"
#include "pins.h"

unsigned long last_reading = 0;

int state; //0 = idle, 1 = ramp up, 2 = hold speed, 3 = ramp down, -1 = calibration
int calib_step;
unsigned long state_start_time;

// spin cycle configuration. Linear ramp from 0 to rpm_goal over ramp_time number of ms, then hold for hold_time and then linear ramp from rpm_goal to 0 over ramp_time number of ms
long ramp_time = 10000;
long hold_time = 10000;
float rpm_goal = 3000;

long throttle;

void setup() {
  Serial.begin(BAUD_RATE);
  throttle = THROTTLE_STOP;

  init_esc();

  init_pid();
  init_tacho();
  set_sample_time(1000);
  set_pid_parameters(Kp_RAMP, Ki_RAMP, Kd_RAMP);

  arm_esc();
  delay(1000);

  set_state(1);
}

void loop() {
  update_tacho();

  switch (state) {
    case -1:
      run_calibration();
      break;
    case 1:
      spin_ramp_up();
      break;
    case 2:
      spin_steadystate();
      break;
    case 3:
      spin_ramp_down();
      break;
    default:
      throttle = THROTTLE_STOP;
  }

  update_esc(throttle);

  if (millis() - last_reading > READOUT_PERIOD) {
    Serial.print(get_rpm());
    Serial.print(" ");
    Serial.print(get_setpoint());
    Serial.print(" ");
    Serial.println(get_throttle());
    last_reading = millis();
  }
}

void set_state(int new_state) {
  if (state != new_state) {
    state = new_state;

    if (state == -1) {
      calib_step = 0;
    }
    state_start_time = millis();
  }
}

void spin_steadystate() {
  long delta = millis() - state_start_time;
  throttle = compute_throttle(rpm_goal);

  if (delta > hold_time) {
    set_state(3);
  }
}

void spin_ramp_up() {
  long delta = millis() - state_start_time;
  double sp = rpm_goal;
  if (delta < ramp_time) {
    sp = map(delta, 0, ramp_time, 0, rpm_goal);
  } else {
    sp = rpm_goal;
    set_state(2);
  }

  double err = rpm_goal - get_rpm();
  if (err < rpm_goal * STEADY_THRES) {
    //switch to steady state
    set_pid_parameters(Kp_SS, Ki_SS, Kd_SS);
  }

  throttle = compute_throttle(sp);
}

void spin_ramp_down() {
  long delta = millis() - state_start_time;
  double sp = 0;
  if (delta < ramp_time) {
    sp = map(delta, 0, ramp_time, rpm_goal, 0);
    if (sp < 0) {
      sp = 0;
    }
  } else {
    set_state(0);
  }
  throttle = compute_throttle(sp);
}

void run_calibration() {
  //calibration
  long delta = millis() - state_start_time;
  if (delta > CALIB_TIME) {
    update_lut(calib_step, get_rpm(), throttle);
    calib_step++;
    state_start_time = millis();
  }

  if (calib_step < NUM_THROTTLE) {
    throttle = THROTTLE_MIN;
    throttle += (long)(calib_step * ((THROTTLE_MAX - THROTTLE_MIN) / (NUM_THROTTLE - 1)));
  } else {
    throttle = THROTTLE_STOP;
    print_calibration_table();
    set_state(0);
  }
}

// change set-point, compute PID and update the ESC
long compute_throttle(double sp) {
  set_setpoint(sp);
  set_input(get_rpm());
  long t = round(compute_pid());
  return t;
}


