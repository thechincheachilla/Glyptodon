#include "Adafruit_PWMServoDriver.h"
#include "Leg.h"
#include "Swerve.h"
#include "Head.h"
#include "Tail.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "FastLED.h"
#define DATA_PIN 7

unsigned long tracks[] = {6500, 13500, 7500, 4500, 6500, 3500};/*{15500, 9500, 6500, 8500, 5500, 8500}; */
// The order is top to bottom on the SD card.

int track = 0;

const int robotSpeed = 50;

const int hallEffectPin = 8;
const int buttonPin = 12;
const int leftPhotoresistor = A1;
const int rightPhotoresistor = A2;
const int ledPin = 13;
const int ultrasonicPin = A0;

typedef enum {FORWARD, AVOIDANCE, HUNTING, SPEAKING, FOLLOWING} Mode;

Mode currentMode = FORWARD;

unsigned long modeStartTime;

int submodeStep;
int huntingStep;

bool preButtonPressed;

bool moveLegs;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

SoftwareSerial softwareSerial(10, 11);
DFRobotDFPlayerMini player;

Leg frontLeft(pwm, 0,  2, 450, false);
Leg frontRight(pwm, 9, 3, 450, true);
Leg backLeft(pwm, 1, 4, 450, false);
Leg backRight(pwm, 13, 5, 450, true);

Swerve swerve(pwm, 14, 15, 2);

Tail tail(pwm, 12, 10);

Head head(pwm, 11);

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);

  if (!player.begin(softwareSerial)) {
    Serial.println("DFPlayerMini failed to begin");
  }

  player.volume(50);

  randomSeed(analogRead(A5));

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(hallEffectPin, INPUT_PULLUP);

  swerve.setDrive(0, 0);
  
  zero();
  while (!frontLeft.isAtLimit() || !frontRight.isAtLimit() || !backLeft.isAtLimit() || !backRight.isAtLimit()) {
    Serial.println("Zeroing legs");
    zero();
    swerve.periodicUpdate();
    tail.periodicUpdate();
  } //TODO

  Serial.println("All legs zeroed");

  moveLegs = true;//TODO
}

void loop() {
  unsigned long currentTime = millis();
  double distance = getDistance();
  bool huntingTriggered = getMagnetFood();
  bool buttonPressed = getButton();
  bool followTriggered = getLightTriggered();

  switch (currentMode) {
    case FORWARD:
      if (buttonPressed && !preButtonPressed) {
        Serial.println("Speaking");
        currentMode = SPEAKING;
        moveLegs = false;
      } else if (followTriggered) {
        Serial.println("Following Triggered");
        currentMode = FOLLOWING;
        moveLegs = true;
      } else if (huntingTriggered) {
        Serial.println("Hunting Triggered");
        currentMode = HUNTING;
        moveLegs = false;
      } else if (distance < 35) {
        Serial.println("Avoidance Triggered");
        currentMode = AVOIDANCE;
        moveLegs = false;
      } else {
        moveLegs = true;//TODO
        swerve.setDrive(0, robotSpeed);
      }
      break;
    case SPEAKING:
      switch (submodeStep) {
        case 0:
          swerve.setDrive(0, 0);
          moveLegs = false;
          if (!frontLeft.isAtLimit() || !frontRight.isAtLimit() || !backLeft.isAtLimit() || !backRight.isAtLimit()) {
            frontLeft.zero();
            frontRight.zero();
            backLeft.zero();
            backRight.zero();
          } else {//TODO
            Serial.print("Playing track ");
            Serial.print(track + 1);
            Serial.print(" of length ");
            Serial.print(tracks[track]);
            player.play(track+1);
            modeStartTime = currentTime;
            if (track >= 4) {
              track = 0;
              unsigned long tracks[] = {6500, 13500, 7500, 4500, 6500, 3500};
            }
            else {
              track++;
            }
            submodeStep++;
          }
          break;
        case 1:
          if (currentTime - modeStartTime > tracks[track]) {
            submodeStep++;
          }
          break;
        case 2:
          submodeStep = 0;
          currentMode = FORWARD;
          break;
      }
      break;
    case AVOIDANCE:
      switch (submodeStep) {
        case 0:
          swerve.setDrive(0, 0);
          moveLegs = false;
          if (!frontLeft.isAtLimit() || !frontRight.isAtLimit() || !backLeft.isAtLimit() || !backRight.isAtLimit()) {
            frontLeft.zero();
            frontRight.zero();
            backLeft.zero();
            backRight.zero();
          } else {//TODO
            head.addOpens(4);
            submodeStep++;
          }
          break;
        case 1:
          if (head.getOpens() == -1) {
            submodeStep++;
            modeStartTime = currentTime;
            tail.addWags(4);
          }
          break;
        case 2:
          if (tail.getWags() == 0) {
            modeStartTime = currentTime;
            submodeStep++;
          }
          break;
        case 3:
          moveLegs = true;//TODO
          if (currentTime - modeStartTime > 2000) {
            modeStartTime = currentTime;
            submodeStep++;
          } else {
            swerve.setDrive(90, -100);
          }
          break;
        case 4:
          if (currentTime - modeStartTime > 2000) {
            submodeStep++;
            modeStartTime = currentTime;
          } else {
            swerve.setDrive(-90, 100);
          }
          break;
        case 5:
          submodeStep = 0;
          currentMode = FORWARD;
          break;
      }
      break;
    case HUNTING:
      if (!huntingTriggered) {
        currentMode = AVOIDANCE;
        digitalWrite(ledPin, LOW);
      } else {
        if (head.getOpens() < 0) {
          head.addOpens(2);
        }
        digitalWrite(ledPin, HIGH);
        swerve.setDrive(0, 0);
        moveLegs = false;
      }
      break;
    case FOLLOWING:
      if (!followTriggered) {
        currentMode = FORWARD;
      } else {
        int left = analogRead(leftPhotoresistor);
        int right = analogRead(rightPhotoresistor);
        double pos = (left - right) / (double) max(left, right);
        Serial.println(pos);
        swerve.setDrive(pos * 90, robotSpeed);
      }
      break;
  }

  preButtonPressed = buttonPressed;
  allPeriodic();
}

double getDistance() {
  return 5.0 * analogRead(ultrasonicPin) / 10.0;
}

bool getMagnetFood() {
  return !digitalRead(hallEffectPin);
}

bool getButton() {
  return !digitalRead(buttonPin);
}

void zero() {
  frontLeft.zero();
  frontRight.zero();
  backLeft.zero();
  backRight.zero();
}

bool getLightTriggered() {
  return (analogRead(leftPhotoresistor) + analogRead(rightPhotoresistor)) > 300;
}

void allPeriodic() {
  frontLeft.runLeg(moveLegs);
  frontRight.runLeg(moveLegs);
  backLeft.runLeg(moveLegs);
  backRight.runLeg(moveLegs);

  frontLeft.periodicUpdate();
  frontRight.periodicUpdate();
  backLeft.periodicUpdate();
  backRight.periodicUpdate();

  swerve.periodicUpdate();
  tail.periodicUpdate();
  head.periodicUpdate();
}
