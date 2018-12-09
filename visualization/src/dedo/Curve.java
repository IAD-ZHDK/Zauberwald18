package dedo;

import processing.core.*;
import processing.data.*;

import java.util.ArrayList;

import static processing.core.PApplet.*;

// TODO: Cleanup.

class Curve {
  int A;
  private PApplet p;
  private int C;
  private float colorChange = 255;
  private int offSet = 160;
  private ArrayList<Segment> sList;

  private float speed = 1;
  private int m = 6;
  private int[] months = new int[m];
  private int totalMonth;
  private int end = 0;

  private float Off = 0;
  private int speedOff = 4;
  private float maxOff = 0;
  private String id;

  Curve(PApplet parent, TableRow r, float max, int c, int a, String st) {
    p = parent;

    id = st;
    sList = new ArrayList<>();

    for (int i = 0; i < r.getColumnCount() - 1; i++) {
      float ymin = map(r.getFloat(i), 0, max, 0, p.height / 2f);
      float xmin =
          map(
              i + 1,
              0,
              r.getColumnCount(),
              (p.width / 2) - (p.height / 2) - offSet,
              (p.width / 2f) + (p.height / 2f));
      float ymax = map(r.getFloat(i + 1), 0, max, 0, p.height / 2f);
      float xmax =
          map(
              i + 2,
              0,
              r.getColumnCount(),
              (p.width / 2) - (p.height / 2) - offSet,
              (p.width / 2f) + (p.height / 2f));
      sList.add(new Segment(p, xmin, xmax, ymin, ymax));
    }

    C = c;
    A = a;

    totalMonth = r.getColumnCount() - 1;

    for (int i = m - 1; i >= 0; i--) {
      months[i] = i;
      sList.get(i).minX =
          map(i, 0, m, (p.width / 2) - (p.height / 2) - offSet, (p.width / 2f) + (p.height / 2f));
      sList.get(i).maxX =
          map(
              i + 1,
              0,
              m,
              (p.width / 2) - (p.height / 2) - offSet,
              (p.width / 2f) + (p.height / 2f));
    }
  }

  void display(float solar, float wind, float water) {
    p.fill(C);
    p.noStroke();

    float lastmax = 0;

    for (int j = 0; j < months.length; j++) {
      int i = months[j];
      // if(j>0){sList.get(i).minY=lastmax;}
      float scale = map(sList.get(i).minX, (p.width / 2f) - (p.height / 2f), p.width / 2f, 0, 1);
      if (sList.get(i).minX - 100 > p.width / 2) {
        scale = map(sList.get(i).minX, p.width / 2f, (p.width / 2f) + (p.height / 2f), 1, 0);
      }
      float ymin = sList.get(i).minY * sin(radians(A)) * scale + p.height / 2f;

      float scale2 = map(sList.get(i).maxX, (p.width / 2f) - (p.height / 2f), p.width / 2f, 0, 1);
      if (sList.get(i).maxX > p.width / 2) {
        scale2 = map(sList.get(i).maxX, p.width / 2f, (p.width / 2f) + (p.height / 2f), 1, 0);
      }
      float ymax = sList.get(i).maxY * sin(radians(A)) * scale2 + p.height / 2f;
      // if(i%2==0){Off=-1*Off;}
      if (j == 0) {
        sList.get(i).display(ymin, ymax, Off, (int) maxOff / 12);
      } else {
        sList.get(i).display(lastmax, ymax, Off, (int) maxOff / 12);
      }
      sList.get(i).minX += speed;
      sList.get(i).maxX += speed;
      lastmax = ymax;
    }

    rot();
    off();
    if (sList.get(months[m - 1]).minX > (p.width / 2) + (p.height / 2)) {
      next();
    }

    if (id == "solar") {
      maxOff = solar;
      colorChange = (int) map(solar, 0, 130, 255, 0);
      C = p.color(255, 255, colorChange, 100);
    }
    if (id == "wind") {
      maxOff = wind;
      colorChange = (int) map(wind, 0, 130, 255, 0);
      C = p.color(255, colorChange, 255, 99);
    }
    if (id == "wasser") {
      maxOff = water;
      colorChange = (int) map(water, 0, 130, 255, 0);
      C = p.color(colorChange, 255, 255, 50);
    }
  }

  private void off() {
    if (Off >= maxOff) {
      speedOff = (int) p.random(-3, -5);
    }

    if (Off <= (-1) * maxOff) {
      speedOff = (int) p.random(3, 5);
    }

    // if(maxOff>0){ Off+=speedOff;}
    if (maxOff <= 0) {
      Off = 0;
    } else {
      Off += speedOff;
    }
  }

  private void next() {
    if (end == totalMonth - 1) {
      end = 0;
    } else {
      end++;
    }

    int next = end;
    for (int i = m - 1; i >= 0; i--) {
      months[i] = next;
      // sList.get(next).minX=map(i,0,m,(width/2)-(height/2),(width/2)+(height/2));
      sList.get(next).minX =
          map(i, 0, m, (p.width / 2) - (p.height / 2) - offSet, (p.width / 2) + (p.height / 2));
      sList.get(next).maxX =
          map(i + 1, 0, m, (p.width / 2) - (p.height / 2) - offSet, (p.width / 2) + (p.height / 2));
      if (next == totalMonth - 1) {
        next = 0;
      } else {
        next++;
      }
    }
  }

  private void rot() {
    if (A < 360) {
      A++;
    } else {
      A = 0;
    }
  }
}
