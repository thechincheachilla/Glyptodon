#include "Arduino.h"
#include "Heart.h"
#include "FastLED.h"
#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 255
#define NUM_LEDS 129
#define DATA_PIN 7

int hue = 0;
int divisor = 30;

CRGB leds[NUM_LEDS];

Heart::Heart(int heartPin) {
  pin = heartPin;
}

void Heart::periodicUpdate() {
  float breath = (exp(cos(millis()/fadeSpeed*PI)) - 0.36787944)*108.0; //changing 750 hanges breathing speed
  breath = map(breath, 0, 255, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  FastLED.setBrightness(breath);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));
  if(hue == (255 * divisor)) {
    hue = 0;
}}

void Heart::setMode(Mood mood) {
  currentMood = mood;
  switch (mood) {
    case CALM:
      fadeSpeed = 1000;
      break;
    case HUNT:
      fadeSpeed = 300;
      break;
    case SCARED:
      fadeSpeed = 500;
      break;
  }
}
