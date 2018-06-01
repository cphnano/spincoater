#include <Servo.h>
#include "configuration.h"
#include "pins.h"

Servo ESC;

unsigned long last_reading = 0;
unsigned long last_esc_update = 0;
unsigned long tacho_last_trig = 0;

unsigned int ind = 0;
long output_values[OUTPUT_SAMPLES];
long total_output = 0;
long throttle = THROTTLE_STOP;
unsigned int ind_output = 0;

/***PID parameters***/
//Steady  state
double kp_ss = 51.27 * 0.001;
double ki_ss = 35.89 * 0.001;
double kd_ss = 0.0;
//Ramp
double kp_r = 85.0 * 0.001;
double ki_r = 0.0;
double kd_r = 0.0;

int state = 0; //0 = idle, 1 = ramp, 2 = spinning, 3 = calibration
int calib_step;
unsigned long state_start_time;

// spin cycle configuration. Linear ramp from 0 to rpm_goal over ramp_time number of ms, then hold for hold_time and then linear ramp from rpm_goal to 0 over ramp_time number of ms
long ramp_time = 2000;
long hold_time = 10000;
float rpm_goal = 3000;

int throttletable[NUM_THROTTLE][2] = { //{RPM, throttle [ms]}
  {0, 1080},
  {1295, 1104},
  {1939, 1128},
  {2627, 1152},
  {3296, 1176},
  {4024, 1200},
  {4714, 1224},
  {5382, 1248},
  {6055, 1272},
  {6755, 1296}
};



void setup() {
  for (int i = 0; i < OUTPUT_SAMPLES; i++) {
    output_values[i] = THROTTLE_STOP;
    total_output += THROTTLE_STOP;
  }

  Serial.begin(BAUD_RATE);

  ESC.attach(ESC_PIN);
  while (millis() < 1000) {
    ESC.writeMicroseconds(1000);
    delay(20);
  }

  //kp_r = (a / 1024.0f) * 200 * 0.001;
  if (false) {
    Serial.println("Ramp: ");
    Serial.print("Kp: ");
    Serial.print(kp_r * 1000);
    Serial.print(", Ki: ");
    Serial.print(ki_r * 1000);
    Serial.print(", Kd: ");
    Serial.println(kd_r * 1000);
    Serial.println("Steady state: ");
    Serial.print("Kp: ");
    Serial.print(kp_ss * 1000);
    Serial.print(", Ki: ");
    Serial.print(ki_ss * 1000);
    Serial.print(", Kd: ");
    Serial.println(kd_ss * 1000);
  }

  init_pid();
  init_tacho();
  set_sample_time(1000);
  set_pid_parameters(kp_r, ki_r, kd_r);

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
        throttletable[calib_step][0] = get_rpm();
        throttletable[calib_step][1] = t;
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
    if (delta < ramp_time + hold_time){
      set_setpoint(rpm_goal);
    } else if (delta < ramp_time + ramp_time + hold_time){
      double sp = map(delta - (ramp_time + hold_time), 0, ramp_time, rpm_goal, 0);
      if (sp < 0){
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
      set_pid_parameters(kp_ss, ki_ss, kd_ss);
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

  /*int a = analogRead(0);
    if (a > 100) {
    setpoint = map(a, 100, 1023, RPM_MIN, RPM_MAX);
    } else {
    setpoint = 0;
    }*/



  //throttle test
  /*int step_n = round((millis() - time_start) / 2000);
    int throttle = step_n * (THROTTLE_MAX - THROTTLE_MIN) / 9 + THROTTLE_MIN;
    throttle = min(throttle, THROTTLE_MAX);
    update_esc(throttle);*/

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

int update_output(int new_o) {
  total_output -= output_values[ind_output];
  total_output += (long)new_o;
  double d_throttle = ((double)total_output) / ((double)OUTPUT_SAMPLES);
  throttle = (int)d_throttle;
  output_values[ind_output] = (long)new_o;
  ind_output++;
  if (ind_output >= OUTPUT_SAMPLES) {
    ind_output = 0;
  }

  return throttle;
}

void update_esc(int p) {
  if (micros() - last_esc_update > ESC_PERIOD) {
    update_output(p);
    last_esc_update = micros();
    ESC.writeMicroseconds(throttle);
  }
}

void start_calibration() {
  state = 3;
  calib_step = 0;
  state_start_time = millis();
}

void print_calibration_table() {
  for (int i = 0; i < NUM_THROTTLE; i++) {
    int r = throttletable[i][0];
    int t = throttletable[i][1];
    Serial.print("{");
    Serial.print(r);
    Serial.print(", ");
    Serial.print(t);
    Serial.println("},");
  }
}

