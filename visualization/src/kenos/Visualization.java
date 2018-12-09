package kenos;

import processing.core.*;
import processing.data.*;

import static processing.core.PApplet.*;

public class Visualization {
  private PApplet p;

  private Clock clockConsumption;
  private FixedClock clockSun;
  private FixedClock clockWind;
  private FixedClock clockRain;

  private Rings ringWind;
  private Rings ringRain;
  private Rings ringSun;

  public Visualization(PApplet parent) {
    // set parent
    this.p = parent;
  }

  public void setup() {
    // load table
    Table data = p.loadTable("kenos.csv", "header,csv");

    // create clocks
    clockConsumption =
        new Clock(p, data, "consumption", p.color(255), 2500000); // eigentlich 3000000
    clockWind = new FixedClock(p, p.color(255, 0, 255));
    clockSun = new FixedClock(p, p.color(255, 255, 0));
    clockRain = new FixedClock(p, p.color(0, 255, 255));

    // create rings
    ringWind = new Rings(p, data, "wind", p.color(255, 0, 255));
    ringRain = new Rings(p, data, "rainfall", p.color(0, 255, 255));
    ringSun = new Rings(p, data, "sun", p.color(255, 255, 0));
  }

  public void draw(float t, float water, float wind, float solar) {
    // draw background
    p.background(0);

    // translate all points
    p.translate(p.width / 2f, p.height / 2f);

    // get consumption
    float consumption = clockConsumption.get();
    float multiplier = map(consumption, 0, 2500000, 0, 1);

    // calculate inputs
    float i1 = water * (multiplier / 3);
    float i2 = wind * (multiplier / 3);
    float i3 = solar * (multiplier / 3);

    // set current data
    clockSun.set(i1 + i2 + i3);
    clockWind.set(i1 + i2);
    clockRain.set(i1);

    // draw clocks
    clockConsumption.display();
    clockSun.display();
    clockWind.display();
    clockRain.display();

    // draw black inner circle
    p.fill(0);
    p.noStroke();
    p.ellipse(0, 0, 250, 250);

    // draw rings
    ringWind.paint();
    ringRain.paint();
    ringSun.paint();
  }
}
