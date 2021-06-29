#include <SoftwareSerial.h>

SoftwareSerial bluetooth(7,8); //블루투스 TXD, RXD 핀

int in1 = 5; //A-2A
int in2 = 6; //A-1A
int in3 = 9; //B-1A
int in4 = 10; //B-2A

int buzzer = 12; //부저 핀 설정
int led = 2; //전원 led
int led_front = 3; //고휘도 LED 핀 설정
int light_sensor = A7; //광센서 핀 설정

#define c4 3822 // 261.63Hz 도
#define d4 3405 // 293.67Hz 레
#define e4 3034 // 329.63Hz 미
#define f4 2863 // 349.23Hz 파
#define g4 2551 // 392.11Hz 솔
#define g4s 2408 // 415.30Hz 솔#
#define a4 2273 // 440.11Hz 라
#define b4 2025 // 493.88Hz 시
#define c5 1910 // 523.25Hz 도
#define d5 1703 // 587.33Hz 레

#define d5s 1607 // 584.37Hz 레#
#define e5 1517 // 659.26Hz 미
#define f5 1432 // 698.46Hz 파
#define g5 1276 // 783.99Hz 솔
#define a5 1136 // 880.11Hz 라
#define b5 1012 // 987.77Hz 시

int song[] = { e5, d5s, e5, d5s, e5, b4, d5, c5, a4, c4, e4, a4, b4,
							 e4, g4s, b4, c5, e4, e5, d5s, e5, d5s, e5, b4, d5, c5,
							 a4, c4, e4, a4, b4, e4, c5, b4, a4 };

int song_pivot = 0; //노래 음계 배열을 참조하는 인덱스 값
unsigned long mil = 0; //시간 판별하는 변수

int state = 0; //모터 상태

void cmd(const char *msg){ //시리얼통신으로 아두이노에 입력하게 해주는 함수
  bluetooth.write(msg);
  unsigned long stime = millis();
  while((millis()-stime) < 1000){
    if(bluetooth.available()){
      Serial.write(bluetooth.read());
    }
  }
}

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

  //부저 핀 활성화
  pinMode(buzzer, OUTPUT);

  //고휘도 LED를 활성화
  pinMode(led_front, OUTPUT); 

  //전원 LED 활성화
  pinMode(led, OUTPUT);

  Stop(); //모터 정지
  digitalWrite(led, HIGH); // 전원 LED 켜기
}

void loop(){

  //조이스틱에서 보낸 블루투스값을 받아와서 모터를 제어하는 함수
  BluetoothInput();

  //후진 시 음악 재생
  Song();

  //광센서 값을 통해 고휘도 LED를 제어하는 함수
  front_light();
  
}

void Song(){
  if(state == 2){ //후진할 때
      Buzzer(); //부저 음악 울리기
    }
  else{
    song_pivot = 0; //노래 초기화
  }
}

/////////////////////고휘도 LED 제어

//어두운 정도를 판별하는 기준 변수
//센서에 따라 값이 다를 수 있으므로 이 변수 값을 조정하여 어두워질 때 불이 켜질 수 있도록 합니다.
int threshold = 700;

void front_light(){
  int val = analogRead(light_sensor); //광센서 값을 받아온다.
  Serial.println(val); //시리얼 모니터에 광센서 값 출력
  if(val >= threshold){ //광센서 값이 threshold보다 클 경우 (클수록 어두워진다.)
    analogWrite(led_front, val-threshold); //어두운 정도에 따라 밝게 출력한다.
  }
  else{
    digitalWrite(led_front, LOW); //threshold보다 작을 경우 고휘도 LED 불을 끈다.
  }
  
}

/////////////////////블루투스 제어
unsigned long b_time_prev = 0;
unsigned long b_time_curr = 0;

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
       case 'Z': // 경적
        Horn();
        break;
    }
  }
  
  else if(b_time_curr - b_time_prev > 1000){ //블루투스 끊겼을 때 모터 정지
    Stop();
  }
}


//////////////////////아래 모터제어
void FR() { //약한 우회전
  state = 1;
  analogWrite(in1, 0);
  analogWrite(in2, 180);
  analogWrite(in3, 0);
  analogWrite(in4, 100);
}

void FL() { //약한 좌회전
  state = 1;
  analogWrite(in1, 0);
  analogWrite(in2, 100);
  analogWrite(in3, 0);
  analogWrite(in4, 180);
}

void Forward(){ //직진
  state = 1;
  analogWrite(in1, 0);
  analogWrite(in2, 170);
  analogWrite(in3, 0);
  analogWrite(in4, 170);
}

void Backward(){ //후진
  state = 2;
  analogWrite(in1, 170);
  analogWrite(in2, 0);
  analogWrite(in3, 170);
  analogWrite(in4, 0);
}

void Right(){ //우회전
  state = 1;
  analogWrite(in1, 150);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 150);
}

void Left(){ //좌회전
  state = 1;
  analogWrite(in1, 0);
  analogWrite(in2, 150);
  analogWrite(in3, 150);
  analogWrite(in4, 0);
}

void Stop(){ //정지
  state = 1;
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
}

void Horn(){
    //부저 울리기
    tone(buzzer, b5, 500); //솔#
}

void Buzzer(){ //음악 재생
    if (millis() - mil > 100) {
      sing(song[song_pivot++]); //음계 배열 인덱스를 1더해서 재생
      mil = millis();
      if(song_pivot > 34){ // 음계 배열이 끝났을 때, 다시 0으로 만들어서 처음부터 재생
        song_pivot = 0;
      }
    }
}

void sing(int eum) {
  long lele = 125000;

  for(long i=0; i< lele ; i+= eum) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(eum/3);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(eum/3);
  }
}