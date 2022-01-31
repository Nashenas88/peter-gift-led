#define RED1   5
#define GREEN1 6
#define BLUE1  3

#define RED2   10
#define GREEN2 11
#define BLUE2  9

#define MIN 0
#define MAX 255

#define SNOW 0
#define CAR  1

#define DELAY 1
#define PRINT 0

#define PARTY_MODE_SPEED 7

// Party mode counter.
int k;

void setup() {
  #ifdef PRINT
  Serial.begin(9600);
  #endif
  // no-op
  analogWrite(RED2,   0);
  analogWrite(GREEN2, 0);
  analogWrite(BLUE2,  0);
  k = 0;
}

void loop() {
  // Set brightness based on the value from the potentiometer.
  int potentiometer = analogRead(A0);
  int brightness = map(potentiometer, 0, 2048, MIN, MAX);
  #ifdef PRINT
  Serial.print("Potentiometer:");
  Serial.print(potentiometer);
  #endif

  // If the pot reads about 1000, trigger party mode.
  if (potentiometer > 1000) {
    k = (k+PARTY_MODE_SPEED)%360;

    sineLED(CAR, k);

    // While the car is going through the hue wave, adjust the snow to
    // flash brightness in sequence.
    int b = min(359, k >= 180 ? (359 - k) * 2 : k * 2);
    b = map(b, 0, 359, 0, 128);
    setRGBpoint(SNOW, b, b, b, /* print */ false);
  } else {
    // When not in party mode set the car and snow brightness to the level
    // determined by the pot.
    // We only want the car to be blue.
    setRGBpoint(CAR, 0, 0, brightness, /* print */ true);
    setRGBpoint(SNOW, brightness, brightness, brightness, /* print */ false);
  }

  #ifdef PRINT
  Serial.println();
  #endif

  delay(1);
}

// Lookup table where lights[i] == sin(i) (where i is in degrees).
const uint8_t lights[360]={
  0,   0,   0,   0,   0,   1,   1,   2, 
  2,   3,   4,   5,   6,   7,   8,   9, 
 11,  12,  13,  15,  17,  18,  20,  22, 
 24,  26,  28,  30,  32,  35,  37,  39, 
 42,  44,  47,  49,  52,  55,  58,  60, 
 63,  66,  69,  72,  75,  78,  81,  85, 
 88,  91,  94,  97, 101, 104, 107, 111, 
114, 117, 121, 124, 127, 131, 134, 137, 
141, 144, 147, 150, 154, 157, 160, 163, 
167, 170, 173, 176, 179, 182, 185, 188, 
191, 194, 197, 200, 202, 205, 208, 210, 
213, 215, 217, 220, 222, 224, 226, 229, 
231, 232, 234, 236, 238, 239, 241, 242, 
244, 245, 246, 248, 249, 250, 251, 251, 
252, 253, 253, 254, 254, 255, 255, 255, 
255, 255, 255, 255, 254, 254, 253, 253, 
252, 251, 251, 250, 249, 248, 246, 245, 
244, 242, 241, 239, 238, 236, 234, 232, 
231, 229, 226, 224, 222, 220, 217, 215, 
213, 210, 208, 205, 202, 200, 197, 194, 
191, 188, 185, 182, 179, 176, 173, 170, 
167, 163, 160, 157, 154, 150, 147, 144, 
141, 137, 134, 131, 127, 124, 121, 117, 
114, 111, 107, 104, 101,  97,  94,  91, 
 88,  85,  81,  78,  75,  72,  69,  66, 
 63,  60,  58,  55,  52,  49,  47,  44, 
 42,  39,  37,  35,  32,  30,  28,  26, 
 24,  22,  20,  18,  17,  15,  13,  12, 
 11,   9,   8,   7,   6,   5,   4,   3, 
  2,   2,   1,   1,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0};

uint8_t outputPins[6] = {
  RED1, GREEN1, BLUE1,
  RED2, GREEN2, BLUE2
}; // PWM pins

// setRGBpoint (0, ...) for pins 5, 6, 3; setRGBpoint (1, ...) for pins 10, 11, 9.
// See array above
void setRGBpoint(byte LED, uint8_t red, uint8_t green, uint8_t blue, bool print)
{
  // this code is for common anode LEDs. If you use common cathode ones,
  // remove the '255-' bits.
  analogWrite(outputPins[LED*3], red);
  analogWrite(outputPins[LED*3+1], green);
  analogWrite(outputPins[LED*3+2], blue);

  #ifdef PRINT
  if (print) {
    Serial.print(" Red");
    Serial.print(LED);
    Serial.print(":");
    Serial.print(red);
    Serial.print(" Green");
    Serial.print(LED);
    Serial.print(":");
    Serial.print(green);
    Serial.print(" Blue");
    Serial.print(LED);
    Serial.print(":");
    Serial.print(blue);
  }
  #endif
}

// sine wave rainbow
void sineLED(byte LED, int angle) {
  #ifdef PRINT
  Serial.print(" Angle:");
  Serial.print(angle);
  #endif
  setRGBpoint(LED, lights[(angle+120)%360], lights[angle],  lights[(angle+240)%360], true);
}
