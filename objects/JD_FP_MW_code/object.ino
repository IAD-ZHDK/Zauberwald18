//pins
#define pinA       33  // encoder pin
#define master     16  // motor low impedence pin
#define M1a       19  // motor
#define M1b       18  // motor
#define M2a       17  // motor
#define M2b       23  // motor
//
int pulses = 0; // used for keeping track of encoder speed
unsigned long lastReading;

// running average
const int numReadings = 10;
float readings[numReadings];
int readIndex = 0;
int total = 0;
int highestValue = 2;
int lowestValue = 0;
int average = 0;
//
float motorSpeed = 0;

const char* getObjectName() {
  const char* objectName = "wind1";
  return objectName;
}

void objectSetup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(master, OUTPUT);
  pinMode(M1a, OUTPUT);
  pinMode(M2a, OUTPUT);
  pinMode(M1b, OUTPUT);
  pinMode(M2b, OUTPUT);
  //
  digitalWrite(master, HIGH);
  digitalWrite(M1a, LOW);
  digitalWrite(M2a, LOW);
  digitalWrite(M1b, LOW);// always low since we only rotate in one direction
  digitalWrite(M2b, LOW);// always low since we only rotate in one direction
  attachInterrupt(digitalPinToInterrupt(pinA), encoderPulse, FALLING); // only one pin needed since we don't care what direction it is going
}

void encoderPulse() {
  pulses++;
}

float object() {
 
  if (millis() - lastReading > 50) { // check readings every 50 miliseconds
    lastReading =  millis();
    // smoothing
    total = total - readings[readIndex]; // subtract the last reading
    readings[readIndex] = pulses;
    pulses = 0;
    total = total + readings[readIndex]; // add the new reading
    readIndex = readIndex + 1; // advance the index
    readIndex = readIndex % numReadings; // wrap index
    average = total / numReadings;
  }
  float  Hz = average;
  Hz = (Hz / 200) * 20;  //200 pulses per rotation. Since we check every 50 millis, we multiply by 20 to get rotations per second
  //
  if (Hz > highestValue) {
    highestValue = Hz; 
  }

  float newSpeed =  mapFloat(Hz, lowestValue, highestValue, 0, 150);
  if (newSpeed > motorSpeed) {
    motorSpeed = newSpeed;
  }
  if (motorSpeed > 0) {
    digitalWrite(master, HIGH);
    motorSpeed -=  .5; //ramp down motor speed slowly
  } else {
    digitalWrite(master, LOW);
    motorSpeed = 0;
  }

  analogWrite(M1a, int(motorSpeed));
  analogWrite(M2a, int(motorSpeed));

  float returnValue =  mapFloat(Hz, lowestValue, highestValue, 0.0, 1.0);
  return returnValue;
}

//
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
