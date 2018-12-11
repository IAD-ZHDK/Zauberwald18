package radar;

import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*;
import processing.core.*;
import processing.data.*;

import java.util.ArrayList;

import static common.Helpers.*;
import static processing.core.PApplet.*;

public class Visualization {
  private PApplet p;

  private ArrayList<Location> solarLocations = new ArrayList<>();
  private ArrayList<Location> waterLocations = new ArrayList<>();
  private ArrayList<Location> windLocations = new ArrayList<>();

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

    // load solar data
    Table solarDataCSV = p.loadTable("rd_solar.csv", "header, csv");
    for (TableRow solarPosRow : solarDataCSV.rows()) {
      float lat = solarPosRow.getFloat("Latitude");
      float lng = solarPosRow.getFloat("Longitude");
      solarLocations.add(new Location(lat, lng));
    }

    // load water data
    Table waterDataCSV = p.loadTable("rd_water.csv", "header, csv");
    for (TableRow waterPosRow : waterDataCSV.rows()) {
      float lat = waterPosRow.getFloat("Latitude");
      float lng = waterPosRow.getFloat("Longitude");
      waterLocations.add(new Location(lat, lng));
    }

    // load wind data
    Table windDataCSV = p.loadTable("rd_wind.csv", "header, csv");
    for (TableRow windPosRow : windDataCSV.rows()) {
      float lat = windPosRow.getFloat("Latitude");
      float lng = windPosRow.getFloat("Longitude");
      windLocations.add(new Location(lat, lng));
    }
  }

  public void draw(float water, float wind, float solar) {
    // set background
    p.background(0);

    // move radar
    angle += 3;
    if (angle >= 360) {
      angle = angle - 360;
    }

    // draw points and circles
    viz(solarLocations, solar, p.color(255, 255, 0), 5, 1, 20);
    viz(waterLocations, water, p.color(0, 255, 255), 6, 1, 50);
    viz(windLocations, wind, p.color(255, 0, 255), 10, 3, 60);
  }

  private void viz(
      ArrayList<Location> locations,
      float input,
      int color,
      int pointSize,
      int circleWeight,
      int circleSize) {
    // iterate through all locations
    for (Location loc : locations) {
      // get screen position
      ScreenPosition pos = map.getScreenPosition(loc);

      // get heading
      float heading = degrees((new PVector(pos.x - p.width / 2f, pos.y - p.height / 2f)).heading());

      // get angle diff
      float angleDiff = angleDiff(heading, angle);

      // check angle diff
      if (angleDiff < 90) {
        // calculate opacity
        float opacity = map(angleDiff, 90, 0, 0, 255);

        // draw point
        p.noStroke();
        p.fill(color, opacity);
        p.ellipse(pos.x, pos.y, pointSize, pointSize);

        // check input
        if (input > 0) {
          // calculate size
          float size = map(angleDiff, 90, 0, 0, circleSize) * input;

          // draw circle
          p.noFill();
          p.stroke(color, opacity);
          p.strokeWeight(circleWeight);
          p.ellipse(pos.x, pos.y, size, size);
        }
      }
    }
  }
}
