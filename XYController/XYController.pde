import netP5.*;
import oscP5.*;

OscP5 osc;
NetAddress hostLocation;
final String HOST_IP = "127.0.0.1";
final int HOST_PORT = 50000;

PVector position = new PVector();
PVector destination = new PVector();
final float easing = 0.1;
final int DIAMITER = 20;

Metro metro;
final float INTERVAL = 100.0;

void setup() {
  size(480, 480);
  background(0);
  position.set(width * 0.5, height * 0.5);
  destination.set(width * 0.5, height* 0.5);

  osc = new OscP5(this, 56789);
  hostLocation = new NetAddress(HOST_IP, HOST_PORT);

  metro = new Metro(INTERVAL);
}

void drawGrid() {
  stroke(128);
  strokeWeight(2);
  line(0, height * 0.5, width, height * 0.5);
  line(width * 0.5, 0, width * 0.5, height);
}

void sendOSC() {
  OscMessage msg = new OscMessage("/position");

  float x = position.x / width * 2.0  - 1.0;  
  float y = -1.0 * position.y / height * 2.0 + 1.0; 

  msg.add(x);
  msg.add(y);
  osc.send(msg, hostLocation);
}

void updatePosition() {
  if (mousePressed) {
    destination.set(mouseX, mouseY);
  }

  position.x += (destination.x - position.x) * easing;
  position.y += (destination.y - position.y) * easing;
}

void draw() {
  background(0);

  drawGrid();
  updatePosition();
  
  noStroke();
  ellipse(position.x, position.y, DIAMITER, DIAMITER);

  if (metro.bang()) {
    sendOSC();
  }
}


void mouseReleased() {
  destination.set(width*0.5, height * 0.5);
}
