#ifndef Tail_h
#define Tail_h

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class Tail {
  public:
    Tail(Adafruit_PWMServoDriver pwmServoDriver, int leftServo, int rightServo);
    void periodicUpdate();
    void addWags(int numberOfWags);
    int getWags();
  private:
    Adafruit_PWMServoDriver pwm;
    int remainingWags;
    int left;
    int right;
    bool isMovingLeft;
    unsigned long startOfMovementTime;
};

#endif
