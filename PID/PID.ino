#include <QTRSensors.h>

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

float Kp = 12;
float Ki = 0;
float Kd = 0;

int P;
int I;
int D;
int lastError = 0;
int basespeed = 205;

// Motor pins
int in1 = 3; // Motor A input 1
int in2 = 5; // Motor A input 2
int in3 = 6; // Motor B input 1
int in4 = 9; // Motor B input 2

bool isBotStarted = false; // Renamed from startBot

void setup() {
  Serial.begin(9600);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(2); // LEDON PIN

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Turn off LED initially

  Serial.println("Ready for commands via Bluetooth.");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equals("calibrate")) {
      calibrateSensors();
    } else if (command.equals("start")) {
      startBot(); // Call the function startBot
    } else if (command.equals("stop")) {
      stopBot();
    } else if (command.startsWith("setKp=")) {
      float value = command.substring(6).toFloat();
      Kp = value;
      Serial.print("Kp set to ");
      Serial.println(Kp);
    } else if (command.startsWith("setKi=")) {
      float value = command.substring(6).toFloat();
      Ki = value;
      Serial.print("Ki set to ");
      Serial.println(Ki);
    } else if (command.startsWith("setKd=")) {
      float value = command.substring(6).toFloat();
      Kd = value;
      Serial.print("Kd set to ");
      Serial.println(Kd);
    } else {
      Serial.println("Invalid command.");
    }
  }

  if (isBotStarted) { // Check the boolean variable isBotStarted
    uint16_t position = qtr.readLineBlack(sensorValues);
    int error = 3500 - position;

    P = error;
    I = I + error;
    D = error - lastError;
    lastError = error;

    int motorspeed = P * Kp + I * Ki + D * Kd;
    int motorspeedA = basespeed + motorspeed;
    int motorspeedB = basespeed - motorspeed;

    if (motorspeedA > 255) {
      motorspeedA = 255;
    }
    if (motorspeedA < 0) {
      motorspeedA = 0;
    }
    if (motorspeedB > 255) {
      motorspeedB = 255;
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

void calibrateSensors() {
  digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate calibration mode
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // Turn off LED after calibration

  Serial.println("Calibration complete.");
}

void startBot() {
  isBotStarted = true; // Set the boolean variable to true
  Serial.println("Bot started.");
}

void stopBot() {
  isBotStarted = false; // Set the boolean variable to false
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  Serial.println("Bot stopped.");
}
