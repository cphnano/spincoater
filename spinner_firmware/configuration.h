/* CONFIGURATION */
#define ESC_PERIOD         2000L // µs
#define BAUD_RATE        115200
#define RPM_SAMPLES          12 // size of the running average for RPM measurement
#define TACHO_THRES         470 
#define TACHO_HYST          150
#define OUTPUT_SAMPLES        1 // size of the running average for the output throttle

#define READOUT_PERIOD       50 // ms
#define THROTTLE_NEUTRAL   1500 // throttle value for stopping the spinner
#define THROTTLE_MIN       1530 // minimum throttle value where the spinner rotates
#define THROTTLE_MAX       2000 // maximum throttle value
#define THROTTLE_REVERSE   1000 // maximum reverse/brake
#define NUM_THROTTLE          8 // number of throttle values in the LUT. More = more accurate feed-forward, but slower
#define D_SMOOTHING           8 // size of running average for the D term. This eliminates jitter
#define CALIB_TIME        10000 // ms, time spent on each step when doing calibration. This should be long enough for the RPM to settle on a stable value
#define STEADY_THRES          0.08 //8%. When the current RPM is within 8% of the setpoint, the state is switched from RAMP to  STEADY STATE
#define FEEDFORWARD_GAIN      1.0 // gain for the feed-forward term. This can be tuned to use less feed-forward and more PID.
#define SAMPLE_TIME       1000 // µs, sample time for control loop

#define DEFAULT_RAMP_TIME  5000
#define DEFAULT_HOLD_TIME 10000
#define DEFAULT_RPM_GOAL   3000

/***PID parameters***/
//Steady  state
#define DEFAULT_HOLD_KP 51.27 * 0.001
#define DEFAULT_HOLD_KI 35.89 * 0.001
#define DEFAULT_HOLD_KD 0.0
#define DEFAULT_HOLD_KF 1.0
//Ramp
#define DEFAULT_RAMP_KP 85.0 * 0.001
#define DEFAULT_RAMP_KI 0.0
#define DEFAULT_RAMP_KD 0.0
#define DEFAULT_RAMP_KF 1.0
