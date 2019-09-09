#include "Arduino.h"
#include "Tail.h"
#include "Adafruit_PWMServoDriver.h"

#define SERVOMIN 220
#define SERVOMAX 530

Tail::Tail(Adafruit_PWMServoDriver pwmServoDriver, int leftServo, int rightServo) {
  pwm = pwmServoDriver;
  left = leftServo;
  right = rightServo;
}

void Tail::periodicUpdate() {
  unsigned long currentTime = millis();

  if (remainingWags > 0) {
    if (currentTime - startOfMovementTime > 1000) {
      startOfMovementTime = currentTime;
      isMovingLeft = !isMovingLeft;
      remainingWags--;
    }

    pwm.setPWM(left, 0, isMovingLeft ? SERVOMAX : SERVOMIN);
    pwm.setPWM(right, 0, isMovingLeft ? SERVOMAX : SERVOMIN);
  } else {
    pwm.setPWM(left, 0, (SERVOMAX + SERVOMIN) / 2);
    pwm.setPWM(right, 0, (SERVOMAX + SERVOMIN) / 2);
  }
}

void Tail::addWags(int wags) {
  remainingWags += wags;
}

int Tail::getWags() {
  return remainingWags;
}
