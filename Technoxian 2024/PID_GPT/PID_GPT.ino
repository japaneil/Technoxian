#include <QTRSensors.h>

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

float Kp = 11.5;
float Ki = 0;
float Kd = 0;

int P;
int I;
int D;
int lastError = 0;
int Last = 0;

// Set up the motor pins
const uint8_t maxspeed = 255; // Max speed for L298N
const uint8_t basespeed = 205;

int in1 = 3; // Motor A input 1
int in2 = 5; // Motor A input 2
int in3 = 6; // Motor B input 1
int in4 = 9; // Motor B input 2

void setup() {
  Serial.begin(9600);
  qtr.setTypeAnalog();
  // Set up the sensor array pins
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(2); // LEDON PIN

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);

  // Print calibration values
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void loop() {
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = 3500 - position;

  P = error;
  I = I + error;
  D = error - lastError;
  lastError = error;

  int motorspeed = P * Kp + I * Ki + D * Kd;
  int motorspeedA = basespeed + motorspeed;
  int motorspeedB = basespeed - motorspeed;

  if (motorspeedA > maxspeed) {
    motorspeedA = maxspeed;
  }
  if (motorspeedA < 0) {
    motorspeedA = 0;
  }
  if (motorspeedB > maxspeed) {
    motorspeedB = maxspeed;
  }
  if (motorspeedB < 0) {
    motorspeedB = 0;
  }

  forward_brake(motorspeedA, motorspeedB);

  // Print sensor values and position for debugging
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);
}

void forward_brake(int speedA, int speedB) {
  if (speedA > 0) {
    analogWrite(in1, speedA);
    analogWrite(in2, 0);
  } else {
    analogWrite(in1, 0);
    analogWrite(in2, -speedA);
  }

  if (speedB > 0) {
    analogWrite(in3, speedB);
    analogWrite(in4, 0);
  } else {
    analogWrite(in3, 0);
    analogWrite(in4, -speedB);
  }
}
