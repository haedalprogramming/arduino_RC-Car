#include <SoftwareSerial.h>

SoftwareSerial bluetooth(7, 8); //블루투스 TXD, RXD 핀

void cmd(const char *msg) { //시리얼통신으로 아두이노에 입력하게 해주는 함수
  bluetooth.write(msg);
  unsigned long stime = millis();
  while((millis() - stime) < 1000) {
    if (bluetooth.available()) {
      Serial.write(bluetooth.read());
    }
  }
}

int in1 = 5; //A-2A
int in2 = 6; //A-1A
int in3 = 9; //B-1A
int in4 = 10; //B-2A

unsigned long b_time_prev = 0; //블루투스가 끊겼는지 판별하는 변수
unsigned long b_time_curr = 0; //블루투스가 끊겼는지 판별하는 변수

void setup(){
  Serial.begin(9600); //시리얼 통신 동기화
  bluetooth.begin(9600); //블루투스 통신 동기화

  cmd("AT+RESET"); //블루투스 설정 리셋
  cmd("AT+BAUD4"); //블루투스 통신 속도를 9600보드레이트로 설정 / 반응 : OK9600
  cmd("AT+PIN1234"); //블루투스 비밀번호 설정 (자신만의 번호로 설정하세요) / 반응 : OKsetPIN
  cmd("AT+NAMERCcar"); //블루투스 이름 설정 (자신만의 이름으로 설정하세요) / 반응 : OKsetname
  cmd("AT+ROLE=S"); //블루투스 역할 설정 / 반응 : OK+ROLE:S
  Serial.println("블루투스 초기화 완료");
  
  //모터 핀 활성화
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  Stop(); //모터 정지
}

void loop(){
  BluetoothInput(); //조이스틱에서 보낸 블루투스값을 받아와서 모터를 제어하는 함수
}



void BluetoothInput(){
  b_time_curr = millis();
  if(bluetooth.available()){ //블루투스가 연결되었을 때
    b_time_prev = b_time_curr;
    char bt = bluetooth.read(); //조이스틱에서 보낸 블루투스 값을 받아옴
    switch (bt) {
      case 'O': // 정지
        Stop();
        break;
      case 'Q': // 약한 좌회전
        FR();
        break;
      case 'W': // 직진
        Forward();
        break;
      case 'E': // 약한 우회전
        FL();
        break;
      case 'A': // 좌회전
        Left();
        break;
      case 'S': // 후진
        Backward();
        break;
      case 'D': // 우회전
        Right();
        break;
    }
  }
  
  else if(b_time_curr - b_time_prev > 1000){ //블루투스 끊겼을 때 모터 정지
    Stop();
  }
}

//////////////////////모터제어 함수
void FR() { //약한 우회전
  analogWrite(in1, 0);
  analogWrite(in2, 220);
  analogWrite(in3, 0);
  analogWrite(in4, 120);
}

void FL() { //약한 좌회전
  analogWrite(in1, 0);
  analogWrite(in2, 120);
  analogWrite(in3, 0);
  analogWrite(in4, 220);
}

void Forward(){ //직진
  analogWrite(in1, 0);
  analogWrite(in2, 200);
  analogWrite(in3, 0);
  analogWrite(in4, 200);
}

void Backward(){ //후진
  analogWrite(in1, 200);
  analogWrite(in2, 0);
  analogWrite(in3, 200);
  analogWrite(in4, 0);
}

void Right(){ //우회전
  analogWrite(in1, 180);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 180);
}

void Left(){ //좌회전
  analogWrite(in1, 0);
  analogWrite(in2, 180);
  analogWrite(in3, 180);
  analogWrite(in4, 0);
}

void Stop(){ //정지
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
}