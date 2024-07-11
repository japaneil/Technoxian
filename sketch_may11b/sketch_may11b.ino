#include <QTRSensors.h>

// Define pins for QTR sensors
#define NUM_SENSORS   8
#define NUM_SAMPLES   4
#define EMITTER_PIN   2

// Define pins for motor driver
#define IN1 3
#define IN2 5
#define IN3 6
#define IN4 9

// PID parameters
#define KP 0.1
#define KI 0.01
#define KD 0.01

unsigned int sensorValues[NUM_SENSORS];
int lastError = 0;
int integral = 0;
int setpoint = 4000; // Setpoint value for sensor reading
int speed = 150; // Base speed for motors

QTRSensors qtr;

// Define sensor pin array
const uint8_t sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7};

void setup()
{
  Serial.begin(9600);

  // Set up the QTR sensors
  qtr.setTypeAnalog();
  qtr.setEmitterPin(EMITTER_PIN);
  qtr.setSensorPins(sensorPins, NUM_SENSORS);
  qtr.setTimeout(2500); // 2500 us (2.5 ms) max wait

  // Calibrate the sensors
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED during calibration
  for (int i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // Turn off the LED after calibration

  // Set up motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{
  // Read sensor values
  qtr.read(sensorValues);
  
  // Calculate position
  int position = qtr.readLineBlack(sensorValues);
  
  // PID control
  int error = position - setpoint;
  integral += error;
  int derivative = error - lastError;
  
  int pwmLeft = speed + KP * error + KI * integral + KD * derivative;
  int pwmRight = speed - KP * error - KI * integral - KD * derivative;
  
  // Limit PWM values to avoid overflow
  pwmLeft = constrain(pwmLeft, -255, 255);
  pwmRight = constrain(pwmRight, -255, 255);
  
  // Set motor speeds
  if (pwmLeft > 0) {
    analogWrite(IN1, pwmLeft);
    analogWrite(IN2, 0);
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, -pwmLeft);
  }
  
  if (pwmRight > 0) {
    analogWrite(IN3, pwmRight);
    analogWrite(IN4, 0);
  } else {
    analogWrite(IN3, 0);
    analogWrite(IN4, -pwmRight);
  }
  
  lastError = error;
  
  delay(10); // Delay to control update rate
}
