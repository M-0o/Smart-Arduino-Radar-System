#include <Servo.h>
#include <Stepper.h>

const int STEPS_PER_REV = 2048;
Stepper myStepper(STEPS_PER_REV, 30, 31, 32, 33);

Servo s_front, s_back, s_right, s_left;

const int trigPins[] = {22, 24, 26, 28};
const int echoPins[] = {23, 25, 27, 29};

const int firePin = 13;
const int laserPin = 6;
const int buzzerPin = 49;

const int MIN_TARGET = 5;
const int MAX_TARGET = 10;

const int SERVO_MIN = 60;
const int SERVO_MAX = 120;

const unsigned long TARGET_HOLD_MS = 5000;

float filteredDist[4] = {400, 400, 400, 400};
float kalmanGain = 0.5;

int sweepAngle = 90;
int sweepStep = 1;

int currentStepperAngle = 0;

unsigned long lastActionMs = 0;
unsigned long ignoreUntil = 0;

bool isPaused = false;
bool buzzerActive = false;

unsigned long buzzerStartTime = 0;
const unsigned long BUZZER_DURATION = 200;

bool targetLocked = false;
unsigned long lastTargetTime = 0;
int lockedAngle = 90;
int lockedDistances[4] = {400, 400, 400, 400};

int offset[] = {0, 180, 90, 270};

void setup() {
  Serial.begin(115200);

  pinMode(firePin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(firePin, LOW);
  digitalWrite(laserPin, LOW);
  digitalWrite(buzzerPin, LOW);

  s_front.attach(4);
  s_back.attach(3);
  s_right.attach(2);
  s_left.attach(5);

  myStepper.setSpeed(16);

  for (int i = 0; i < 4; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  s_front.write(sweepAngle);
  s_back.write(sweepAngle);
  s_right.write(sweepAngle);
  s_left.write(sweepAngle);
}

void loop() {
  int rawDistances[4];

  for (int i = 0; i < 4; i++) {
    int raw = getDistance(trigPins[i], echoPins[i]);

    filteredDist[i] =
      (kalmanGain * filteredDist[i]) +
      ((1.0 - kalmanGain) * raw);

    rawDistances[i] = (int)filteredDist[i];
  }

  if (isPaused && millis() > ignoreUntil) {
    isPaused = false;
  }

  if (buzzerActive && millis() - buzzerStartTime >= BUZZER_DURATION) {
    digitalWrite(buzzerPin, LOW);
    buzzerActive = false;
  }

  int targetIdx = -1;

  if (!isPaused) {
    for (int i = 0; i < 4; i++) {
      if (filteredDist[i] >= MIN_TARGET && filteredDist[i] <= MAX_TARGET) {
        targetIdx = i;
        break;
      }
    }
  }

  if (targetIdx != -1) {
    targetLocked = true;
    lastTargetTime = millis();
    lockedAngle = sweepAngle;

    for (int i = 0; i < 4; i++) {
      lockedDistances[i] = rawDistances[i];
    }

    if (!buzzerActive) {
      digitalWrite(buzzerPin, HIGH);
      buzzerActive = true;
      buzzerStartTime = millis();
    }

    int localAngle = sweepAngle - 90;
    int targetAngle = offset[targetIdx] + localAngle;

    if (targetAngle < 0) targetAngle += 360;
    if (targetAngle >= 360) targetAngle -= 360;

    followTarget(targetAngle);

    sendDataMega(lockedAngle, lockedDistances);

    if (Serial.available() > 0) {
      char cmd = Serial.read();

      if (cmd == 'F') {
        digitalWrite(firePin, HIGH);
        digitalWrite(laserPin, HIGH);

        delay(100);

        digitalWrite(firePin, LOW);
        digitalWrite(laserPin, LOW);

        startIgnorePeriod(5000);
      }

      else if (cmd == 'C') {
        startIgnorePeriod(3000);
      }
    }
  }

  else {
    if (targetLocked && millis() - lastTargetTime < TARGET_HOLD_MS) {
      sendDataMega(lockedAngle, lockedDistances);
    }

    else {
      targetLocked = false;

      if (buzzerActive) {
        digitalWrite(buzzerPin, LOW);
        buzzerActive = false;
      }

      if (millis() - lastActionMs >= 20) {
        lastActionMs = millis();

        sweepAngle += sweepStep;

        if (sweepAngle >= SERVO_MAX) {
          sweepAngle = SERVO_MAX;
          sweepStep = -1;
        }

        if (sweepAngle <= SERVO_MIN) {
          sweepAngle = SERVO_MIN;
          sweepStep = 1;
        }

        s_front.write(sweepAngle);
        s_back.write(sweepAngle);
        s_right.write(sweepAngle);
        s_left.write(sweepAngle);

        int clearD[] = {400, 400, 400, 400};
        sendDataMega(sweepAngle, clearD);
      }

      releaseStepper();
    }
  }
}

void startIgnorePeriod(unsigned long duration) {
  isPaused = true;
  ignoreUntil = millis() + duration;

  targetLocked = false;

  digitalWrite(buzzerPin, LOW);
  buzzerActive = false;

  digitalWrite(laserPin, LOW);

  int neut[] = {400, 400, 400, 400};
  sendDataMega(sweepAngle, neut);
}

void followTarget(int targetAngle) {
  int angleDiff = targetAngle - currentStepperAngle;

  if (angleDiff > 180) angleDiff -= 360;
  if (angleDiff < -180) angleDiff += 360;

  if (abs(angleDiff) > 1) {
    long steps = (long)angleDiff * STEPS_PER_REV / 360;

    myStepper.step(steps);

    currentStepperAngle = targetAngle;
  }
}

void releaseStepper() {
  for (int i = 30; i <= 33; i++) {
    digitalWrite(i, LOW);
  }
}

void sendDataMega(int a, int d[]) {
  Serial.print(a);

  for (int i = 0; i < 4; i++) {
    Serial.print(",");
    Serial.print(d[i]);
  }

  Serial.println(".");
}

int getDistance(int t, int e) {
  digitalWrite(t, LOW);
  delayMicroseconds(2);

  digitalWrite(t, HIGH);
  delayMicroseconds(10);

  digitalWrite(t, LOW);

  long dur = pulseIn(e, HIGH, 12000);

  return (dur <= 0) ? 400 : dur * 0.034 / 2;
}
