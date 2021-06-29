int initialX = 0; //조이스틱 X값 초기 변수
int initialY = 0; //조이스틱 Y값 초기 변수
int X = 0; //조이스틱의 X값
int Y = 0; //조이스틱의 Y값
int Z = 0; //조이스틱의 스위치 값

//조이스틱의 방향을 판별하기 위한 변수
int stateX = 0;
int stateY = 0;
int stateZ = 0;

void setup() {
  Serial.begin(9600); //시리얼 통신 동기화
  pinMode(7, INPUT_PULLUP); //조이스틱의 스위치 설정
  initialX = analogRead(A1); //조이스틱의 X값 초기값 설정
  initialY = analogRead(A0); //조이스틱의 Y값 초기값 설정
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
  //조이스틱의 값이 450보다 클 경우 메시지를 보냄
  if (value > 450) { 
    return 1;
  } 
	else if (value < -450) {
    return -1;
  } 
	else return 0;
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