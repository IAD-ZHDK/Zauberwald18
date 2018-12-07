package blitzkraftwerk;

import processing.core.PApplet;

class ArcSun {
  private PApplet p;

  private float diameter;
  private float startPoint;
  private float endPoint;
  private float end;
  private float positiveIncrement;
  private float negativeIncrement;
  private boolean direction;
  private boolean trigger;

  ArcSun(
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
    end = startPoint;
  }

  void display(boolean anim) {
    p.arc(p.width / 2f, p.height / 2f, diameter, diameter, startPoint, end);
    if (end < startPoint) {
      direction = false;
    }
    if (end > endPoint) {
      direction = true;
    }
    if (!direction && anim) {
      end += positiveIncrement;
    }
    if (direction) {
      end -= negativeIncrement;
    }
    if (!anim && end > startPoint) {
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
