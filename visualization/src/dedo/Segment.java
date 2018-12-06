package dedo;

import processing.core.PApplet;

import static processing.core.PApplet.*;

class Segment {
  private PApplet p;

  float minX;
  float maxX;
  float minY;
  float maxY;

  private final static float WIDTH = 15;
  private final static int NUM = 4;

  Segment(PApplet parent, float minX, float maxX, float minY, float maxY) {
    p = parent;
    this.minX = minX;
    this.maxX = maxX;
    this.minY = minY;
    this.maxY = maxY;
  }

  void display(float yMin, float yMax, float off, int num) {
    if (num < NUM) {
      num = NUM;
    }

    float xd = abs(maxX - minX) / NUM;
    float yd = abs((yMax - yMin)) / NUM;

    if (yMin > yMax) {
      yd = yd * -1;
    }

    for (int i = 0; i < num; i++) {
      if (minX + (xd * i) > (p.width / 2) - (p.height / 2)
          && minX + (xd * i) < (p.width / 2) + (p.height / 2)) {
        p.beginShape();
        p.vertex(minX + (xd * i), p.height / 2f);
        p.vertex(minX + (xd * i) + off, yMin + (i * yd));
        p.vertex(minX + WIDTH + (xd * i) + off, yMin + (i * yd));
        p.vertex(minX + WIDTH + (xd * i), p.height / 2f);
        p.endShape();
      }
    }
  }
}
