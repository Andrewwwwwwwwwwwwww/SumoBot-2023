'''
This is the code for the sumobot with the xMotion board (you know, the real heavy one with the white board?)
All the movement is hardcoded, meaning if using a different battery or motors, the time/power values would 
probably have to be altered.
'''

#include <xmotion.h>


#define Start = 10; //pin for start

int QTI[] = {A4, A2, A1};
uint8_t QTIvalue[] = {0, 0, 0}; //0 = black, 1 = white

#define lineThreshhold 200

uint8_t IR[] = {A5, 4, 2, 1, 0}; //pinouts
uint8_t IRvalue[] = {0, 0, 0, 0, 1}; //might be 00001 or 10000 since the robots start next to each other
uint8_t LastValue = 0;

uint32_t currentTime;
uint32_t TOL = 0; //TimeOfLastLineDetection
#define timeBetweenLineRecognition 250
bool timePassed = true; //has the timeBetweenLineRecognition passed since last line was detected?

#define DipSwitch1 5 // Dipswitch 1 tied to Digital 5, controls initial movement
#define DipSwitch2 6 // Dipswitch 2 tied to Digital 6, nothing at this stage
#define DipSwitch3 7 // Dipswitch 3 tied to Digital 7, controls timer (plan)

void setup() {
   pinMode(DipSwitch1, INPUT);pinMode(DipSwitch2, INPUT);pinMode(DipSwitch3, INPUT); //Dipswitch 1,2,3 Declared as Input
   digitalWrite(DipSwitch1, HIGH); //############ i honestly forgot what this is..... ######################################################################################
   pinMode(A4,INPUT);pinMode(A2,INPUT);pinMode(A1,INPUT); //QTI pins
   pinMode(A5, INPUT);pinMode(4, INPUT);pinMode(2, INPUT);pinMode(1, INPUT);pinMode(0, INPUT); //IR pins
}



void loop() {
  while (digitalRead(Start) == 0) // Button push is waited. When Button is pushed, it gives 1 value.
  {
    xmotion.UserLed2(50); // Blink User Led 2 50ms
  }
  xmotion.CounterLeds(990, 5); //4950ms timer
  if (digitalRead(DipSwitch1) == 0){
    xmotion.Left0(100,140);xmotion.Right0(100,10);xmotion.StopMotors(1);
  }
  while (true){
  currentTime = millis();
  QTICheck();
  IRCheck();
  EdgeDetected();
  Movement(QTIvalue[0],QTIvalue[1],QTIvalue[2]);
  if (QTIvalue[0] == 0 && QTIvalue[1] == 0 && QTIvalue[2] == 0){
    OpponentTracking(IRvalue[0], IRvalue[1], IRvalue[2], IRvalue[3], IRvalue[4]);
 }
  }
}



void QTICheck(){
  for (int i = 0; i < 3; i++){
    if ((analogRead(QTI[i])) <= lineThreshhold){
      QTIvalue[i] = 1;
    } else {
      QTIvalue[i] = 0;
    }
  }
}


void EdgeDetected(){
  if ((QTIvalue[0] == 1)||(QTIvalue[1] == 1)||(QTIvalue[2] == 1)){ 
    TOL = millis();
    xmotion.Backward(100,90);
  }
}


void Movement(int left_line_sensor, int middle_line_sensor, int right_line_sensor){
  if (timePassed == true){
    if ((left_line_sensor == 1) && (middle_line_sensor == 1) && (right_line_sensor == 1)){
      xmotion.Backward(100,100);xmotion.Forward(20,20);
      xmotion.Right0(100,160);xmotion.Left0(20,10); //does an approx. 180 degrees
    }
    else if ((left_line_sensor == 1) && (middle_line_sensor == 0) && (right_line_sensor == 1)){
      xmotion.Backward(100,100);xmotion.Forward(20,20);
      xmotion.Right0(100,160);xmotion.Left0(20,10); //does an approx. 180 degrees
    }
    else if ((left_line_sensor == 1) && (middle_line_sensor == 0) && (right_line_sensor == 0)){
      xmotion.Right0(100,190);xmotion.Left0(100,20);
    }
    else if ((left_line_sensor == 0) && (middle_line_sensor == 0) && (right_line_sensor == 1)){
      xmotion.Left0(100,190);xmotion.Right0(100,20);
    }
    else if ((left_line_sensor == 1) && (middle_line_sensor == 1) && (right_line_sensor == 0)){
      xmotion.Right0(100,190);xmotion.Left0(100,20);
    }
    else if ((left_line_sensor == 0) && (middle_line_sensor == 1) && (right_line_sensor == 1)){
      xmotion.Left0(100,190);xmotion.Right0(100,20);
    }
    else if ((left_line_sensor == 0) && (middle_line_sensor == 1) && (right_line_sensor == 0)){
      xmotion.Backward(100,100);xmotion.Forward(100,20);
      xmotion.Right0(100,160);
    }
    xmotion.StopMotors(1);
    timePassed = false;
  }
  else if ((currentTime - TOL) >= timeBetweenLineRecognition){
    timePassed = true;
  }
}

void IRCheck(){
  for (int i = 0; i < 5; i++){
    IRvalue[i] = digitalRead(IR[i]);
    }
}

void OpponentTracking(int LeftSensor, int LeftDiagonalSensor, int MiddleSensor, int RightDiagonalSensor, int RightSensor){
    if (MiddleSensor == 1 ) //Middle Sensor see the opponent (0 Not Seen, 1 Seen)
    {
      xmotion.Forward(100, 2); // Both Motors Forward %100 Speed, 1 Milliseconds
      LastValue = 0;
    }
    else if (RightSensor == 1) //Right Sensor see the opponent
    {
      xmotion.Right0(70, 1); // Right Turn %70 Power 1 Milliseconds
      LastValue = 1;
    }
    else if (LeftSensor == 1) //Left Sensor see the opponent
    {
      xmotion.Left0(70, 1); // Left Turn %70 Power 1 Milliseconds
      LastValue = 2;
    }
    else if (LeftDiagonalSensor == 1) //Left Diagonal Sensor see the opponent
    {
      xmotion.ArcTurn(20, 70, 1); // Left Motor %20 Speed, Right %70 Speed 1 ms.
      LastValue = 2;
    }
    else if (RightDiagonalSensor == 1) //Right Diagonal Sensor see the opponent
    {
      xmotion.ArcTurn(70, 20, 1); // Left Motor %70 Speed, Right %20 Speed 1 ms.
      LastValue = 1;
    } else if (LastValue == 0) { //Middle Sensor Saw the opponent
      xmotion.Forward(35, 1);
    } else if (LastValue == 1) { //Right or Right Diagonal Sensor saw the opponent
      xmotion.Right0(30, 1);
    } else if (LastValue == 2) { //Left or Left Diagonal Sensor saw the opponent.
      xmotion.Left0(30, 1);
    }
}