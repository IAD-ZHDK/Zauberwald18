package radar;

import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*;
import processing.core.*;
import processing.data.*;

import java.util.ArrayList;

import static processing.core.PApplet.*;

// TODO: Fix Radar Jump.

// TODO: Merge three viz functions.

public class Visualization {
  private PApplet p;

  private ArrayList<Location> solarPositions = new ArrayList<>();
  private ArrayList<Location> waterPositions = new ArrayList<>();
  private ArrayList<Location> windPositions = new ArrayList<>();

  private UnfoldingMap map;
  private float angle = 0;

  private final static int SOLAR_SIZE = 5;
  private final static int WATER_SIZE = 6;
  private final static int WIND_SIZE = 10;

  public Visualization(PApplet parent) {
    // set parent
    this.p = parent;
  }

  public void setup() {
    // create interactive map centered around Switzerland
    map = new UnfoldingMap(this.p);
    map.zoomAndPanTo(8, new Location(46.986991, 8.178334));

    // Load CSV Solar data
    Table solarDataCSV = p.loadTable("rd_solar.csv", "header, csv");

    for (TableRow solarPosRow : solarDataCSV.rows()) {
      float lat = solarPosRow.getFloat("Latitude");
      float lng = solarPosRow.getFloat("Longitude");
      solarPositions.add(new Location(lat, lng));
    }

    // Load CSV Wind data
    Table waterDataCSV = p.loadTable("rd_water.csv", "header, csv");

    for (TableRow waterPosRow : waterDataCSV.rows()) {
      float lat = waterPosRow.getFloat("Latitude");
      float lng = waterPosRow.getFloat("Longitude");
      waterPositions.add(new Location(lat, lng));
    }

    // Load CSV Wind data
    Table windDataCSV = p.loadTable("rd_wind.csv", "header, csv");

    for (TableRow windPosRow : windDataCSV.rows()) {
      float lat = windPosRow.getFloat("Latitude");
      float lng = windPosRow.getFloat("Longitude");
      windPositions.add(new Location(lat, lng));
    }
  }

  public void draw(float water, float wind, float solar) {
    p.background(0, 50);

    angle += 3;
    if (angle > 360) {
      angle = 0;
    }

    vizSolar(solar);
    vizWater(water);
    vizWind(wind);
  }

  private void vizSolar(float solar) {
    float pointDist;
    float pointAngle;
    float pointerDist;
    float pointOpacity;
    float pointRadInner;
    int pointerDistMin = 0;
    int pointerDistMax = 80;
    float circleSizeSolar;

    p.noStroke();

    for (Location solarPos : solarPositions) {
      ScreenPosition pos = map.getScreenPosition(solarPos);
      pointDist = dist(p.width / 2f, p.height / 2f, pos.x, pos.y);
      pointRadInner = (pos.x - (p.width / 2f)) / pointDist;
      if (pos.y < (p.height / 2)) {
        pointAngle = map(pointRadInner, 1, -1, 3, 180);
      } else {
        pointAngle = map(pointRadInner, -1, 1, 183, 360);
      }

      pointerDist = angle - pointAngle;
      if (pointerDist < -340 && pointerDist > -360) {
        pointerDist = map(pointerDist, -300, -360, 150, 0);
      }
      if (pointerDist < pointerDistMax && pointerDist > pointerDistMin) {
        pointOpacity = map(pointerDist, pointerDistMax, pointerDistMin, 0, 100);
        p.noStroke();
        p.fill(255, 255, 0, pointOpacity);
        p.ellipse(pos.x, pos.y, SOLAR_SIZE, SOLAR_SIZE);
        if (solar > 0 && pointerDist > 0) {
          p.noFill();
          p.stroke(255, 255, 0, pointOpacity);
          p.strokeWeight(1);
          circleSizeSolar = map(pointerDist, 150, 0, 0, 20);
          p.ellipse(pos.x, pos.y, circleSizeSolar, circleSizeSolar);
        }
      }
    }
  }

  private  void vizWater(float water) {
    float pointDist;
    float pointAngle;
    float pointerDist;
    float pointOpacity;
    float pointRadInner;
    float pointerDistMin = 0;
    float pointerDistMax = 120;
    float circleSizeWater;

    p.noStroke();

    for (Location waterPos : waterPositions) {
      ScreenPosition pos = map.getScreenPosition(waterPos);
      pointDist = dist(p.width / 2f, p.height / 2f, pos.x, pos.y);
      pointRadInner = (pos.x - (p.width / 2f)) / pointDist;
      if (pos.y < (p.height / 2)) {
        pointAngle = map(pointRadInner, 1, -1, 3, 180);
      } else {
        pointAngle = map(pointRadInner, -1, 1, 183, 360);
      }

      pointerDist = angle - pointAngle;
      if (pointerDist < -340 && pointerDist > -360) {
        pointerDist = map(pointerDist, -300, -360, 150, 0);
      }
      if (pointerDist < pointerDistMax && pointerDist > pointerDistMin) {
        pointOpacity = map(pointerDist, pointerDistMax, pointerDistMin, 0, 100);
        p.noStroke();
        p.fill(0, 255, 255, pointOpacity);
        p.ellipse(pos.x, pos.y, WATER_SIZE, WATER_SIZE);
        if (water > 0 && pointDist > 0) {
          p.noFill();
          p.stroke(0, 255, 255, pointOpacity);
          p.strokeWeight(1);
          circleSizeWater = map(pointerDist, 0, 150, 50, 0);
          p.ellipse(pos.x, pos.y, circleSizeWater, circleSizeWater);
        }
      }
    }
  }

  private void vizWind(float wind) {
    float pointAngle;
    float pointerDistMin = 0;
    float pointerDistMax = 150;

    p.noStroke();

    for (Location windPos : windPositions) {
      ScreenPosition pos = map.getScreenPosition(windPos);

      float pointDist = dist(p.width / 2f, p.height / 2f, pos.x, pos.y);
      float pointRadInner = (pos.x - (p.width / 2f)) / pointDist;

      if (pos.y < (p.height / 2)) {
        pointAngle = map(pointRadInner, 1, -1, 3, 180);
      } else {
        pointAngle = map(pointRadInner, -1, 1, 183, 360);
      }

      float pointerDist = angle - pointAngle;
      if (pointerDist < -340 && pointerDist > -360) {
        pointerDist = map(pointerDist, -300, -360, 150, 0);
      }

      if (pointerDist < pointerDistMax && pointerDist > pointerDistMin) {
        float pointOpacity = map(pointerDist, pointerDistMax, pointerDistMin, 0, 100);

        p.noStroke();
        p.fill(255, 0, 255, pointOpacity);
        p.ellipse(pos.x, pos.y, WIND_SIZE, WIND_SIZE);

        if (wind > 0 && pointerDist > 0) {
          p.noFill();
          p.stroke(255, 0, 255, pointOpacity);
          p.strokeWeight(3);

          float circleSizeWind = map(pointerDist, 0, 150, 60, 0);
          p.ellipse(pos.x, pos.y, circleSizeWind, circleSizeWind);
        }
      }
    }
  }
}
