void viz(float t, float water, float wind, float solar) {
  // add your code here! :)
  
  println(t);
  background(255);
  fill(solar * 255, wind * 255, water * 255);
  ellipse(width/2, height/2, height - 10, height - 10);
}
