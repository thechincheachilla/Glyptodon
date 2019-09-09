#include "Arduino.h"
#include "Head.h"
#include "Adafruit_PWMServoDriver.h"

#define SERVOMIN 200 //was 200 before editing
#define SERVOMAX 550 //was 550 before editing

Head::Head(Adafruit_PWMServoDriver pwmServoDriver, int headServo) {
  pwm = pwmServoDriver;
  servo = headServo;
}

void Head::periodicUpdate() {
  unsigned long currentTime = millis();

  if (remainingOpens > -1) {
    if (isMoving) {
      if (currentTime - startOfStepTime > (isOpening ? 100 : 100)) {
        isMoving = false;
        startOfStepTime = currentTime;
        isOpening = !isOpening;
        remainingOpens--;
      }
    } else {
      if (currentTime - startOfStepTime > 1000) {
        isMoving = true;
        startOfStepTime = currentTime;
      }
    }
    pwm.setPWM(servo, 0, isMoving ? (isOpening ? SERVOMAX : SERVOMIN) : 0);
  } else {
    pwm.setPWM(servo, 0, 0);
  }
}

void Head::addOpens(int opens) {
  remainingOpens += opens;
}

int Head::getOpens() {
  return remainingOpens;
}
