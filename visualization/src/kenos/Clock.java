package kenos;

import java.util.ArrayList;

import processing.core.*;
import processing.data.*;

import static processing.core.PApplet.*;

class Clock {
  private PApplet p;

  private int fillColor;

  private ArrayList<PVector> list;
  private int counter = 0;
  private int counterArray = 0;

  Clock(PApplet parent, Table table, String column, int color, float mapper) {
    this.p = parent;

    float angle = 0;
    float angleSpeed = 2.2f;

    list = new ArrayList<>();
    for (int i = 0; i < table.getRowCount(); i++) {
      float data = table.getFloat(i, column);
      float lineSize = map(data, 0, mapper, 0, parent.height / 2f);
      float x = cos(radians(angle)) * lineSize + parent.width / 2f;
      float y = sin(radians(angle)) * lineSize + parent.height / 2f;
      angle = angle + angleSpeed;
      list.add(new PVector(x, y));
    }
    this.fillColor = color;
  }

  void display() {
    p.beginShape();
    p.noStroke();
    p.curveVertex(p.width / 2f, p.height / 2f);
    p.curveVertex(p.width / 2f, p.height / 2f);

    for (int i = counterArray; i < counter; i++) {
      // stroke(255);
      p.fill(fillColor);
      p.curveVertex(list.get(i).x, list.get(i).y);
    }

    p.endShape();

    if (counter > 165 && counter < list.size() - 1) {
      counterArray++;
    }

    if (counter < list.size()) {
      counter++;
    }
  }
}
