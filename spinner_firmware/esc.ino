#include <Servo.h>
#include "configuration.h"
#include "pins.h"

Servo ESC;

unsigned long last_esc_update;

long output_values[OUTPUT_SAMPLES];
long total_output;
long throttle_out;
unsigned int ind_output;

int throttletable[NUM_THROTTLE][2] = { //{RPM, throttle [ms]}
  {746, 1080},
  {1115, 1097},
  {1609, 1114},
  {2093, 1131},
  {2594, 1148},
  {3111, 1165},
  {3617, 1182},
  {4157, 1199}
};

void init_esc() {
  total_output = 0;
  for (int i = 0; i < OUTPUT_SAMPLES; i++) {
    output_values[i] = THROTTLE_NEUTRAL;
    total_output += THROTTLE_NEUTRAL;
  }
  last_esc_update = 0;
  throttle_out = THROTTLE_NEUTRAL;
  ind_output = 0;
  ESC.attach(ESC_PIN);
}

void setup_esc() {
  send_signal(5000, THROTTLE_NEUTRAL);
  send_signal(2000, THROTTLE_MAX);
  send_signal(2000, THROTTLE_MIN);
}

void emulate_transmitter(unsigned long pulse_length, unsigned long throttle) {
  unsigned long esc_start_time = millis();
  while (millis() - esc_start_time < pulse_length) {
    ESC.writeMicroseconds(throttle);
    delay(20);
  }
}

void arm_esc() {
  emulate_transmitte(2000, THROTTLE_NEUTRAL);
}

void update_esc(int p) {
  if (micros() - last_esc_update > ESC_PERIOD) {
    throttle_out = update_output(p);
    last_esc_update = micros();
    ESC.writeMicroseconds(throttle_out);
  }
}

long update_output(long new_o) {
  total_output -= output_values[ind_output];
  total_output += new_o;
  long thrott = (total_output) / ((double)OUTPUT_SAMPLES);
  output_values[ind_output] = new_o;
  ind_output++;
  if (ind_output >= OUTPUT_SAMPLES) {
    ind_output = 0;
  }

  return thrott;
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

void update_lut(int ind, int rpm, int thrott) {
  throttletable[ind][0] = rpm;
  throttletable[ind][1] = thrott;
}

long get_throttle() {
  return throttle_out;
}

