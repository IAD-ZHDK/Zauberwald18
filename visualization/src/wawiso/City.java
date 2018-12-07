package wawiso;

import processing.core.*;

import static processing.core.PApplet.*;
import static wawiso.Visualization.DOME_RADIUS;

class City {
  float magnetRadius;
  PVector pos;
  int size;
  private PApplet p;
  private PVector center_gravity;
  private PVector[] _p, acc, vel;
  private float topSpeed;
  private int num, nmax;

  City(PApplet parent, float _x, float _y, int size, int magnet) {
    p = parent;
    pos = new PVector(_x, _y);
    this.size = size;
    magnetRadius = magnet;
    reset();
  }

  void reset() {
    num = size;
    nmax = size + 10;
    topSpeed = num / 18.75f;
    center_gravity = new PVector(p.width / 2f, p.height / 2f);
    _p = new PVector[nmax];
    acc = new PVector[nmax];
    vel = new PVector[nmax];
    for (int i = 0; i < num; i++) {
      _p[i] =
          new PVector(
              p.width / 2f - ((DOME_RADIUS - 15) * cos(p.random(TWO_PI))),
              p.height / 2f - ((DOME_RADIUS - 15) * sin(p.random(TWO_PI))));
      float distance = dist(_p[i].x, _p[i].y, p.width / 2f, p.height / 2f);
      if (distance > DOME_RADIUS) {}
      acc[i] = new PVector(0, 0);
      vel[i] = new PVector(0, 0);
    }
  }

  void drawCity(float water, float solar, float wind) {
    center_gravity.x = pos.x;
    center_gravity.y = pos.y;
    update();
    display(water, solar, wind);
  }

  void update() {
    for (int i = 0; i < num; i++) {
      acc[i].x = center_gravity.x - _p[i].x;
      acc[i].y = center_gravity.y - _p[i].y;
      acc[i].normalize();
      acc[i].mult(0.65f);
      vel[i].add(acc[i]);
      vel[i].limit(topSpeed);
      _p[i].add(vel[i]);
      float distance = dist(_p[i].x, _p[i].y, p.width / 2f, p.height / 2f);
      if (distance > DOME_RADIUS) {
        float theta = atan2(_p[i].y - p.height / 2f, _p[i].x - p.width / 2f);
        _p[i].x = (p.width / 2f + (DOME_RADIUS * cos(theta + PI)));
        _p[i].y = (p.height / 2f + (DOME_RADIUS * sin(theta + PI)));
      }
    }
  }

  void display(float water, float solar, float wind) {
    // topSpeed = map(water, 0, 10, minspeed, minspeed*1.1);
    for (int i = 0; i < num - 1; i++) {
      if (i < num * 0.78) {
        if (water > 0) {
          p.fill(255 - water * 255, 255, 255);
        } else {
          p.fill(255);
        }
      } else if (i < num * (0.78 + 0.17)) {
        if (solar > 0) {
          p.fill(255, 255, 255 - solar * 255);
        } else {
          p.fill(255);
        }
      } else {
        if (wind > 0) {
          p.fill(255, 255 - wind * 255, 255);
        } else {
          p.fill(255);
        }
      }
      p.ellipse(_p[i].x, _p[i].y, 3, 3);
    }
  }
}
