#include "configuration.h"
#include "pins.h"

unsigned long last_reading = 0;

int state; // -1 = calibration, 0 = do nothing, 1 = ramp up, 2 = hold speed, 3 = ramp down (unused), 4 = spin slowly (for checking alignment)
int calib_step;
unsigned long state_start_time;

// spin cycle configuration. Linear ramp from 0 to rpm_goal over ramp_time number of ms, then hold for hold_time and then linear ramp from rpm_goal to 0 over ramp_time number of ms
long ramp_time = DEFAULT_RAMP_TIME;
long hold_time = DEFAULT_HOLD_TIME;
float rpm_goal = DEFAULT_RPM_GOAL;
float ramp_from = 0;

long throttle;

String instr;

void setup() {
  instr = "";
  Serial.begin(BAUD_RATE);
  throttle = THROTTLE_STOP;

  init_esc();

  init_pid();
  init_tacho();
  set_sample_time(1000);
  set_pid_parameters(Kp_RAMP, Ki_RAMP, Kd_RAMP);

  arm_esc();
  delay(1000);
}

void loop() {
  // read new commands
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      interpret_command(instr);
      instr = "";
    } else {
      instr = instr + c;
    }
  }

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
    case 4:
      throttle = THROTTLE_MIN;
      break;
    default:
      throttle = THROTTLE_STOP;
      break;
  }

  update_esc(throttle);

  if (millis() - last_reading > READOUT_PERIOD && state != 0) {
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
    } else if (state == 1) {
      set_pid_parameters(Kp_RAMP, Ki_RAMP, Kd_RAMP);
    } else if (state == 2) {
      set_pid_parameters(Kp_SS, Ki_SS, Kd_SS);
    }
    state_start_time = millis();
  }
}

void spin_steadystate() {
  long delta = millis() - state_start_time;
  throttle = compute_throttle(rpm_goal);
}

void spin_ramp_up() {
  long delta = millis() - state_start_time;
  double sp = rpm_goal;
  if (delta < ramp_time) {
    sp = map(delta, 0, ramp_time, ramp_from, rpm_goal);
  } else {
    sp = rpm_goal;
    set_state(2);
  }

  /*double err = rpm_goal - get_rpm();
  if (err < rpm_goal * STEADY_THRES) {
    //switch to steady state
    sp = rpm_goal;
    set_state(2);
  }*/

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

void set_ramp_time(float new_ramp_time) {
  ramp_time = round(new_ramp_time * 1000);
}

void set_rpm_goal(float new_rpm_goal) {
  rpm_goal = new_rpm_goal;
}

void start_ramp() {
  ramp_from = get_setpoint();
  set_state(1);
}


