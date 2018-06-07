#include "configuration.h"

void interpret_command(String inputstr) {
  String cmd = "";
  String param = "";
  int ind = inputstr.indexOf(' ');
  if(ind != -1){
    cmd = inputstr.substring(0, ind);
    param = inputstr.substring(ind+1);
  } else {
    cmd = inputstr;
  }
  
  if (cmd == "RAMP") { //Example: RAMP <ramp up time>;<desired RPM>
    int semicolon_ind = param.indexOf(';');
    if (semicolon_ind != -1){
      String time_str = param.substring(0, semicolon_ind);
      String rpm_str = param.substring(semicolon_ind+1);
      boolean is_float = true;
      is_float = (is_float && string_is_float(time_str));
      is_float = (is_float && string_is_float(rpm_str));
      if(is_float){
        set_ramp_time(time_str.toFloat());
        set_rpm_goal(rpm_str.toFloat());
        start_ramp();
      }
    }
  } else if (cmd == "STOP") {
    set_state(0);
  }
}

boolean string_is_int(String str) { // checks if string is a positive integer (including 0)
  int n = str.length();
  for (int i = 0; i < n; i++) {
    if (!isDigit(str[i])) {
      return false;
    }
  }

  return true;
}

boolean string_is_float(String str) { // checks if string is a floating point number
  int n = str.length();
  for (int i = 0; i < n; i++) {
    if (!(isDigit(str[i]) || str[i] == '-' || str[i] == '.')) {
      return false;
    }
  }

  return true;
}
