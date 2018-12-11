package radar;

import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*;
import processing.core.*;
import processing.data.*;

import java.util.ArrayList;

import static processing.core.PApplet.*;

// TODO: Fix Radar Jump.

public class Visualization {
  private PApplet p;

  private ArrayList<Location> solarPositions = new ArrayList<>();
  private ArrayList<Location> waterPositions = new ArrayList<>();
  private ArrayList<Location> windPositions = new ArrayList<>();

  private UnfoldingMap map;
  private float angle = 0;

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

    viz(solarPositions, solar, p.color(255, 255, 0), 5, 1, 20);
    viz(waterPositions, water, p.color(0, 255, 255), 6, 1, 50);
    viz(windPositions, wind, p.color(255, 0, 255), 10, 3, 60);
  }

  private void viz(
      ArrayList<Location> positions,
      float wind,
      int color,
      int pointSize,
      int circleWeight,
      int circleSize) {
    float pointAngle;
    float pointerDistMin = 0;
    float pointerDistMax = 150;

    p.noStroke();

    for (Location loc : positions) {
      ScreenPosition pos = map.getScreenPosition(loc);

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
        p.fill(color, pointOpacity);
        p.ellipse(pos.x, pos.y, pointSize, pointSize);

        if (wind > 0 && pointerDist > 0) {
          p.noFill();
          p.stroke(color, pointOpacity);
          p.strokeWeight(circleWeight);

          float circleSizeWind = map(pointerDist, 0, 150, circleSize, 0);
          p.ellipse(pos.x, pos.y, circleSizeWind, circleSizeWind);
        }
      }
    }
  }
}
