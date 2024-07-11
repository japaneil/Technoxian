#include <QTRSensors.h>
QTRSensors qtr;
int Last = 0;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup()
{
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(2);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void loop()
{
   
  uint16_t position = qtr.readLineBlack(sensorValues);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);
  if(sensorValues[3] > qtr.calibrationOn.maximum[3] - 150 || sensorValues[4] > qtr.calibrationOn.maximum[4] - 150){
    analogWrite(3, 127);
    analogWrite(5, 0);
    analogWrite(6, 127);
    analogWrite(9, 0);
    Serial.print("FORWARD");
    Serial.print('\n');
  }
  else if(sensorValues[6] > qtr.calibrationOn.maximum[6] - 150 || sensorValues[7] > qtr.calibrationOn.maximum[7] - 150){
    analogWrite(3, 0);
    analogWrite(5, 0);
    analogWrite(6, 80);
    analogWrite(9, 0);
    Serial.print("RIGHT");
    Serial.print('\n');
    Last = 1;
  }
  else if(sensorValues[0] > qtr.calibrationOn.maximum[0] - 150 || sensorValues[1] > qtr.calibrationOn.maximum[1] - 150){
    analogWrite(3, 80);
    analogWrite(5, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);
    Serial.print("LEFT");
    Serial.print('\n');
    Last = 2;
  }
  else if(sensorValues[1] > qtr.calibrationOn.maximum[1] - 150 && sensorValues[2] > qtr.calibrationOn.maximum[2] - 150 && sensorValues[3] > qtr.calibrationOn.maximum[3] - 150 && sensorValues[4] > qtr.calibrationOn.maximum[4] - 150 && sensorValues[5] > qtr.calibrationOn.maximum[5] - 150 && sensorValues[6] > qtr.calibrationOn.maximum[6] - 150){
    analogWrite(3, 225);
    analogWrite(5, 0);
    analogWrite(6, 225);
    analogWrite(9, 0);
    Serial.print("FORWARD");
    Serial.print('\n');
  }
  else{
    if (Last == 1){
    analogWrite(3, 0);
    analogWrite(5, 0);
    analogWrite(6, 80);
    analogWrite(9, 0);
    Serial.print("RIGHT");
    Serial.print('\n');
    }
    if (Last == 2){
    analogWrite(3, 80);
    analogWrite(5, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);
    Serial.print("LEFT");
    Serial.print('\n');
    }
  }
}
