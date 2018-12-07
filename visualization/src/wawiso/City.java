package wawiso;

import processing.core.*;

import static processing.core.PApplet.*;
import static wawiso.Visualization.DOME_RADIUS;

class City {
  PVector pos;
  int size;
  private PApplet p;
  private PVector center_gravity;
  private PVector[] _p, acc, vel;
  private float topSpeed;
  private int num;

  City(PApplet parent, float _x, float _y, int size) {
    p = parent;
    pos = new PVector(_x, _y);
    this.size = size;
    reset();
  }

  void reset() {
    num = size;
    int nMax = size + 10;
    topSpeed = num / 18.75f;
    center_gravity = new PVector(p.width / 2f, p.height / 2f);
    _p = new PVector[nMax];
    acc = new PVector[nMax];
    vel = new PVector[nMax];
    for (int i = 0; i < num; i++) {
      _p[i] =
          new PVector(
              p.width / 2f - ((DOME_RADIUS - 15) * cos(p.random(TWO_PI))),
              p.height / 2f - ((DOME_RADIUS - 15) * sin(p.random(TWO_PI))));

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

  private void update() {
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

  private void display(float water, float solar, float wind) {
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
