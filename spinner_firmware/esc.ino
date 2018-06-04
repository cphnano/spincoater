#include <Servo.h>
#include "configuration.h"
#include "pins.h"

Servo ESC;

unsigned long last_esc_update;

long output_values[OUTPUT_SAMPLES];
long total_output;
long throttle;
unsigned int ind_output;

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

void init_esc() {
  for (int i = 0; i < OUTPUT_SAMPLES; i++) {
    output_values[i] = THROTTLE_STOP;
    total_output += THROTTLE_STOP;
  }
  last_esc_update = 0;
  total_output = 0;
  throttle = THROTTLE_STOP;
  ind_output = 0;
  ESC.attach(ESC_PIN);
}

void arm_esc() {
  unsigned long esc_start_time = millis();
  while (millis() - esc_start_time < 1000) {
    ESC.writeMicroseconds(1000);
    delay(20);
  }
}

void update_esc(int p) {
  if (micros() - last_esc_update > ESC_PERIOD) {
    throttle = update_output(p);
    last_esc_update = micros();
    ESC.writeMicroseconds(throttle);
  }
}

int update_output(int new_o) {
  total_output -= output_values[ind_output];
  total_output += (long)new_o;
  double d_throttle = ((double)total_output) / ((double)OUTPUT_SAMPLES);
  thrott = (int)d_throttle;
  output_values[ind_output] = (long)new_o;
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

void update_lut(int ind, int rpm, int throttle) {
  throttletable[ind][0] = rpm;
  throttletable[ind][1] = throttle;
}

