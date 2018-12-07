package blitzkraftwerk;

import processing.core.PApplet;
import processing.data.Table;

import java.util.ArrayList;

import static processing.core.PApplet.radians;

public class Visualization {
  private ArrayList<ArcLight> arcsLight;
  private ArrayList<ArcRain> arcsRain;
  private ArrayList<ArcWind> arcsWind;
  private ArrayList<ArcSun> arcsSun;
  private Table lightning;
  private Table rain;
  private Table wind;
  private Table sun;
  private int randomChoose;
  private int randomChoose180;
  private int randomChoose2;
  private int counterLight = 0;
  private int counterSun = 0;
  private int counterRain = 0;
  private int counterWind = 0;
  private float inter;
  private int previousMillis = 0;
  private boolean state, pstate;
  private PApplet p;

  public Visualization(PApplet parent) {
    // set parent
    this.p = parent;
  }

  public void setup() {
    inter = p.random(100, 2000);

    lightning = p.loadTable("bk_lightning.csv", "header");
    rain = p.loadTable("bk_rain.csv", "header");
    wind = p.loadTable("bk_wind.csv", "header");
    sun = p.loadTable("bk_sun.csv", "header");

    arcsLight = new ArrayList<>();
    arcsRain = new ArrayList<>();
    arcsWind = new ArrayList<>();
    arcsSun = new ArrayList<>();
  }

  public void draw(float t, float water, float wind, float solar) {
    p.background(0);

    p.strokeWeight(9.5f);
    p.noFill();

    boolean animRain = water > 0;
    boolean animWind = wind > 0;
    boolean animSun = solar > 0;

    state = true;
    if (state != pstate)
      if (arcsRain.size() == 0 && arcsWind.size() == 0 && arcsSun.size() == 0) {
        randomChoose2 = (int) p.random(0, 11);
        if (randomChoose2 > 5) {
          randomChoose180 = randomChoose2 - 6;
        } else {
          randomChoose180 = randomChoose2 + 6;
        }
        pstate = state;
      }

    if (p.millis() - previousMillis >= inter) {
      randomChoose = (int) p.random(0, 11);
      previousMillis = p.millis();
      for (int i = 0; i < arcsLight.size(); i++) {
        arcsLight.get(i).negativeIncrement = 0.1f;
      }
      drawArcLight(randomChoose, 0.5f, 0);
      inter = p.random(100, 2000);
    }

    for (int i = 0; i < arcsLight.size(); i++) {
      if (arcsLight.get(i).reset()) {
        counterLight++;
      }
    }
    for (int i = counterLight - 1; i >= 0; i--) {
      arcsLight.remove(i);
      counterLight = 0;
    }

    if (arcsRain.size() < 1 && animRain) {
      drawArcRain(randomChoose2);
      drawArcRain(randomChoose180);
    }

    for (int i = 0; i < arcsRain.size(); i++) {
      if (!animRain && arcsRain.get(i).reset()) {
        counterRain++;
      }
    }
    if (counterRain >= arcsRain.size()) {
      arcsRain.clear();
      counterRain = 0;
    }

    if (arcsWind.size() < 1 && animWind) {
      drawArcWind(randomChoose2);
      drawArcWind(randomChoose180);
    }
    for (int i = 0; i < arcsWind.size(); i++) {
      if (!animWind && arcsWind.get(i).reset()) {
        counterWind++;
      }
    }
    if (counterWind >= arcsWind.size()) {
      arcsWind.clear();
      counterWind = 0;
    }
    if (arcsSun.size() < 1 && animSun) {
      drawArcSun(randomChoose2);
      drawArcSun(randomChoose180);
    }
    for (int i = 0; i < arcsSun.size(); i++) {
      if (!animSun && arcsSun.get(i).reset()) {
        counterSun++;
      }
    }
    if (counterSun >= arcsSun.size()) {
      arcsSun.clear();
      counterSun = 0;
    }

    for (int i = 0; i < arcsLight.size(); i++) {
      p.stroke(255, 100);
      ArcLight tempArc = arcsLight.get(i);
      tempArc.display();
    }

    for (int i = 0; i < arcsRain.size(); i++) {
      p.stroke(0, 255, 255, 150);
      ArcRain tempArc = arcsRain.get(i);
      tempArc.display(animRain);
    }

    for (int i = 0; i < arcsWind.size(); i++) {
      p.stroke(255, 0, 255, 125);
      ArcWind tempArc = arcsWind.get(i);
      tempArc.display(animWind);
    }

    for (int i = 0; i < arcsSun.size(); i++) {
      p.stroke(255, 255, 0, 125);
      ArcSun tempArc = arcsSun.get(i);
      tempArc.display(animSun);
    }
  }

  void drawArcLight(int column, float speedforw, float speedback) {
    for (int i = 0; i < lightning.getRowCount(); i++) {
      int diameter = lightning.getInt(i, column * 3);
      float startPoint = radians(270 + column * 30);
      float endPoint = lightning.getInt(i, (column * 3) + 2);
      endPoint = radians(endPoint) + column * radians(30);
      if (diameter != 0 || endPoint != column * radians(30)) {
        arcsLight.add(
            new ArcLight(p, 150 + diameter * 19, startPoint, endPoint, speedforw, speedback));
      }
    }
  }

  void drawArcRain(int column) {
    for (int i = 0; i < rain.getRowCount(); i++) {
      int diameter = rain.getInt(i, column * 3);
      float frequenz = rain.getFloat(i, (column * 3) + 2);
      float startPoint = radians(270 + column * 30);
      if (diameter != 0 || !Float.isNaN(frequenz)) {
        arcsRain.add(
            new ArcRain(
                p, 155 + diameter * 19, startPoint, startPoint + radians(360), frequenz, 0.1f));
      }
    }
  }

  void drawArcWind(int column) {
    for (int i = 0; i < wind.getRowCount(); i++) {
      int diameter = wind.getInt(i, column * 3);
      float frequenz = wind.getFloat(i, (column * 3) + 2);
      float startPoint = radians(270 + column * 30);
      if (diameter != 0 || !Float.isNaN(frequenz)) {
        arcsWind.add(
            new ArcWind(
                p, 160 + diameter * 19, startPoint, startPoint + radians(360), frequenz, 0.1f));
      }
    }
  }

  void drawArcSun(int column) {
    for (int i = 0; i < sun.getRowCount(); i++) {
      int diameter = sun.getInt(i, column * 3);
      float frequenz = sun.getFloat(i, (column * 3) + 2);
      float startPoint = radians(270 + column * 30);
      if (diameter != 0 || !Float.isNaN(frequenz)) {
        arcsSun.add(
            new ArcSun(p,165 + diameter * 19, startPoint, startPoint + radians(360), frequenz, 0.1f));
      }
    }
  }
}
