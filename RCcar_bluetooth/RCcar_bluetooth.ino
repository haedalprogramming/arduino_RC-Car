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

void setup() {
  Serial.begin(9600); //시리얼 통신 동기화
  bluetooth.begin(9600); //블루투스 통신 동기화
  
  
  cmd("AT+RESET"); //블루투스 설정 리셋
  cmd("AT+BAUD4"); //블루투스 통신 속도를 9600보드레이트로 설정 / 반응 : OK9600
  cmd("AT+PIN1234"); //블루투스 비밀번호 설정 (자신만의 번호로 설정하세요) / 반응 : OKsetPIN
  cmd("AT+NAMERCcar"); //블루투스 이름 설정 (자신만의 이름으로 설정하세요) / 반응 : OKsetname
  cmd("AT+ROLE=S"); //블루투스 역할 설정 / 반응 : OK+ROLE:S
  Serial.println("블루투스 초기화 완료");
}

void loop() {
  if (Serial.available()){bluetooth.write(Serial.read());} //추가
  else if (bluetooth.available()){Serial.write(bluetooth.read());} //추가
}