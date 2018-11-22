PShape vizMask;

float water = 0;
float wind = 0;
float solar = 0;

float t = 0;

void setup() {
  //fullScreen();
  size(1050, 1050, P3D);
  frameRate(60);

  // prepare mask  
  vizMask = loadShape("mask.svg");
  vizMask.setFill(color(0,0,0));
  vizMask.scale(vizMask.height / height);
  vizMask.translate((vizMask.width - width) / -2, 0);
}

void draw() {
  // increment time
  t += 1.0 / 60.0 / 150.0;
  
  // draw viz
  viz(t, water, wind, solar);

  // mask image
  shape(vizMask, 0, 0);
  
  // reset time
  if (t > 1) {
    t = 0;
  }
  
  noStroke();
  fill(255);
  text(frameRate, 20, 20);
}

void keyPressed() {
  float delta = 0.1;

  switch(key) {
  case 'q':
    water = constrain(water + delta, 0, 1);
    break;
  case 'w':
    wind = constrain(wind + delta, 0, 1);
    break;
  case 'e':
    solar = constrain(solar + delta, 0, 1);
    break;
  case 'a':
    water = constrain(water - delta, 0, 1);
    break;
  case 's':
    wind = constrain(wind - delta, 0, 1);
    break;
  case 'd':
    solar = constrain(solar - delta, 0, 1);
    break;
  }
  
  println("water: " + water + ", wind: " + wind + ", solar:", + solar);  
}
