package kenos;

import processing.core.*;
import processing.data.*;

class Rings {
  private PApplet p;

  private int color;
  private float[] list;
  private int index = 0;

  Rings(PApplet parent, Table table, String column, int color) {
    // set p
    this.p = parent;

    // set parameters
    this.color = color;

    // create array
    list = new float[table.getRowCount()];

    // parse data
    for (int i = 0; i < table.getRowCount(); i++) {
      float data = table.getFloat(i, column);
      // float dataMapped = map(data, 0, mapper, 100, 20); // this maps rain and sun proportionally
      // to the wind Data
      // float ampMapped = map(mapPropRainSun,0,3565,0,100);
      // amplitude = dataMapped;
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

  void paint() {
    // set color
    p.stroke(color);
    p.strokeWeight(3);
    p.noFill();

    // begin shape
    p.beginShape();

    // copy index
    int index2 = index;

    // draw points
    for (float angle = 360; angle > 0; angle -= 0.8) {
      // calculate point
      PVector v = common.Helpers.pointOnCircle(angle, list[index2]);

      // add vertex
      p.vertex(v.x, v.y);

      // increment index
      index2++;
      if (index2 > list.length) {
        index2 = 0;
      }
    }

    // end shape
    p.endShape(p.CLOSE);

    // increment index
    index++;
    if (index > list.length) {
      index = 0;
    }
  }
}
