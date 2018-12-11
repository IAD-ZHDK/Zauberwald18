package dedo;

import processing.core.*;
import processing.data.*;

import java.util.ArrayList;

public class Visualization {
  private PApplet p;

  private ArrayList<Curve> curves;

  public Visualization(PApplet parent) {
    // set parent
    this.p = parent;
  }

  public void setup() {
    curves = new ArrayList<>();

    Table solar = p.loadTable("dedo_solar.csv");
    Table wind = p.loadTable("dedo_wind.csv");
    Table water = p.loadTable("dedo_water.csv");

    int a = 0;
    int aa =
        360 / ((solar.getRowCount() - 1) + (wind.getRowCount() - 1) + (water.getRowCount() - 1));

    for (int i = 1; i < solar.getRowCount(); i++) {
      TableRow rows = solar.getRow(i);
      curves.add(new Curve(p, rows, 1646, p.color(255, 255, 255, 100), a, "solar"));
      a += aa;
    }

    for (int i = 1; i < wind.getRowCount(); i++) {
      TableRow rows = wind.getRow(i);
      curves.add(new Curve(p, rows, 12113, p.color(255, 255, 255, 99), a, "wind"));
      a += aa;
    }

    for (int i = 1; i < water.getRowCount(); i++) {
      TableRow rows = water.getRow(i);
      curves.add(new Curve(p, rows, 29508, p.color(255, 255, 255, 50), a, "wasser"));
      a += aa;
    }
  }

  public void draw(float water, float wind, float solar) {
    p.background(0);

    p.noStroke();
    p.fill(0);
    p.ellipse(p.width / 2f, p.height / 2f, p.height, p.height);

    for (Curve c : curves) {
      if (c.A < 180) {
        c.display(solar * 200, wind * 200, water * 200);
      }
    }

    for (int i = curves.size() - 1; i >= 0; i--) {
      if (curves.get(i).A >= 180) {
        curves.get(i).display(solar * 200, wind * 200, water * 200);
      }
    }
  }
}
