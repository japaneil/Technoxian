#include <QTRSensors.h>

// QTRSensors
QTRSensors qtr;

// Number of sensors to use
#define IR 8
unsigned short qtrValues[IR];

#define Kp 0.1     // PID constants (adjust as needed)
#define Ki 0.05
#define Kd 0.003
#define rightMaxSpeed 255
#define leftMaxSpeed  255

int SetPoint = (IR - 1) * 500;  // Desired line position

// Motor pins
int IN1 = 3; // Motor A input 1
int IN2 = 5; // Motor A input 2
int IN3 = 6; // Motor B input 1
int IN4 = 9; // Motor B input 2

// PID variables
int lastError = 0;
unsigned long cTime, pTime;
float eTime;
float P_error;
float I_error;
float D_error;
float PID_value;

void setup() {
  Serial.begin(9600);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, IR); // Use analog pins for sensors
  qtr.setEmitterPin(12); // Emitter pin for QTRSensors

  // Calibrate QTR sensors
  Serial.println("Calibrating sensors...");
  for (uint8_t i = 0; i < 250; i++) {
    if (i % 5 == 0) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    qtr.calibrate();
    delay(20);
  }
  Serial.println("Calibration complete.");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);  // Use the built-in LED for debugging
  digitalWrite(LED_BUILTIN, LOW);
}

void run_fwd(int valueSA, int valueSB) {
  // Motor A
  analogWrite(IN1, valueSA);
  analogWrite(IN2, 0);
  // Motor B
  analogWrite(IN3, valueSB);
  analogWrite(IN4, 0);
}

void LEDs() {
  if (abs(P_error) < 150) digitalWrite(LED_BUILTIN, HIGH);
  else digitalWrite(LED_BUILTIN, LOW);
}

void Run_robot() {
  int position = qtr.readLineWhite(qtrValues); // Read line position
  P_error = position - SetPoint;
  cTime = millis();
  eTime = (float)(cTime - pTime) / 1000.0;
  I_error = I_error * 2 / 3 + P_error * eTime;
  D_error = (P_error - lastError) / eTime;
  PID_value = Kp * P_error + Ki * I_error + Kd * D_error;

  lastError = P_error;
  pTime = cTime;

  int med_Speed_L = leftMaxSpeed - abs(PID_value);
  int med_Speed_R = rightMaxSpeed - abs(PID_value);
  int leftMotorSpeed = med_Speed_L + PID_value;
  int rightMotorSpeed = med_Speed_R - PID_value;

  leftMotorSpeed = constrain(leftMotorSpeed, 0, leftMaxSpeed);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, rightMaxSpeed);

  run_fwd(leftMotorSpeed, rightMotorSpeed);

  delayMicroseconds(140);
}

void loop() {
  Run_robot();
  LEDs();
}
