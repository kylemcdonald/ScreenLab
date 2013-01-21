import oscP5.*;
import netP5.*;
import processing.net.*; 
import promidi.*;

float minDistance = .5, maxDistance = 3.;
float minRange = .2, maxRange = 2;
boolean enableSsr = true;
boolean enableMidi = true;

boolean randomizePositions = false;

OscP5 osc;
NetAddress v4;
Client ssrOut;
MidiIO midiIO;
MidiOut midiOut;

String v4Ip = "146.87.67.41";
String ssrIp = "localhost";
int ssrPort = 4711;
int oscPort = 8001;
int v4Port = 4096;
int channels = 4;
float cameraX, cameraY;
float[] x = new float[channels];
float[] y = new float[channels];

void setup() {
  size(800, 800);
  // frameRate(30);

  if (enableSsr) {
    ssrOut = new Client(this, ssrIp, ssrPort);
  }
  osc = new OscP5(this, oscPort);
  midiIO = MidiIO.getInstance(this);
  midiIO.printPorts();
  midiIO.openInput(0);
  midiOut = midiIO.openOutput(0);
  v4 = new NetAddress(v4Ip, v4Port);
}

// receive midi from ableton, send to v4
void noteOn(Note note) {
  int channel = note.getMidiChannel();
  if (randomizePositions) {
    float radius = random(minRange, maxRange);
    float theta = random(-PI, PI);
    x[channel] = sin(theta) * radius;
    y[channel] = cos(theta) * radius;
    updateChannel(channel);
  }
  OscMessage msg = new OscMessage("/ping");
  msg.add(channel);
  osc.send(msg, v4);
}

void updateChannel(int channel) {
  if (enableSsr && ssrOut.available() > 0) {
    float jx = x[channel], jy = y[channel];
    float theta = PI;
    float xNorm = jx * cos(theta) + jy * sin(theta);
    float yNorm = jy * cos(theta) + jx * sin(theta);
    ssrOut.write("<request><source id=\"" + (channel+1) + "\"><position x=\""+xNorm+"\" y=\""+yNorm+"\"/></source></request>\0");
  }
  if (enableMidi && channel < 4) {
    float distance = map(dist(cameraX, cameraY, x[channel], y[channel]), minDistance, maxDistance, 0, 127);
    distance = constrain(distance, 0, 127);
    midiOut.sendController(new Controller(0, channel, int(distance)));
    //midiOut.sendController(new Controller(0, channel, int(map(sin(millis() / 1000.), -1, 1, 0, 127))));
  }
}

// receive osc from v4, send to ableton
void oscEvent(OscMessage msg) {
  if(msg.checkAddrPattern("/volume")) {
    try {
      midiOut.sendController(new Controller(0, 64, int(127 * msg.get(0).floatValue()))); // noise level
      midiOut.sendController(new Controller(0, 65, int(127 * msg.get(1).floatValue()))); // sine level
      midiOut.sendController(new Controller(0, 66, int(127 * msg.get(2).floatValue()))); // sine level
      midiOut.sendController(new Controller(0, 67, int(127 * msg.get(3).floatValue()))); // square level
    } catch (Exception e) {
      println("Poorly formatted message: " + msg);
    }
  }
  if (!randomizePositions && msg.checkAddrPattern("/move")) {
    try {
      int j = 0;
      cameraX = msg.get(j++).floatValue();
      cameraY = msg.get(j++).floatValue();
      for (int i = 0; i < channels; i++) {
        x[i] = msg.get(j++).floatValue();
        y[i] = msg.get(j++).floatValue();
        updateChannel(i);
      }
    } 
    catch (Exception e) {
      println("Poorly formatted message: " + msg);
    }
  }
}

void draw() {
  background(0); 
  translate(width/2, height/2);

  ellipseMode(CENTER);
  textAlign(CENTER);  
  textSize(12);

  float zoom = 100;
  
  stroke(255);
  noFill();
  rect(cameraX * zoom, cameraY * zoom, 32, 32);
  text("cam", cameraX * zoom + 10, cameraY * zoom + 10);

  for (int i = 0; i < channels; i++) {
    float cx = x[i] * zoom, cy = y[i] * zoom;
    noFill();
    stroke(255);
    ellipse(cx, cy, 32, 32);
    noStroke();
    fill(255);
    pushMatrix();
    float theta = map(i, 0, channels, 0, TWO_PI);
    translate(cos(theta) * 10, sin(theta) * 10);
    text(i, cx, cy + 5);
    popMatrix();
  }
} 

