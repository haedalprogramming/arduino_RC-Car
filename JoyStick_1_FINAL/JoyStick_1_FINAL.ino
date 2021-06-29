#include <SoftwareSerial.h>

SoftwareSerial bluetooth(9, 8); //블루투스 TXD, RXD 핀

int initialX = 0; //조이스틱 X값 초기 변수
int initialY = 0; //조이스틱 Y값 초기 변수
int X = 0; //조이스틱의 X값
int Y = 0; //조이스틱의 Y값
int Z = 0; //조이스틱의 스위치 값

//조이스틱의 방향을 판별하기 위한 변수
int stateX = 0;
int stateY = 0;
int stateZ = 0;

char myString[4] = {' ', ' ', ' ', '\0'};

void cmd(const char *msg) { //시리얼통신으로 아두이노에 입력하게 해주는 함수
  bluetooth.write(msg); 
  unsigned long stime = millis();
  while((millis() - stime) < 1000) {
    if (bluetooth.available()) {
      Serial.write(bluetooth.read());
    }
  }
}

void setup() {
  Serial.begin(9600); //시리얼 통신 동기화
  bluetooth.begin(9600); //블루투스 통신 동기화
  pinMode(7, INPUT_PULLUP); //조이스틱 스위치 지정
  
  initialX = analogRead(A1); //조이스틱의 X초기값 설정
  initialY = analogRead(A0); //조이스틱의 Y초기값 설정

  cmd("AT+RESET"); //블루투스 설정 리셋
  cmd("AT+BAUD4"); //블루투스 통신 속도를 9600보드레이트로 설정 / 반응 : OK9600
  cmd("AT+PIN1234"); //블루투스 비밀번호 설정 (자신만의 번호로 설정하세요) / 반응 : OKsetPIN
  cmd("AT+NAMEJoyStick"); //블루투스 이름 설정 (자신만의 이름으로 설정하세요) / 반응 : OKsetname
  cmd("AT+ROLE=M"); //블루투스 역할 설정 / 반응 : OK+ROLE:M
  Serial.println("블루투스 초기화 완료");
}

void loop() {
  char sendMsg;
  X = readX(); //조이스틱의 X값을 받아옴
  Y = readY(); //조이스틱의 Y값을 받아옴
  Z = readZ(); //조이스틱의 스위치 값을 받아옴

  stateX = stateCheck(X); //X값에 따라 stateX 변수에 -1, 0, 1을 저장
  stateY = stateCheck(Y); //Y값에 따라 stateY 변수에 -1, 0, 1을 저장

  sendMsg = moveCheck(); //stateX와 stateY에 따라 방향을 판별하여 sendMsg에 저장

  Serial.println(sendMsg); //보낼 메시지를 시리얼 모니터에 출력

  bluetooth.println(sendMsg);
  delay(100);
}

int readX() {
  return analogRead(A1) - initialX;
}

int readY() {
  return initialY - analogRead(A0);
}

int readZ() {
  return digitalRead(7);
}

int stateCheck(int value) {
  if (value > 450) { //조이스틱의 값이 450보다 클 경우 메시지를 보냄
    return 1;
  } else if (value < -450) {
    return -1;
  } else return 0;
}

/*
 * RC카에 보낼 메시지
 * 
 * 전진 : W
 * 후진 : S
 * 좌 : A
 * 우 : D
 * 직좌 : Q
 * 직우 : E
 */

char moveCheck() {
  char result;
  if (Z == 0) {
    result = 'Z';
  }
  else if (stateX == 1) {
    if (stateY == 1) result = 'D';
    else if (stateY == 0) result = 'S';
    else if (stateY == -1) result = 'A';
  } 
  else if (stateX == 0) {
    if (stateY == 1) result = 'D';
    else if (stateY == 0) result = 'O';
    else if (stateY == -1) result = 'A';
  } 
  else if (stateX == -1) {
    if (stateY == 1) result = 'E';
    else if (stateY == 0) result = 'W';
    else if (stateY == -1) result = 'Q';
  }

  return result;
}