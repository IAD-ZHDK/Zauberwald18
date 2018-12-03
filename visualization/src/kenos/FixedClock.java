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

  private final static int SEGMENTS = 180;

  FixedClock(PApplet parent, int color) {
    // save reference
    this.p = parent;

    // save color
    this.fillColor = color;

    // fill array
    for (int i=0; i<SEGMENTS; i++) {
      list.add(new PVector(0, 0));
    }
  }

  void set(float value) {
    // advance angle
    float angle = counter * 360f / SEGMENTS;

    // calculate position
    float lineSize = map(value, 0, 1, 0, p.height / 2f);
    float x = cos(radians(angle)) * lineSize + p.width / 2f;
    float y = sin(radians(angle)) * lineSize + p.height / 2f;

    // set position
    list.set(counter, new PVector(x, y));

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
