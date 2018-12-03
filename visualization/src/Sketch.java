import mqtt.MQTTClient;
import processing.core.PApplet;

// TODO: Check MQTT stability.

public class Sketch extends PApplet {
  private MQTTClient client;

  private float water1;
  private float water2;
  private float wind1;
  private float wind2;
  private float solar1;
  private float solar2;

  private int start = 0;

  private kenos.Visualization kenos;

  private static final int LENGTH = 180;

  public void settings() {
    // set size
    size(1050, 1050, P3D);
  }

  public void setup() {
    // create mqtt client
    client = new MQTTClient(this);
    client.connect("mqtt://0.0.0.0:1884", "visualization");

    // subscribe to topics
    client.subscribe("#");

    // create visualizations
    kenos = new kenos.Visualization(this);

    // setup visualization
    kenos.setup();

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

    // push matrix
    this.pushMatrix();

    // TODO: Draw mask?

    // draw visualization
    this.kenos.draw(time, water, wind, solar);

    // pop matrix
    this.popMatrix();

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
