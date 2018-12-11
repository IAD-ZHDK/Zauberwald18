package common;

import processing.core.*;

import static processing.core.PApplet.*;

public class Helpers {
  public static PVector pointOnCircle(float angle, float distance) {
    float r = radians(angle);
    float x = cos(r);
    float y = sin(r);
    PVector p = new PVector(x, y, 0);
    return p.mult(distance);
  }

  public static float angleDiff(float a1, float a2) {
    float phi = abs(a1 - a2) % 360;
    return phi > 180 ? 360 - phi : phi;
  }
}
