#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
// sensor connected through analog pins A0 - A5 i.e. digital pins 14-19
// note: the next two lines are really one line of code – it word-wrappedQTRSensors qtrrc((unsigned char[]) {14, 15, 16, 17, 18, 19, 20, 21}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 

void setup()
{
   qtr.setTypeAnalog();
   qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
   qtr.setEmitterPin(2);
   Serial.begin(9600);
   delay(500);
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
   for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
   {
    qtr.calibrate();   // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
   }
   digitalWrite(13, LOW);  // turn off Arduino's LED to indicate we are through with calibration
   delay(2000);          //allow time to position the robot
}
void loop()
{
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = position - 2500;  // you may need to adjust this factor
  int errorMargin  = 100;       // you may need to adjust this factor

  Serial.print("Error: ");
  Serial.println(error);
  
  if(error > errorMargin)               
  {
    analogWrite(3, 80);
    analogWrite(5, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);
    Serial.print("LEFT");
    Serial.print('\n');
  }
  else if(error < -errorMargin)
  {
    analogWrite(3, 0);
    analogWrite(5, 0);
    analogWrite(6, 80);
    analogWrite(9, 0);
    Serial.print("RIGHT");
    Serial.print('\n');
  }

    analogWrite(3, 127);
    analogWrite(5, 0);
    analogWrite(6, 127);
    analogWrite(9, 0);
    Serial.print("FORWARD");
    Serial.print('\n');
  delay(50);
}
