int in1 = 5; //A-2A
int in2 = 6; //A-1A
int in3 = 9; //B-1A
int in4 = 10; //B-2A

void setup(){
  Serial.begin(9600); //시리얼 통신 동기화

  //모터 핀 활성화
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Stop();
}

void loop(){

  Forward(); //전진
  delay(500);
  Stop(); //멈춤
  delay(500);
  Backward(); //후진
  delay(500);
  Stop(); //멈춤
  delay(500);
  Left(); //좌회전
  delay(500);
  Stop(); //멈춤
  delay(500);
  Right(); //우회전
  delay(500);
  Stop(); //멈춤
  delay(500);
}

//////////////////////모터제어 함수
void Forward(){
  analogWrite(in1, 0);
  analogWrite(in2, 170);
  analogWrite(in3, 0);
  analogWrite(in4, 170);
}

void Backward(){
  analogWrite(in1, 170);
  analogWrite(in2, 0);
  analogWrite(in3, 170);
  analogWrite(in4, 0);
}

void Right(){
  analogWrite(in1, 150);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 150);
}

void Left(){
  analogWrite(in1, 0);
  analogWrite(in2, 150);
  analogWrite(in3, 150);
  analogWrite(in4, 0);
}

void Stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
}