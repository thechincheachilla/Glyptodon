#ifndef Head_h
#define Head_h

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class Head {
  public:
    Head(Adafruit_PWMServoDriver pwmServoDriver, int headServo);
    void periodicUpdate();
    void addOpens(int opens);
    int getOpens();
  private:
    Adafruit_PWMServoDriver pwm;
    int remainingOpens;
    int servo;
    bool isMoving = true;
    bool isOpening = true;
    unsigned long startOfStepTime;
};

#endif
