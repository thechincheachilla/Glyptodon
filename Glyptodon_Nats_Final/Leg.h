#ifndef Leg_h
#define Leg_h

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class Leg {
  public:
    Leg(Adafruit_PWMServoDriver pwmServoDriver, int servoPin, int limitSwitchPin, unsigned long timeToMoveForwardMs, bool reverse);
    void periodicUpdate();
    bool isAtLimit();
    bool isMovingForward();
    void zero();
    void runLeg(bool enable);
  private:
    Adafruit_PWMServoDriver pwm;
    bool enabled;
    bool flip;
    bool zeroing;
    int servo;
    int limitSwitch;
    unsigned long fullExtensionTime;
    unsigned long startOfMovementTime;
    bool movingForward = true;
    bool previousLimitSwitch;
};

#endif
