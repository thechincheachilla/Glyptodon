#ifndef Heart_h
#define Heart_h

#include "Arduino.h"

typedef enum {CALM, HUNT, SCARED} Mood;

class Heart {
  public:
    Heart(int heartPin);
    void periodicUpdate();
    void setMode(Mood mood);
  private:
    int pin;
    Mood currentMood;
    int fadeSpeed;
    int hue;
    int divisor;
};

#endif
