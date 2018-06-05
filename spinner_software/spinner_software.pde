import processing.serial.*;

int nl = 10; //newline in ASCII

String instr;

int BAUD_RATE = 115200;
int COM_PORT = 0;

Serial port;

void setup(){
  size(800, 600);
  noStroke();
  
  println("COM PORTS: ");
  printArray(Serial.list());
  connect(COM_PORT, BAUD_RATE);
  instr = port.readStringUntil(nl); // throw away the first reading to clear the buffer
  instr = null;
}

void draw(){
  background(225);
  fill(130, 184, 58);
  rect(50, 50, 300, 500);
  fill(160, 0, 0);
  rect(450, 50, 300, 500);
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

void mousePressed(){
  if(mouseX < 400){
    println("START");
    port.write("START\n");
  } else {
    println("STOP");
    port.write("STOP\n");
  }
}
