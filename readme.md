Spincoater
====
Repository for cphnano's DIY spincoater, for spincoating substrates up to 6".
This repository consists of 3 parts: 
 - spinner_firmware/
   
   This folder contains the arduino-based firmware for controlling the spinner.
   
 - spinner_software/
  
   This folder contains a simple client software for starting, stopping, monitoring and logging spin processes.
 
 - spinner_hardware/
 
   This folder contains CAD files for building the spin coater. 

Communication protocol
-----
The spincoater can be started, stopped and configured via a USB serial connection. 
It uses a simple serial communication protocol in plain ASCII text at a baudrate of 115200 
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


FAQ
-----
 - Q: How do I use the spinner software?
 
   A: Download [processing](https://processing.org/) and then open spinner_software/spinner_software.pde

 - Q: The spinner software won't connect! What am I doing wrong? 
 
   A: Make sure that you are connected to the right COM port and using the correct baudrate. This can be set on lines 8 and 9 in spinner_software.pde. Ensure that the arduino is connected and is running the correct firmware.
