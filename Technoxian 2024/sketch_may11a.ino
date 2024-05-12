void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  analogWrite(3, 70);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 70);
  delay(1000);
  analogWrite(3, 0);
  analogWrite(5, 70);
  analogWrite(6, 70);
  analogWrite(9, 0);
}
