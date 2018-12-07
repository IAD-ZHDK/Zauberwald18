import blitzkraftwerk.Visualization;
import mqtt.MQTTClient;
import processing.core.*;

// TODO: Check MQTT stability.

public class Sketch extends PApplet {
  private MQTTClient client;

  private PShape mask;

  private float water1;
  private float water2;
  private float wind1;
  private float wind2;
  private float solar1;
  private float solar2;

  private int start = 0;

  private kenos.Visualization viz1;
  private electroswing.Visualization viz2;
  private dedo.Visualization viz3;
  private blitzkraftwerk.Visualization viz4;
  private radar.Visualization viz5;
  private wawiso.Visualization viz6;

  private static final int LENGTH = 180;

  public void settings() {
    // set size
    size(1050, 1050, P3D); // FX2D
  }

  public void setup() {
    // create mqtt client
    client = new MQTTClient(this);
    client.connect("mqtt://0.0.0.0:1884", "visualization");

    // subscribe to topics
    client.subscribe("#");

    // lod mask
    mask = loadShape("mask.svg");
    mask.setFill(color(0,0,0));
    mask.scale(mask.height / height);
    mask.translate((mask.width - width) / -2, 0);

    // create visualizations
    viz1 = new kenos.Visualization(this);
    viz2 = new electroswing.Visualization(this);
    viz3 = new dedo.Visualization(this);
    viz4 = new Visualization(this);
    viz5 = new radar.Visualization(this);
    viz6 = new wawiso.Visualization(this);

    // setup visualization
    viz1.setup();
    viz2.setup();
    viz3.setup();
    viz4.setup();
    viz5.setup();
    viz6.setup();

    // set start
    start = millis();
  }

  public void draw() {
    // get time difference
    int diff = millis() - start;

    // calculate time
    float time = constrain(map(diff, 0, LENGTH * 1000, 0, 1), 0, 1);

    // get max input levels
    float water = max(water1, water2);
    float wind = max(wind1, wind2);
    float solar = max(solar1, solar2);

    // push matrix and style
    this.pushMatrix();
    this.pushStyle();

    // draw visualization
    // viz1.draw(time, water, wind, solar);
    // viz2.draw(time, water, wind, solar);
    // viz3.draw(time, water, wind, solar);
    // viz4.draw(time, water, wind, solar);
    // viz5.draw(time, water, wind, solar);
    viz6.draw(time, water, wind, solar);

    // pop matrix and style
    this.popStyle();
    this.popMatrix();

    // draw mask
    fill(0);
    noStroke();
    shape(mask, 0, 0);

    // reset time
    if (time >= 1) {
      start = millis();
    }

    // draw frame rate
    fill(255);
    noStroke();
    text(frameRate, 10, 20);
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
}
