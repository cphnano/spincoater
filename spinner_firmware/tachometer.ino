#include "configuration.h"
#include "pins.h"

int tacho_state;
int tacho_an;
boolean tacho_triggered;
unsigned long tacho_last_trig;

double rpm[RPM_SAMPLES];
double total_rpm;
double current_rpm;
unsigned int rpm_ind;

void init_tacho(){
  tacho_state = LOW;
  tacho_an = 0;
  tacho_triggered = false;
  tacho_last_trig = 0;

  for (int i = 0; i < RPM_SAMPLES; i++) {
    rpm[i] = 0;
  }
  
  total_rpm = 0;
  current_rpm = 0;
  rpm_ind = 0;
}

void update_tacho() {
  tacho_an = analogRead(TACHO_PIN);
  if (tacho_triggered) {
    if (tacho_an < TACHO_THRES - TACHO_HYST) {
      if (tacho_state) {
        tacho_state = LOW;
        unsigned long delta = micros() - tacho_last_trig;
        tacho_last_trig = micros();
        double r = (30 * 1000000.0) / delta;
        update_rpm(r);
      }
    } else if (tacho_an > TACHO_THRES + TACHO_HYST) {
      if (!tacho_state) {
        tacho_state = HIGH;
        unsigned long delta = micros() - tacho_last_trig;
        tacho_last_trig = micros();
        double r = (30 * 1000000.0) / delta;
        update_rpm(r);
      }
    }
  } else {
    if (tacho_an < TACHO_THRES - TACHO_HYST) {
      if (tacho_state) {
        tacho_state = LOW;
        tacho_last_trig = micros();
        tacho_triggered = true;
      }
    } else if (tacho_an > TACHO_THRES + TACHO_HYST) {
      if (!tacho_state) {
        tacho_state = HIGH;
        tacho_last_trig = micros();
        tacho_triggered = true;
      }
    }
  }
  if (micros() - tacho_last_trig > 32000) {
    update_rpm(0);
    tacho_last_trig = micros();
    tacho_triggered = false;
  }
}

void update_rpm(double r) {
  total_rpm -= rpm[rpm_ind];
  total_rpm += r;
  current_rpm = total_rpm / RPM_SAMPLES;
  rpm[rpm_ind] = r;
  rpm_ind++;
  if (rpm_ind >= RPM_SAMPLES) {
    rpm_ind = 0;
  }
}

double get_rpm(){
  return current_rpm;
}


