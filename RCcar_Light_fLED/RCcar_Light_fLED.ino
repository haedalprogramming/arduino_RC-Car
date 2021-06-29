int led_front = 3; //고휘도 LED 핀 설정

int light_sensor = A7; //광센서 핀 설정

void setup(){
  Serial.begin(9600); //시리얼 모니터 동기화

  pinMode(led_front, OUTPUT); //고휘도 LED를 활성화
}

void loop(){
  
  //광센서 값을 통해 고휘도 LED를 제어하는 함수
  front_light();
  
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