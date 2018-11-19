const char* objectName;
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient client;
const char ssid[] = "BRIDGE";
const char pass[] = "internet";
unsigned long lastPublish = 0;
float OutputValue = 0;
float lastOutputValue = 0;

// NEOPIXELS
#define PixelPIN       13
#define NUMPIXELS      30
const uint8_t AnimationChannels = 1;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUMPIXELS, PixelPIN);
//
void connect() {
 
  Serial.print("checking wifi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);  // IMPORTANT: change this that the object isn't blocked if there is no wifi for whatever reason
  }
  Serial.print("connecting...");
  while (!client.connect(objectName, "zw18zw18", "zw18zw18")) {
    Serial.print(".");
    delay(1000);  // IMPORTANT: change this that the object isn't blocked if there is no wifi for whatever reason
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}


void setup() {
  
  Serial.begin (9600);
  //object
  objectName = getObjectName();
  objectSetup();

  //neoPixels
  strip.Begin();
  strip.Show();
  //WIFI
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }
  //get value from object and run object routine
  OutputValue = float(object());
  // publish a message every 100 milis, if there is a change in value.
  if (millis() - lastPublish > 100) {
    if (lastOutputValue != OutputValue) {
      lastPublish = millis();
      client.publish(objectName, String(OutputValue));
      Serial.println(OutputValue);
      lastOutputValue = OutputValue;
    }
  }
  //run ambient lighting routine
  neoPixels();
}
