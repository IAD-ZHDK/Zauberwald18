package electroswing;

import processing.core.*;
import processing.data.*;

import static processing.core.PApplet.*;

public class Visualization {
  private PApplet p;

  private Rings waterRing;
  private Rings windRing;
  private Rings solarRing;
  private Rings AKWRing;

  private int counterWind = 0;
  private int counterWater = 0;
  private int counterSolar = 0;

  // ------

  private int[] grossProduction;

  private float[] endValues = new float[12];
  private float maxAKWAmplitude = 0;
  private float currentAKWAmplitude = 0;
  private int k = 0;

  private final static int ADD_SPEED = 5;
  private final static int REMOVE_SPEED = 90;
  private final static float AKW_STEPS = 0.001f;

  public Visualization(PApplet parent) {
    // set parent
    this.p = parent;
  }

  public void setup() {
    // load data
    Table table = p.loadTable("electroswing.csv", "header,csv");

    // prepare array
    grossProduction = new int[table.getRowCount()];

    // fill array
    int i = 0;
    for (TableRow row : table.rows()) {
      grossProduction[i] = row.getInt("bruttoerzeugung");
      i++;
    }

    // create rings
    waterRing = new Rings(p, p.color(0, 255, 255), 25);
    windRing = new Rings(p, p.color(255, 0, 255), 25);
    solarRing = new Rings(p, p.color(255, 255, 0), 25);
    AKWRing = new Rings(p, p.color(255), 25);
  }

  public void draw(float water, float wind, float solar) {
    if (currentAKWAmplitude > maxAKWAmplitude - AKW_STEPS
        && currentAKWAmplitude < maxAKWAmplitude + AKW_STEPS) {
      endValues[k] = map(grossProduction[k], 1322898, 2239285, 0, 1);
      maxAKWAmplitude = endValues[k];
      if (k >= endValues.length - 1) {
        k = 0;
      }
      k++;
    }

    if (maxAKWAmplitude > currentAKWAmplitude) {
      currentAKWAmplitude += AKW_STEPS;
    }

    if (maxAKWAmplitude < currentAKWAmplitude) {
      currentAKWAmplitude -= AKW_STEPS;
    }

    // ----

    // draw background
    p.background(0);

    // translate all points
    p.translate(p.width / 2f, p.height / 2f);

    // prepare variables
    float biggestSize = p.height / 4f * 0.6f;
    float akw = currentAKWAmplitude;
    float windMaxOutside = biggestSize / 1f;
    float waterMaxOutside = biggestSize / 17.29f;
    float solarMaxOutside = biggestSize / 3.46f;
    float AKWMaxOutside = biggestSize / 12.1f;

    if (p.frameCount % ADD_SPEED == 0) {
      if (windRing.count > windMaxOutside - 1
          || waterRing.count > waterMaxOutside - 1
          || solarRing.count > solarMaxOutside - 1) {
        AKWRing.remove();
      } else {
        AKWRing.add(AKWMaxOutside);
      }
    }

    windRing.paint(wind);
    solarRing.paint(solar);
    waterRing.paint(water);
    AKWRing.paint(akw);

    if (water > Rings.MIN_AMPLITUDE) {
      counterWater++;
    } else {
      counterWater = 0;
    }

    if (wind > Rings.MIN_AMPLITUDE) {
      counterWind++;
    } else {
      counterWind = 0;
    }

    if (solar > Rings.MIN_AMPLITUDE) {
      counterSolar++;
    } else {
      counterSolar = 0;
    }

    // add/remove solar rings
    if (counterSolar % ADD_SPEED == 0 && counterSolar != 0) {
      solarRing.add(solarMaxOutside);
    }
    if (counterSolar % REMOVE_SPEED == 0 && counterSolar == 0) {
      solarRing.remove();
    }

    // add/remove wind rings
    if (counterWind % ADD_SPEED == 0 && counterWind != 0) {
      windRing.add(windMaxOutside);
    }
    if (counterWind % REMOVE_SPEED == 0 && counterWind == 0) {
      windRing.remove();
    }

    // add/remove water rings
    if (counterWater % ADD_SPEED == 0 && counterWater != 0) {
      waterRing.add(waterMaxOutside);
    }
    if (counterWater % REMOVE_SPEED == 0 && counterWater == 0) {
      waterRing.remove();
    }
  }
}
