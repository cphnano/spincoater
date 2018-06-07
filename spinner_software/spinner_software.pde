import controlP5.*;
import processing.serial.*;

int nl = 10; //newline in ASCII

String instr;

int BAUD_RATE = 115200;
int COM_PORT = 0;

Serial port;

boolean connected = false;

ControlP5 cp5;

PFont roboto;

float[] points_time = new float[0];
float[] points_rpm = new float[0];

float[] ramp_start_times = new float[0];
float[] ramp_times = new float[0];
float[] ramp_rpm = new float[0];
float process_time = 20.0; // Length of time of the spin process, in seconds. Logging stops after this amount of time.
float max_rpm = 5000.0;

long start_time = 0;
int process_step = 0;

boolean spin_started = false;

void setup() {
  size(800, 600);

  add_ramp(2.0, 3.0, 1500.0);
  add_ramp(7.0, 5.0, 3000.0);
  add_ramp(15.0, 2.0, 500.0);

  roboto = createFont("Roboto-Regular.ttf", 32);
  textFont(roboto);

  init_gui();

  println("COM PORTS: ");
  printArray(Serial.list());
  connected = connect(COM_PORT, BAUD_RATE);

  if (connected) {
    instr = port.readStringUntil(nl); // throw away the first reading to clear the buffer
    instr = null;
  }
}

void init_gui() {
  cp5 = new ControlP5(this);
  cp5.setAutoDraw(false);

  cp5.addButton("button_start")
    .setPosition(32, 32)
    .setSize(64, 32)
    .setColorForeground(color(130, 184, 58))
    .setColorBackground(color(65, 65, 65))
    .setColorActive(color(0, 255, 0))
    .setLabel("START");

  cp5.addButton("button_stop")
    .setPosition(128, 32)
    .setSize(64, 32)
    .setColorForeground(color(160, 0, 0))
    .setColorBackground(color(65, 65, 65))
    .setColorActive(color(255, 0, 0))
    .setLabel("STOP");
}

public void button_start() {
  if (!spin_started) {
    process_step = 0;
    points_time = new float[0];
    points_rpm = new float[0];
    spin_started = true;
    start_time = millis();
    write_cmd("STOP");
  }
}

public void button_stop() {
  if (spin_started) {
    write_cmd("STOP");
    spin_started = false;
  }
}

void update() {
  if (spin_started) {
    float delta_time = (millis()-start_time)/1000.0;
    if (delta_time > process_time) {
      button_stop();
    }

    if (process_step < ramp_start_times.length) {
      if (ramp_start_times[process_step] < delta_time) {
        write_cmd("RAMP "+str(ramp_times[process_step])+";"+str(ramp_rpm[process_step]));
        process_step++;
      }
    } else if (process_step == ramp_start_times.length) {
      if (ramp_start_times[process_step-1]+ramp_times[process_step-1] < delta_time) {
<<<<<<< HEAD
        write_cmd("RAMP 0.1;0.0"); // fast ramp down to zero
=======
        write_cmd("RAMP 0.1;0.0");
>>>>>>> 048105407b7d2948cdca3d280d90ba7a6c155470
        process_step++;
      }
    }

    if (connected) {
      while (port.available() > 0) {
        instr = null;
        instr = port.readStringUntil(nl);
        if (instr != null) {
          instr = trim(instr);
          String[] parts = split(instr, " ");
          float t = delta_time;
          float rpm = float(parts[0]);
          add_point(t, rpm);
        }
      }
    }
  }
}

void render() {
  background(225);

  cp5.draw();

  // draw plot
  noStroke();
  fill(255);
  rect(100, 200, 600, 300);
  stroke(0);
  line(100, 500, 700, 500);
  line(100, 500, 100, 200);
  fill(0);
  textSize(12);
  textAlign(CENTER, TOP);
  text("0", 100, 508);
  text(str(process_time), 700, 508);
  text("Time [s]", 400, 508);
  textAlign(RIGHT, CENTER);
  text("0", 92, 500);
  text(str(max_rpm), 92, 200);
  text("Speed [RPM]", 92, 350);

  // plot desired speed profile
  float t1 = 0.0;
  float r1 = 0.0;
  stroke(130, 184, 58);
  for (int i = 0; i < ramp_start_times.length; i++) {
    float x1 = map(t1, 0.0, process_time, 100.0, 700.0);
    float x2 = map(ramp_start_times[i], 0.0, process_time, 100.0, 700.0);
    float x3 = map(ramp_start_times[i]+ramp_times[i], 0.0, process_time, 100.0, 700.0);
    float y1 = map(r1, 0.0, max_rpm, 500.0, 200.0);
    float y3 = map(ramp_rpm[i], 0.0, max_rpm, 500.0, 200.0);
    line(x1, y1, x2, y1);
    line(x2, y1, x3, y3);
    t1 = ramp_start_times[i]+ramp_times[i];
    r1 = ramp_rpm[i];

    if (i == ramp_start_times.length-1) {
      line(x3, y3, x3, 500.0);
    }
  }

  // plot real-time speed
  stroke(81, 77, 148);
  for (int i = 1; i < points_time.length; i++) {
    float x1 = map(points_time[i-1], 0.0, process_time, 100.0, 700.0);
    float x2 = map(points_time[i], 0.0, process_time, 100.0, 700.0);
    float y1 = map(points_rpm[i-1], 0.0, max_rpm, 500.0, 200.0);
    float y2 = map(points_rpm[i], 0.0, max_rpm, 500.0, 200.0);
    line(x1, y1, x2, y2);
  }

  if (spin_started) {
    stroke(65);
    float x = map(millis()-start_time, 0.0, process_time*1000, 100.0, 700.0);
    line(x, 200, x, 500);
  }
}

void draw() {
  update();
  render();
}

void add_point(float t, float rpm) {
  points_time = append(points_time, t);
  points_rpm = append(points_rpm, rpm);
}

void add_ramp(float start_time, float ramp_time, float rpm) {
  ramp_start_times = append(ramp_start_times, start_time);
  ramp_times = append(ramp_times, ramp_time);
  ramp_rpm = append(ramp_rpm, rpm);
}

boolean connect(int com, int baud) {
  if (Serial.list().length <= com) {
    return false;
  } else {
    try {
      port = new Serial(this, Serial.list()[com], baud);
      port.clear();
    } 
    catch (Exception e) {
      e.printStackTrace();
      println("Could not open port: "+com);
      return false;
    }
  }
  return true;
}

void write_cmd(String cmd) {
  println("Sent: "+cmd);
  if (connected) {
    port.write(cmd+"\n");
  }
}

void keyPressed() {
  if (key==27) { // Prevent ESC key from closing the program
    key=0;
  }
}
