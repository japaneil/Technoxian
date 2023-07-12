//LINE FOLLOWER BOT
//For work with IR sensor array of size 5
//For work with L293D or L298N Motor Driver

int left_motor_forward = 2;
int left_motor_backward = 3;
int right_motor_forward = 4;
int right_motor_backward = 5;
int ir_pins[5]={A0,A1,A2,A3,A4};//initializing variables for pins connected to IR sensors
int ir_sensors[5];//initializing variables for taking input from 8 IR Sensor array
void setup(){
  //Serial.begin(9600);
  int pins;
  for(pins=0;pins<5;pins++){
  pinMode(ir_pins[pins],INPUT);//setting pins connected to IR sensors to INPUT mode
  }
  //setting pins connected to Motor Driver to OUTPUT mode
  pinMode(left_motor_forward,OUTPUT);
  pinMode(left_motor_backward,OUTPUT);
  pinMode(right_motor_forward,OUTPUT);
  pinMode(right_motor_backward,OUTPUT);
}
void goAhead(){
  analogWrite(left_motor_forward,255);
  analogWrite(right_motor_forward,255);
  digitalWrite(left_motor_backward,LOW);
  digitalWrite(right_motor_backward,LOW);
}

void turnLeft(){
  digitalWrite(left_motor_forward,LOW);
  analogWrite(right_motor_forward,255);
  analogWrite(left_motor_backward,255);
  digitalWrite(right_motor_backward,LOW);
}

void turnRight(){
  analogWrite(left_motor_forward,255);
  digitalWrite(right_motor_forward,LOW);
  digitalWrite(left_motor_backward,LOW);
  analogWrite(right_motor_backward,255);
}
/*void Stop(){
  digitalWrite(left_motor_forward,LOW);
  digitalWrite(right_motor_forward,LOW);
  digitalWrite(left_motor_backward,LOW);
  digitalWrite(right_motor_backward,LOW);
}*/
//Stop function is added for the help of user. He/she can use this to code to stop the bot at any specific condition if needed.
void goBack(){
  digitalWrite(left_motor_forward,LOW);
  digitalWrite(right_motor_forward,LOW);
  analogWrite(left_motor_backward,150);
  analogWrite(right_motor_backward,150);
}
void mediumLeft(){
  analogWrite(left_motor_forward,0);
  analogWrite(right_motor_forward,255);
  digitalWrite(left_motor_backward,LOW);
  digitalWrite(right_motor_backward,LOW);
}
void mediumRight(){
  analogWrite(left_motor_forward,255);
  analogWrite(right_motor_forward,0);
  digitalWrite(left_motor_backward,LOW);
  digitalWrite(right_motor_backward,LOW);
}
void loop(){
  int i;
  for(i=0;i<5;i++)
  {
    ir_sensors[i]=digitalRead(ir_pins[i]);//Taking input from the IR sensor array
    //Serial.println(ir_sensors[i]);
  }
  if((ir_sensors[0]==0 && ir_sensors[1]==0 && ir_sensors[3]==0 && ir_sensors[4]==0 && ir_sensors[2]==1))//robot is on the black line,so nothing to do,robot goes ahead
  {
      goAhead();
  }
  else 
  {
    int s;
    s=(5*ir_sensors[0] + 3*ir_sensors[1] -5*ir_sensors[4] - 3*ir_sensors[3]);//Here,we have done this to achieve better control and make full use of IR sensor array of size 5
    if(s==0)//all the sensors are sensing same color(i.e, either white or black).In this case we move the robot backwards with medium speed. 
    {
       goBack();
    }
   if(s>2 && s<=4)//robot is on right of the black line,hence leftwards movement
    {
       mediumLeft();
    }
    if(s<-2 && s>=-4)//robot is on left of the black line,hence rightwards movement
    {
       mediumRight();
    }
    if(s>4)//robot is completely on the right side of black line,hence a strong leftwards movement
    {
       turnLeft();
    }
    if(s<-4)//robot is completely on the left side of black line,hence a strong rightwards movement
    {
       turnRight();
    }
  }
}
