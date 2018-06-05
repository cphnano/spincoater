#include "configuration.h"

void interpret_command(String inputstr) {
  if (inputstr == "START") {
    set_state(4);
  } else if (inputstr == "STOP") {
    set_state(0);
  }
}

