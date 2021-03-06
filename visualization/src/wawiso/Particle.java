package wawiso;

import processing.core.*;

import static processing.core.PApplet.*;
import static wawiso.Visualization.DOME_RADIUS;

class Particle {
  private static final float NOISE_MODULATION = 10;
  private static final float NOISE_VARIATION = 20;
  private static final float NOISE_INTERVAL = 5000;
  private static final float NOISE_RESISTANCE = 100;
  int colorE;
  PVector pos;
  Boolean start = false;
  int index;
  private PApplet p;
  private PVector vel;
  private PVector acc;
  private float size;
  private float maxForce;
  private float maxSpeed;

  Particle(
      PApplet parent,
      float _x,
      float _y,
      float _maxSpeed,
      float _maxForce,
      int _index,
      int colorE) {
    p = parent;

    this.colorE = colorE;
    pos = new PVector(_x, _y);
    vel = new PVector(0, 0);
    acc = new PVector(0, 0);
    size = 3;
    maxForce = _maxForce;
    maxSpeed = _maxSpeed;
    index = _index;
  }

  private float getNoiseAngle(float _x, float _y) {
    return map(
        p.noise(
            _x / NOISE_MODULATION + NOISE_VARIATION,
            _y / NOISE_MODULATION + NOISE_VARIATION,
            p.frameCount / NOISE_INTERVAL + NOISE_VARIATION),
        0,
        1,
        0,
        TWO_PI * 2);
  }

  void flow() {
    float noiseAngle = getNoiseAngle(pos.x, pos.y);
    PVector desired =
        new PVector(cos(noiseAngle) * NOISE_RESISTANCE, sin(noiseAngle) * NOISE_RESISTANCE);
    desired.mult(maxSpeed);
    PVector steer = PVector.sub(desired, vel);
    steer.limit(maxForce);
    applyForce(steer);
  }

  private void applyForce(PVector _force) {
    acc.add(_force);
  }

  private void update() {
    vel.add(acc.x, acc.y);
    vel.limit(maxSpeed);
    pos.add(vel);
    acc.mult(0);
  }

  private void display() {
    p.fill(colorE);
    p.noStroke();
    p.ellipse(pos.x, pos.y, size, size);
  }

  void position() {
    pos.x = p.width / 2f - ((DOME_RADIUS - 15) * cos(p.random(TWO_PI)));
    pos.y = p.height / 2f - ((DOME_RADIUS - 15) * sin(p.random(TWO_PI)));

    float distance = dist(pos.x, pos.y, p.width / 2f, p.height / 2f);
    if (distance > DOME_RADIUS) position();
  }

  private void borders() {
    float distance = dist(pos.x, pos.y, p.width / 2f, p.height / 2f);
    if (distance > DOME_RADIUS) {
      float theta = atan2(pos.y - p.height / 2f, pos.x - p.width / 2f);
      pos.x = (p.width / 2f + (DOME_RADIUS * cos(theta + PI)));
      pos.y = (p.height / 2f + (DOME_RADIUS * sin(theta + PI)));
    }
  }

  void draw() {
    update();
    borders();
    display();
  }
}
