package kenos;

import processing.core.PApplet;
import processing.core.PVector;
import processing.data.Table;

import javax.script.ScriptEngineFactory;
import java.util.ArrayList;

import static processing.core.PApplet.*;

class FixedClock {
  private PApplet p;

  private int fillColor;

  private ArrayList<PVector> list = new ArrayList<>();
  private int counter = 0;

  private static final int SEGMENTS = 180;

  FixedClock(PApplet parent, int color) {
    // save reference
    this.p = parent;

    // save color
    this.fillColor = color;

    // fill array
    for (int i = 0; i < SEGMENTS; i++) {
      list.add(new PVector(0, 0));
    }
  }

  void set(float value) {
    // advance angle
    float angle = counter * 360f / SEGMENTS;

    // calculate position
    PVector v = common.Helpers.pointOnCircle(angle, map(value, 0, 1, 125, p.height / 2f));

    // set position
    list.set(counter, v);

    // increment counter
    counter++;
    if (counter >= list.size()) {
      counter = 0;
    }
  }

  void display() {
    // set color
    p.fill(fillColor);
    p.noStroke();

    // begin shape
    p.beginShape();

    // add other points
    for (int i = 0; i < SEGMENTS; i++) {
      p.curveVertex(list.get(i).x, list.get(i).y);
    }

    // finish shape
    p.endShape();
  }
}
