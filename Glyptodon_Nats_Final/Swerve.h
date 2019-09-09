#ifndef Swerve_h
#define Swerve_h

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class Swerve {
  public:
    Swerve(Adafruit_PWMServoDriver pwmServoDriver, int turnPin, int frontDrivePin, int backDrivePin);
    void periodicUpdate();
    void setDrive(int angleTarget, int powerTarget);
  private:
    Adafruit_PWMServoDriver pwm;
    int turn;
    int frontDrive;
    int backDrive;
    int angle;
    int power;
};

#endif
