import mqtt.MQTTClient;
import processing.core.*;

// TODO: Check MQTT stability.

public class Sketch extends PApplet {
  private static final int LENGTH = 30 * 1000;
  private static final int FADE_LENGTH = 2 * 1000;
  private static final boolean DEBUG = false;
  private MQTTClient client;
  private PShape mask;
  private float water1;
  private float water2;
  private float wind1;
  private float wind2;
  private float solar1;
  private float solar2;
  private int start;
  private int current;
  private electroswing.Visualization viz1;
  private radschlag.Visualization viz2;
  private wawiso.Visualization viz3;
  private awave.Visualization viz4;
  private blitzkraftwerk.Visualization viz5;
  private radar.Visualization viz6;

  public void settings() {
    if (DEBUG) {
      size(1200, 800, P3D);
    } else {
      fullScreen(P3D);
    }
  }

  public void setup() {
    try {
      // create mqtt client
      client = new MQTTClient(this);
      client.connect("mqtt://0.0.0.0:1884", "visualization");

      // load mask
      mask = loadShape("mask.svg");
      mask.setFill(color(0, 0, 0));
      mask.scale(height / mask.height);
      mask.translate((mask.width * (height / mask.height) - width) / -2, 0);

      // create visualizations
      viz1 = new electroswing.Visualization(this);
      viz2 = new radschlag.Visualization(this);
      viz3 = new wawiso.Visualization(this);
      viz4 = new awave.Visualization(this);
      viz5 = new blitzkraftwerk.Visualization(this);
      viz6 = new radar.Visualization(this);

      // setup visualization
      viz1.setup();
      viz2.setup();
      viz3.setup();
      viz4.setup();
      viz5.setup();
      viz6.setup();

      // set start
      start = millis();

      // set first visualization
      current = 1;
    } catch (Exception e) {
      e.printStackTrace();
      exit();
    }
  }

  public void draw() {
    try {
      // get time difference
      int diff = millis() - start;

      // get max input levels
      float water = max(water1, water2);
      float wind = max(wind1, wind2);
      float solar = max(solar1, solar2);

      // push matrix and style
      this.pushMatrix();
      this.pushStyle();

      // draw visualization
      switch (current) {
        case 1:
          viz1.draw(water, wind, solar);
          break;
        case 2:
          viz2.draw(water, wind, solar);
          break;
        case 3:
          viz3.draw(water, wind, solar);
          break;
        case 4:
          viz4.draw(water, wind, solar);
          break;
        case 5:
          viz5.draw(water, wind, solar);
          break;
        case 6:
          viz6.draw(water, wind, solar);
          break;
      }

      // pop matrix and style
      this.popStyle();
      this.popMatrix();

      // draw mask
      fill(0, 0, 0);
      noStroke();
      shape(mask, 0, 0);
      rect(0, 0, (mask.width * (height / mask.height) - width) / -2, height);
      rect(
          ((mask.width * (height / mask.height) - width) / -2)
              + mask.width * (height / mask.height),
          0,
          width,
          height);

      // calculate fade
      float fade = 0;
      if (diff < FADE_LENGTH) {
        fade = constrain(map(diff, 0, FADE_LENGTH, 1, 0), 0, 1);
      } else if (diff > (LENGTH - FADE_LENGTH)) {
        fade = constrain(map(diff, (LENGTH - FADE_LENGTH), LENGTH, 0, 1), 0, 1);
      }

      // apply fade
      if (fade > 0) {
        fill(0, fade * 255);
        rect(0, 0, width, height);
      }

      // reset time and select next visualization
      if (diff >= LENGTH) {
        start = millis();
        current++;
        if (current > 6) {
          current = 1;
        }
      }

      // draw current viz and frame rate
      if (DEBUG) {
        fill(255);
        noStroke();
        text(current + " - " + frameRate, 10, 20);
      }

    } catch (Exception e) {
      e.printStackTrace();
      exit();
    }
  }

  public void clientConnected() {
    println("client connected");

    // subscribe to topics
    client.subscribe("#");
  }

  public void messageReceived(String topic, byte[] payload) {
    // parse payload
    String str = new String(payload);

    // check topic and assign inputs
    switch (topic) {
      case "water1/value":
        water1 = constrain(parseFloat(str), 0, 1);
        break;
      case "water2/value":
        water2 = constrain(parseFloat(str), 0, 1);
        break;
      case "wind1/value":
        wind1 = constrain(parseFloat(str), 0, 1);
        break;
      case "wind2/value":
        wind2 = constrain(parseFloat(str), 0, 1);
        break;
      case "solar1/value":
        solar1 = constrain(parseFloat(str), 0, 1);
        break;
      case "solar2/value":
        solar2 = constrain(parseFloat(str), 0, 1);
        break;
      default:
        // print message if not known
        println("unused message: " + topic + " - " + new String(payload));
    }
  }

  public void connectionLost() {
    println("connection lost");
  }

  public void keyPressed() {
    switch (keyCode) {
      case 37:
        current--;
        if (current < 1) {
          current = 6;
        }
        break;
      case 39:
        current++;
        if (current > 6) {
          current = 1;
        }
        break;
    }
  }
}
