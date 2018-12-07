package blitzkraftwerk;

import processing.core.PApplet;

class ArcLight {
  float negativeIncrement;
  private PApplet p;
  private float diameter;
  private float startPoint;
  private float endPoint;
  private float end;
  private float positiveIncrement;
  private boolean direction;
  private boolean trigger;

  ArcLight(
      PApplet parent,
      float diameter,
      float startPoint,
      float endPoint,
      float positiveIncrement,
      float negativeIncrement) {
    this.p = parent;
    this.diameter = diameter;
    this.startPoint = startPoint;
    this.endPoint = endPoint;
    this.positiveIncrement = positiveIncrement;
    this.negativeIncrement = negativeIncrement;
    direction = false;
    trigger = false;
    end = startPoint + 0.001f;
  }

  void display() {
    p.arc(p.width / 2f, p.height / 2f, diameter, diameter, startPoint, end);
    if (end < startPoint) {
      direction = false;
    }
    if (end > endPoint) {
      direction = true;
      positiveIncrement = 0;
    }
    if (!direction) {
      end += positiveIncrement;
    }
    if (direction) {
      end -= negativeIncrement;
    }
  }

  boolean reset() {
    if (end > startPoint) {
      return false;
    } else if (!trigger) {
      trigger = true;
      return true;
    } else {
      return false;
    }
  }
}
