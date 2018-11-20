PVector pointOnCircle(float angle, float distance) {
  float r = radians(angle);
  float x = cos(r);
  float y = sin(r);
  PVector p = new PVector(x, y, 0);
  return p.mult(distance);
}
