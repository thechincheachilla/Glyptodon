#include "Arduino.h"
#include "Swerve.h"
#include "Adafruit_PWMServoDriver.h"

#define SERVOMIN 200
#define SERVOMAX 550

Swerve::Swerve(Adafruit_PWMServoDriver pwmServoDriver, int turnPin, int frontDrivePin, int backDrivePin) {
  pwm = pwmServoDriver;
  turn = turnPin;
  frontDrive = frontDrivePin;
  backDrive = backDrivePin;
}

void Swerve::periodicUpdate() {
  pwm.setPWM(turn, 0, angle);

  pwm.setPWM(frontDrive, 0, power);
  pwm.setPWM(backDrive, 0, power);
}

void Swerve::setDrive(int angleTarget, int powerTarget){
  angle = map(angleTarget, -90, 90, SERVOMIN, SERVOMAX);
  power = powerTarget == 0 ? 0 : map(powerTarget, 100, -100, SERVOMIN, SERVOMAX);
}
