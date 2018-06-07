/* CONFIGURATION */
#define ESC_PERIOD         2000L // µs
#define BAUD_RATE        115200
#define RPM_SAMPLES          12 // size of the running average for RPM measurement
#define TACHO_THRES         470 
#define TACHO_HYST          150
#define OUTPUT_SAMPLES        1 // size of the running average for the output throttle

#define READOUT_PERIOD       50 // ms
#define THROTTLE_STOP      1000 // throttle value for stopping the spinner
#define THROTTLE_MIN       1080 //minimum throttle value where the spinner rotates
#define THROTTLE_MAX       1200 // maximum throttle value
#define NUM_THROTTLE          8 // number of throttle values in the LUT. More = more accurate feed-forward, but slower
#define D_SMOOTHING           8 // size of running average for the D term. This eliminates jitter
#define CALIB_TIME        10000 // ms, time spent on each step when doing calibration. This should be long enough for the RPM to settle on a stable value
#define STEADY_THRES          0.08 //8%. When the current RPM is within 8% of the setpoint, the state is switched from RAMP to  STEADY STATE
#define FEEDFORWARD_GAIN      1.0 // gain for the feed-forward term. This can be tuned to use less feed-forward and more PID.

#define DEFAULT_RAMP_TIME  5000
#define DEFAULT_HOLD_TIME 10000
#define DEFAULT_RPM_GOAL   3000

/***PID parameters***/
//Steady  state
#define Kp_SS 51.27 * 0.001
#define Ki_SS 35.89 * 0.001
#define Kd_SS 0.0
//Ramp
#define Kp_RAMP 85.0 * 0.001
#define Ki_RAMP 0.0
#define Kd_RAMP 0.0


