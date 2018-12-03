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

  // TODO: Smoothing.
  // TODO: User Input (animate Consumptions).
  // TODO: Bigger Circle in the middle.
  // TODO: Images => Poster.

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
    clockWind = new FixedClock(p, p.color(255, 0, 255, 200));
    clockSun = new FixedClock(p, p.color(255, 255, 0, 200));
    clockRain = new FixedClock(p, p.color(0, 255, 255, 200));

    // create rings
    ringWind = new Rings(p, data, "wind", p.color(255, 0, 255));
    ringRain = new Rings(p, data, "rainfall", p.color(0, 255, 255));
    ringSun = new Rings(p, data, "sun", p.color(255, 255, 0));
  }

  public void draw(float t, float water, float wind, float solar) {
    // draw background
    p.background(0);

    // get consumption
    float consumption = clockConsumption.get();
    float multiplier = map(consumption, 0, 2500000, 0, 1);

    // set current data
    clockRain.set(water * multiplier);
    clockWind.set(wind * multiplier);
    clockSun.set(solar * multiplier);

    // draw clocks
    clockConsumption.display();
    clockSun.display();
    clockWind.display();
    clockRain.display();

    // draw black inner circle
    p.fill(0);
    p.noStroke();
    p.ellipse(p.width / 2f, p.height / 2f, 250, 250);

    // draw rings
    ringWind.paint();
    ringRain.paint();
    ringSun.paint();
  }
}
