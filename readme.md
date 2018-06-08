Spincoater
====
Repository for cphnano's DIY spincoater, for spincoating 6" wafers.

Communication protocol
-----
The spincoater can be started, stopped and configured via a USB serial connection. 
It uses a simple  communication protocal in plain ASCII text at a baudrate of 115200 
(by default, this can be changed in configuration.h). Each command must be ended by a newline character (\n or ASCII value 10).


| Function                         | Command                          | Example                       | 
|:---------------------------------|:--------------------------------:|:-----------------------------:|
| Start ramp up/down               | RAMP \<time>;\<RPM>              | RAMP 3.5;4000                 |
| Stop the spinner                 | STOP                             | STOP                          |
| Calibrate the ESC/motor          | START_CALIBRATION                | START_CALIBRATION             |
| Get calibration table            | GET_CALIBRATION                  | GET_CALIBRATION               |
| Get all control parameters (ramp and hold)      | GET_PID                          | GET_PID                       |
| Sets control parameters for ramp | SET_PID_RAMP \<P>;\<I>;\<D>;\<F> | SET_PID_RAMP 85.0;0.0;0.0;1.0 |
| Sets control parameters for hold | SET_PID_HOLD \<P>;\<I>;\<D>;\<F> | SET_PID_HOLD 85.0;0.0;0.0;1.0 |
| Resets the settings saved in EEPROM to default values (PID and feedforward profiles). Use GET_PID to get the new values. Spinner must be calibrated after an EEPROM reset! | RESET_EEPROM | RESET_EEPROM |



 

