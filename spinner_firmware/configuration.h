#define ESC_PERIOD        2000L
#define BAUD_RATE       115200
#define RPM_SAMPLES         36
#define TACHO_THRES        470
#define TACHO_HYST         150
#define OUTPUT_SAMPLES       1

#define READOUT_PERIOD      50
#define THROTTLE_STOP     1000
#define THROTTLE_MIN      1080
#define THROTTLE_MAX      1300
#define RPM_MIN           1900
#define RPM_MAX          11640
#define NUM_THROTTLE        10
#define D_SMOOTHING          8
#define CALIB_TIME       10000
#define STEADY_THRES         0.08 //8%. When the current RPM is within 8% of the setpoint, the state is switched from RAMP to  STEADY STATE
#define FEEDFORWARD_GAIN     1.0
