package radschlag;

import processing.core.*;
import processing.data.*;

import static common.Helpers.*;
import static processing.core.PApplet.*;

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

    // get max
    float maxData = 0;
    for (int i = 0; i < table.getRowCount(); i++) {
      maxData = max(maxData, table.getFloat(i, column));
    }

    // parse data
    for (int i = 0; i < table.getRowCount(); i++) {
      float data = table.getFloat(i, column);
      data = map(data, 0, maxData, 0, (p.height/6f) - 30) + 15;
      list[i] = data;
    }
  }

  float efficiency() {
    return map(list[index]-15, 0, (p.height/6f) - 30, 0, 1);
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
      PVector v = pointOnCircle(angle, (p.height/6f) - list[index2]);

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
