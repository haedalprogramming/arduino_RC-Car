/int buzzer = 12; //부저 핀 설정

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

//노래의 음계 하나하나를 담은 배열
int song[] = { e5, d5s, e5, d5s, e5, b4, d5, c5, a4, c4, e4, a4, b4,
							 e4, g4s, b4, c5, e4, e5, d5s, e5, d5s, e5, b4, d5, c5,
							 a4, c4, e4, a4, b4, e4, c5, b4, a4 };

int song_pivot = 0; //노래 음계 배열을 참조하는 인덱스 값
unsigned long mil = 0; //시간 판별하는 변수

void setup(){
  pinMode(buzzer, OUTPUT);
}

void loop(){
  Buzzer(); //부저 음악 울리기
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