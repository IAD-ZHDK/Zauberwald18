PGraphics vizMask;

float water = 0;
float wind = 0;
float solar = 0;

void setup() {
  // fullScreen();
  size(1000, 1000);

  // prepare mask  
  vizMask = createGraphics(width, height);
  vizMask.beginDraw();
  vizMask.background(0);
  vizMask.ellipse(width/2, height/2, height, height);
  vizMask.endDraw();
} 

void draw() {
  // draw viz
  viz(1, water, wind, solar);

  // get image
  PImage i = get();

  // mask image
  i.mask(vizMask);

  // draw image
  background(0);
  image(i, 0, 0, width, height);
}

void viz(float t, float water, float wind, float solar) {
  // add your code here! :)
  
  background(255);
  fill(solar * 255, wind * 255, water * 255);
  ellipse(width/2, height/2, height - 10, height - 10);
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
