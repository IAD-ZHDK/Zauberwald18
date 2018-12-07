package wawiso;

import processing.core.*;

import static processing.core.PApplet.*;
import static wawiso.Visualization.DOME_RADIUS;

class Particle {
  private static final float NOISE_MODULATION = 10;
  private static final float NOISE_VARIATION = 20;
  private static final float NOISE_INTERVAL = 5000;
  private static final float NOISE_RESISTANCE = 100;
  private static final int MAX_TREMBLE_TIME = 200;
  private static final float RIPPLE_RADIUS = 12;
  private PApplet p;
  private Boolean inverted = false;
  int colorE;
  PVector pos;
  private PVector vel;
  private PVector acc;
  private float size;
  private float maxForce;
  private float maxSpeed;
  private float angle;
  private float angleIncrement;
  Boolean flowing = true;
  Boolean repelling = false;
  Boolean attracting = false;
  Boolean stopped = false;
  Boolean rippling = false;
  Boolean trembling = false;
  Boolean start = false;
  float ripplingSize = 0;
  float distanceFromCenter = 0;
  private int trembleTime = 0;
  int index;

  Particle(
      PApplet parent,
      float _x,
      float _y,
      float _maxspeed,
      float _maxforce,
      int _index,
      int colorE) {
    p = parent;

    this.colorE = colorE;
    pos = new PVector(_x, _y);
    vel = new PVector(0, 0);
    acc = new PVector(0, 0);
    size = 3;
    angle = 0;
    angleIncrement = p.random(0.005f, 0.1f);
    maxForce = _maxforce;
    maxSpeed = _maxspeed;
    index = _index;
  }

  /**
   * Calculates the noise angle in a given position
   *
   * @param _x Current position on the x axis
   * @param _y Current position on the y axis
   * @return Float Noise angle
   */
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

  /** Sets acceleration to follow the noise flow */
  void flow() {
    float noiseAngle = getNoiseAngle(pos.x, pos.y);
    PVector desired =
        new PVector(cos(noiseAngle) * NOISE_RESISTANCE, sin(noiseAngle) * NOISE_RESISTANCE);
    desired.mult(maxSpeed);
    PVector steer = PVector.sub(desired, vel);
    steer.limit(maxForce);
    applyForce(steer);
  }

  /**
   * Rotates the particle from a given radius
   *
   * @param _radius Distance from the center to rotate from
   */
  void circleRotation(float _radius) {
    angle += angleIncrement;
    if (angle >= TWO_PI) angle = 0;
    PVector target =
        new PVector(p.width / 2f + (_radius * cos(angle)), p.height / 2f + (_radius * sin(angle)));
    PVector desired = PVector.sub(target, pos);
    follow(desired);
  }

  /** Sets a random velocity in three intervals to simulate a trembling effect */
  void tremble() {
    flowing = false;
    PVector variation = new PVector(p.random(-1, 1), p.random(-1, 1));
    vel.add(variation);
    if (trembleTime > MAX_TREMBLE_TIME / 3) vel.add(variation);
    if (trembleTime > 2 * (MAX_TREMBLE_TIME / 3)) vel.add(variation);

    trembleTime++;
    if (trembleTime > MAX_TREMBLE_TIME) {
      trembleTime = 0;
      trembling = false;
      flowing = true;
    }
  }

  /** Maps the particle size according to the distance between its position and the ripple radius */
  void ripple() {
    float distance = abs(dist(pos.x, pos.y, p.width / 2f, p.height / 2f) - RIPPLE_RADIUS);

    if (distance < 50) {
      rippling = true;
      ripplingSize = map(distance, 50, 0, 0, 7);
      repelling = true;
      magnet(p.width / 2f, p.height / 2f, RIPPLE_RADIUS + 25, 10);
    } else {
      rippling = false;
      repelling = false;
    }
  }

  /**
   * Sets the particle acceleration to follow a desired direction
   *
   * @param _desired Vector to follow
   */
  private void follow(PVector _desired) {
    _desired.normalize();
    _desired.mult(maxSpeed);
    PVector steer = PVector.sub(_desired, vel);
    steer.limit(maxForce);
    applyForce(steer);
  }

  /** Particle falls to the closest border of the dome to end the sequence */
  void fall() {
    flowing = false;
    repelling = true;
    attracting = false;
    inverted = false;

    magnet(p.width / 2f, p.height / 2f, DOME_RADIUS, 1500);
  }

  /**
   * Repels or attracts the particles within a distance
   *
   * @param _x X coordinate of the magnet center
   * @param _y Y coordinate of the magnet center
   * @param _radius Distance affected by the magnet effect
   * @param _strength Strength of the magnet force
   */
  void magnet(float _x, float _y, float _radius, float _strength) {
    float magnetRadius = _radius;
    PVector target = new PVector(_x, _y);
    PVector force = PVector.sub(target, pos);
    float distance = force.mag();

    if (distance < magnetRadius) {
      flowing = false;
      distance = constrain(distance, 1.0f, 25.0f);
      force.normalize();
      float strength = 0.00f;
      if (repelling) strength = (500 + _strength) / (distance * distance * -1);
      if (attracting) strength = (50 + _strength) / (distance * distance);
      force.mult(strength);
      applyForce(force);
    } else {
      flowing = true;
    }
  }

  /**
   * Adds a vector to the current acceleration
   *
   * @param _force Vector to add
   */
  private void applyForce(PVector _force) {
    acc.add(_force);
  }

  /** Updates the acceleration, velocity and position vectors */
  private void update() {
    if (!stopped) {
      if (inverted) acc.mult(-1);
      vel.add(acc.x, acc.y);
      vel.limit(maxSpeed);
      pos.add(vel);
      acc.mult(0);
    }
  }

  private void display() {
    p.fill(colorE);
    p.noStroke();
    p.ellipse(pos.x, pos.y, size + ripplingSize, size + ripplingSize);
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
      /* --------Warp particles to a random location-------- */
      // position();

      /* --------Warp particles to opposite side-------- */
      float theta = atan2(pos.y - p.height / 2f, pos.x - p.width / 2f);
      pos.x = (p.width / 2f + (DOME_RADIUS * cos(theta + PI)));
      pos.y = (p.height / 2f + (DOME_RADIUS * sin(theta + PI)));
    }
  }

  void run() {
    update();
    borders();
    display();
  }
}
