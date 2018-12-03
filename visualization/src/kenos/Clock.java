package kenos;

import java.util.ArrayList;

import processing.core.*;
import processing.data.*;

import static processing.core.PApplet.*;

class Clock {
  private PApplet p;

  private int fillColor;

  private ArrayList<PVector> points = new ArrayList<>();
  private float[] values;
  private int startCounter = 0;
  private int endCounter = 0;

  private static final int SEGMENTS = 180;

  Clock(PApplet parent, Table table, String column, int color, float max) {
    // save reference
    this.p = parent;

    // save color
    this.fillColor = color;

    // prepare angle
    float angle = 0;

    // create array
    values = new float[table.getRowCount()];

    // iterate through all entries
    for (int i = 0; i < table.getRowCount(); i++) {
      // get value
      float value = table.getFloat(i, column);

      // save value
      values[i] = value;

      // calculate position
      PVector v = common.Helpers.pointOnCircle(angle, map(value, 0, max, 0, parent.height / 2f));

      // advance angle
      angle = angle + 360f / SEGMENTS;

      // add point
      points.add(v);
    }
  }

  float get() {
    return this.values[this.endCounter];
  }

  void display() {
    // set color
    p.fill(fillColor);
    p.noStroke();

    // begin shape
    p.beginShape();

    // set first two points
    p.curveVertex(0, 0);
    p.curveVertex(0, 0);

    // add other points
    for (int i = startCounter; i < endCounter; i++) {
      p.curveVertex(points.get(i).x, points.get(i).y);
    }

    // finish shape
    p.endShape();

    // increase end
    endCounter++;

    // otherwise increase start if we did one rotation
    if (endCounter > SEGMENTS + 2) {
      startCounter++;
    }

    // check if end has been reached
    if (endCounter >= points.size()) {
      endCounter = 0;
      startCounter = 0;
    }
  }
}
