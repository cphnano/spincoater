#include "configuration.h"
#include "pins.h"

unsigned long last_reading = 0;

int state = 0; //0 = idle, 1 = ramp, 2 = spinning, 3 = calibration
int calib_step;
unsigned long state_start_time;

// spin cycle configuration. Linear ramp from 0 to rpm_goal over ramp_time number of ms, then hold for hold_time and then linear ramp from rpm_goal to 0 over ramp_time number of ms
long ramp_time = 2000;
long hold_time = 10000;
float rpm_goal = 3000;

void setup() {
  Serial.begin(BAUD_RATE);

  init_esc();
  arm_esc();

  //print_parameters();

  init_pid();
  init_tacho();
  set_sample_time(1000);
  set_pid_parameters(Kp_RAMP, Ki_RAMP, Kd_RAMP);

  state_start_time = millis();
  state = 1;
  //start_calibration();
}

void loop() {
  update_tacho();

  if (state == 3) {
    //calibration
    if (calib_step < NUM_THROTTLE) {
      long t = THROTTLE_MIN;
      t += (long)(calib_step * ((THROTTLE_MAX - THROTTLE_MIN) / (NUM_THROTTLE - 1)));
      int next_step = (int)floor((millis() - state_start_time) / CALIB_TIME);
      if (next_step > calib_step) {
        update_lut(calib_step, get_rpm(), t);
        calib_step = next_step;
      }
      update_esc(t);
    } else {
      state = 0;
      state_start_time = millis();
      print_calibration_table();
    }
  } else if (state == 2) {
    long delta = millis() - state_start_time;
    if (delta < ramp_time + hold_time) {
      set_setpoint(rpm_goal);
    } else if (delta < ramp_time + ramp_time + hold_time) {
      double sp = map(delta - (ramp_time + hold_time), 0, ramp_time, rpm_goal, 0);
      if (sp < 0) {
        sp = 0;
      }
      set_setpoint(sp);
    } else {
      set_setpoint(0);
    }
    set_input(get_rpm());
    int o = round(compute_pid());
    update_esc(o);
  }
  else if (state == 1) {
    long delta = millis() - state_start_time;
    if (delta < ramp_time) {
      double sp = map(delta, 0, ramp_time, 0, rpm_goal);
      set_setpoint(sp);
    } else {
      set_setpoint(rpm_goal);
    }

    double sp = rpm_goal;
    double err = sp - get_rpm();
    if (err < sp * STEADY_THRES) {
      //switch to steady state
      set_pid_parameters(Kp_SS, Ki_SS, Kd_SS);
      state_start_time = millis();
      set_setpoint(rpm_goal);
      state = 2;
    }

    set_input(get_rpm());
    int o = round(compute_pid());
    update_esc(o);
  } else {
    set_setpoint(0);
  }

  if (millis() - last_reading > READOUT_PERIOD) {
    Serial.print(get_rpm());
    Serial.print(" ");
    Serial.print(get_setpoint());
    Serial.print(" ");
    Serial.println(throttle);
    /*float t = millis()/1000.0f;
      Serial.print(current_rpm);
      Serial.print(", ");
      Serial.println(t);*/
    last_reading = millis();
  }
}

void start_calibration() {
  state = 3;
  calib_step = 0;
  state_start_time = millis();
}


