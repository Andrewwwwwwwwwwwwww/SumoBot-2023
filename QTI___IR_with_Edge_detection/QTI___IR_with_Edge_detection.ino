int QTI[] = {A4, A2, A1};
int QTIvalue[] = {0, 0, 0}; //0 = black, 1 = white

int lineThreshhold = 200;


int IR[] = {A5, 4, 2, 1, 0};
int IRvalue[] = {0, 0, 0, 0, 0,}; //might be 00001 or 10000 since the robots start next to each other


void setup() {
   Serial.begin(9600);
   pinMode(A4,INPUT);pinMode(A2,INPUT);pinMode(A1,INPUT); //QTI pins
   pinMode(A5, INPUT);pinMode(4, INPUT);pinMode(2, INPUT);pinMode(1, INPUT);pinMode(0, INPUT); //IR pins
}



void loop() {
  QTICheck();
  IRCheck();
  
//  Serial.print(QTIvalue[0]);
//  Serial.print(QTIvalue[1]);
//  Serial.println(QTIvalue[2]);

//  Serial.print(IRvalue[0]);
//  Serial.print(IRvalue[1]);
//  Serial.print(IRvalue[2]);
//  Serial.print(IRvalue[3]);
//  Serial.println(IRvalue[4]);

  if (EdgeDetected()){
    Movement();
  } else {
    Serial.println("nothing detected");
  }
}



void QTICheck(){
  for (int i = 0; i < 3; i++){
    if ((analogRead(QTI[i])) <= lineThreshhold){
      QTIvalue[i] = 1;
      //Serial.print(analogRead(QTI[i])); //gives its raw data
    } else {
      QTIvalue[i] = 0;
    }
    //Serial.print(i);
  }
}

bool EdgeDetected(){
  if ((QTIvalue[0] + QTIvalue[1] + QTIvalue[2]) == 0){ //i can't find the "or"(two vertical bars) (cry emoji)
    return false;
  } else {
    return true;
  }
}

void Movement(){
  if ((QTIvalue[0] == 1) && (QTIvalue[1] == 1) && (QTIvalue[2] == 1)){
    //back out
    Serial.println("back out(3)");
  } else {
    
    if (QTIvalue[0] == 1){
      switch (QTIvalue[2]){
        case 0:
          //turn right
          Serial.println("right");
          break;
        case 1:
          //back out
          Serial.println("180 turn");
          break;
      }
    } else {
    if (QTIvalue[2] == 1){
      //turn left
      Serial.print("left");
    }
  }
  }
}

void IRCheck(){
  for (int i = 0; i < 5; i++){
    IRvalue[i] = digitalRead(IR[i]);
    }
}
