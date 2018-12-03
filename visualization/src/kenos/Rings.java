package kenos;

import processing.core.*;
import processing.data.*;

import static processing.core.PApplet.*;

class Rings {
  private PApplet parent;

  private int color;
  private float list[] = new float[8000];
  private int index = 0;

  Rings(PApplet parent, Table table, String column, int color) {
    // set parent
    this.parent = parent;

    // set parameters
    this.color = color;

    // parse data
    for (int i = 0; i < table.getRowCount(); i++) {
      float data = table.getFloat(i, column);
      // float dataMapped = map(data, 0, mapper, 100, 20); // this maps rain and sun proportionally
      // to the wind Data
      // float ampMapped = map(mapPropRainSun,0,3565,0,100);
      // amplitude = dataMapped;
      if (i < 8000) {
        if (column.equals("rainfall")) {
          data = 5 / data + 30;
        } else if (column.equals("wind")) {
          data = 140 / data + 30;
        } else {
          data = 40 / data + 30;
        }

        list[i] = data;
      }
    }
  }

  void paint() {
    parent.stroke(color);
    parent.strokeWeight(3);
    parent.noFill();

    if (index < 8000 - 1) {
      index++;
    }

    int index2 = index;
    parent.pushMatrix();
    parent.translate(parent.width / 2f, parent.height / 2f);
    parent.beginShape();

    for (float angle = 360; angle > 0; angle -= 0.8) {
      float theta = radians(angle);
      float x = cos(theta);
      float y = sin(theta);
      float r1 = list[index2] + parent.random(-20, 20);
      if (index < 8000 - 1) {
        index2++;
      }

      parent.vertex(x * r1, y * r1);
    }

    parent.endShape(parent.CLOSE);
    parent.popMatrix();
  }
}
