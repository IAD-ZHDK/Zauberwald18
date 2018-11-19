//subtle breathing effect
int brightness = 100;
int fadeAmount = 1;
const int dimDelay = 10;
unsigned long lastNepPixelupdate = 0;
void neoPixels() {
  if (millis() - lastNepPixelupdate > dimDelay) {
    lastNepPixelupdate = millis();
    brightness = brightness + fadeAmount;
    if (brightness <= 80 || brightness >= 200) {
      fadeAmount = -fadeAmount;
    }
    // yellow for wind object
    RgbColor color(brightness, brightness, 0);
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.SetPixelColor(i, color);
    }
    strip.Show();
  }
}
