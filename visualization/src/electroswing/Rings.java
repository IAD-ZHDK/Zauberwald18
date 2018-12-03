package electroswing;

import processing.core.PApplet;

import static processing.core.PApplet.*;

class Rings {
  private PApplet p;

  private float minRadius;
  private int color;
  private float seed;

  int count = 0;

  final static float MIN_AMPLITUDE = 0.2f;
  private final static float RING_DISTANCE = 5f;

  Rings(PApplet parent, int color, float minRadius) {
    p = parent;

    this.color = color;
    this.minRadius = minRadius;
    this.seed = p.random(0, 1000);
  }

  void add(float max) {
    if (count > max) {
      return;
    }

    count++;
  }

  void remove() {
    count--;

    if (count < 0) {
      count = 0;
    }
  }

  void paint(float changer) {
    p.stroke(color);
    p.strokeWeight(1.5f);
    p.noFill();

    for (int i = 0; i < count; i++) {
      paintRing(i, changer);
    }
  }

  private void paintRing(int i, float changer) {
    float amplitude = map(changer, 0, 1, MIN_AMPLITUDE, 0.45f);

    float radius = (this.minRadius + (i * RING_DISTANCE)) / 2;

    float variance = amplitude * radius;
    float min = radius - variance;
    float max = radius + variance;

    p.beginShape();

    for (float angle = 0; angle < 360; angle += 0.8) {
      float theta = radians(angle);
      float x = cos(theta);
      float y = sin(theta);
      float r1 = p.noise(theta, (p.frameCount + seed) * 0.06f);
      r1 = map(r1, 0, 1, min, max);
      p.vertex(x * r1, y * r1);
    }

    p.endShape(CLOSE);
  }
}
