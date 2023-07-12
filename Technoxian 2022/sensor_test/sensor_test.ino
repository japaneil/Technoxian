const int s1 = A5;
const int s2 = A0;
const int s3 = 8;
const int s4 = A4;
const int s5 = A3;

int colors3 = 0;


void setup() {
   
    pinMode(s3, INPUT);

  
    Serial.begin(9600);
}



void loop() {

    colors3 = digitalRead(s3);



    Serial.print("Middle Sensor: ");
    Serial.println(colors3);


    Serial.println("");


    delay(1000);
}
