/*
^@배선도  , 23-03-16 배선 확정함
Hardware  ||  External Vin   ||  DHT22( 5V , 3.3V 모두 가능)
                      3.3V   ||   VCC
                      GND    ||   GND
                     GPIO 2  ||   DATA

Hardware  ||  External Vin   ||  OLED 128x64 SSD1306 ( 5V , 3.3V 모두 가능)
                      3.3V   ||   VCC
                      GND    ||   GND
                      GP4    ||   SDA
                      GP5    ||   SCL

Hardware  ||  External Vin   ||  SDCARD @ YUROBOT ( 3.3V  , 5V 가능 )
                      3.3V   ||   VCC
                      GND    ||   GND
                    GPIO 17  ||   CS
                    GPIO 19  ||   MOSI
                    GPIO 16  ||   MISO
                    GPIO 18  ||   SCK

Hardware  ||  external Vin   ||  DS3231RTC ( 5V , 3.3V 모두 가능)
                      3.3V   ||   VCC
                      GND    ||   GND
                      GP4    ||   SDA
                      GP5    ||   SCL

Hardware  ||  external Vin   ||  RotaryEncorder ( 5V , 3.3V 모두 가능)
                      3.3V   ||   VCC
                      GND    ||   GND
                      GP13   ||   SW ( 내부풀업)
                      GP14   ||   DT ( 내부풀업)
                      GP15   ||   CLK ( 내부풀업)

^@부품 : DHT22, OLED, SD카드, DS3231RTC, RotaryEncorder , 브레드보드용 전원 공급 보드
  - DS3231 RTC 상세 설명 : https://components101.com/modules/ds3231-rtc-module-pinout-circuit-datasheet
  - YUROBOT SDCARD 상세 설명 : https://www.devicemart.co.kr/goods/view?no=1311842
^@보드 : raspberry pi pico( no wifi version)
^@기능 : 온습도, 시간, 날짜, SD카드에 저장, RotaryEncorder로 메뉴 선택
^@베이스 코드 : arduino nano 로 만든 008 버전
!@주의사항 :  파이피코 보드에서 전원 끌어다 쓰지 말것 ( 브레드보드용 전원 공급 보드 사용할것 )
^@원가 분석( 23.03.11 기준 , 네이버 쇼핑)
  - DHT22 : 3,500원 (PCB 형태)
  - OLED : 3,000원
  - SDCARD@YUROBOT : 2,000원
  - DS3231RTC : 3,000원
  - RotaryEncorder : 700원
  - TTL LEVEL CONVERTER 5V <-> 3.3V(4채널기준 ) : 500원
  - 브레드보드용 전원 공급 보드 : 3,000원
  - raspberry pi pico( no wifi version) : 5,000원
  - 총합 : 20,700원

^@피코보드 업로드 방법
  https://blog.naver.com/PostView.naver?blogId=jcosmoss&logNo=222457510932&categoryNo=0&parentCategoryNo=56&currentPage=1
  https://cafe.naver.com/dgarduino/14716

  ^ 최초 업로드 하는 동안만 계속 부트버튼 누르면 되고 wrote ~ 라는 빨강색 메세지가 뜨면 부트버튼 떼도 된다.
    아두이노 연결 usb를 뺐다 다시 끼고 포트선택란에 보면 파이피코 com 번호가 보일 것이다 .선택
    이젠 부트 버튼 안 눌러도 업로드가 가능해진다.

^@진행 사항(글이 길어질경우 맨 아래로 이동)
  - 2023.03.11 : 001 버전 시작
  - 2023.03.12 : 로터리 엔코더 기초 코드 공부
  - 2023.03.12 #2 : 파이피코 전원 공급 방법 공부 ( 왠만하면  GPIO 40에 연결하지 말것 )
                    VSYS(GPIO 39) 에 다이오드로 연결해서 외부전원공급할것 최소 2볼트 필요
                    이렇게하면 코딩도 가능하고 업로드도 가능하다. - 이건 유튜브 내용잘살펴볼것
                    36번핀  3.3V OUTPUT MAX 300mA 출력이 가능하다(공식문서 참고)
  - 2023.03.16 : pico 보드와 개별 모듈 연결 및 테스트
  - 2023.03.16 #1 : SD카드 동작 확인함 , 새로운 파일이 있으면 +1을 생성해서 겹치지 않는 것 확인함
  - 2023.03.16 #2 : SD카드 로그파일 생성시 현재날짜 생성되도록 수정함
                    - 관련 힌트는  1) C:\Users\User\Documents\Arduino\libraries\SD\src\utility\SdFile.cpp 와
                                  2) C:\Users\User\Documents\Arduino\libraries\RP2040_SD\src\utility\SdFile.cpp
                                  를 비교 및 검색해서 callback 함수를 찾아서 수정된 부분만 코드에 적용함
  - 2023.03.17 #1 : DHT 동작 및 SD 기록 확인함
  - 2023.03.17 #2 : 로터리 엔코더 동작 확인함
  - 2023.03.17 #3 : u8g2 풀버퍼 코드의 첫 ~ 끝 실행까지 지연이 약 40ms 생기면서 나머지 변수에 의한 동작을
                    제어하는 코드들에 영향을 주고 있다. 이를 해결 하려면 로터리 부분 코드를 인터럽트로 바꿔야 한다.
                    나와 같은 고민을 이미 하신 분이 있어 링크 : https://m.blog.naver.com/ysahn2k/221865584318
                    u8x8 모드로 할경우 3줄의 글자줄을 쓰는데 15ms이 걸린다.
                    firstPage() 모드로 할 경우 풀버퍼 모드랑 비슷하게 40ms의 지연이 생긴다.
  - 2023.03.19 #1 : 로터리 엔코더 + u8g2( full buffer , page buffer , u8x8 ) 동작에서 문제가 있음을 발견함
                    해외 포럼에 이에 대한 토론이 있었으나 질문자는 펄스 카운팅으로 회피했다고 하는데 이걸 ???
  - 2023.03.19 #2 : adafruit ssd1306 라이브러리                    


^@참고 사이트
- 파이피코 변형보드 만드신분  : https://sira-engineer.tistory.com/6?category=966430
- 파이피코 핀맵 깔끔한 거 : https://blog.naver.com/yunc26/222590881823
- 로터리엔코더 사용법 안내 및코드 : https://cafe.naver.com/dgarduino/9173
- 파이피코 전원 공급에 대한 안내 : https://remnant24c.tistory.com/268
- 파이피코 전원 공급 안내 유튜브 (해외 ) : https://youtu.be/3PH9jzRsb5E
- 파이피코 상제 내역 블로그 ( 한글 ) : https://fishpoint.tistory.com/7369
- 파이피코 제원 상세 : https://www.etechnophiles.com/raspberry-pi-pico-pinout-specifications-datasheet-in-detail/
- 파이피코 + 로터리엔더코 기초코드 : https://cafe.naver.com/dgarduino/9173
- 타이머 인터럽트 설명 : https://blog.naver.com/yunc26/222577012494
- mbed 용 타이머 인터럽트 : https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt
- 엔코더 값 읽는 방법 3가지에 대한 장단점 : https://m.blog.naver.com/i2asys/220863186925
                                          (인터럽트 )  https://m.blog.naver.com/i2asys/220864716104
- 엔코더 분석 : https://m.blog.naver.com/kimchulki1/220011691097
- 타이머 / 카운터 에 대한 심층 : https://recall.tistory.com/30
- pico i2c 핀번호 변경하는 방법 
  pico 보드에 보면 i2c0 핀들이 있습니다. 그럼 setup() {  } 안에 아래처럼 하면됩니다.

  #include <Wire.h> 필요 라이브러리 불러와주고 

   Wire.setSDA(8);
  Wire.setSCL(9);
  Wire.begin();

  i2c(1) 핀들은 setup() {  } 안에 아래처럼 하면됩니다.
  Wire1.setSDA(6);
  Wire1.setSCL(7);
  Wire1.begin();

  !주의사항 !  혹시 i2c(0) 핀들에  i2c(1)의 Wire1.setSDA ~ 코드를 넣으면 PICO 먹통됩니다.
  ^이때는 상단의 글 안내대로 PICO 보드 다시 살리면 됩니다. 

^@ 중요변경 사항
- U8glib -> U8g2 로 변경 ( 002 버전 code부터 적용 )
- simple dht 라이브러리 -> DHT22 라이브러리 ( 002 버전 code부터 적용 )
- 아두이노 SD 라이브러리 -> RP2040_SD 라이브러리 ( 002 버전 code부터 적용 )
- setup() 과  loop() 의 위치를 맨 아래로 수정함 ( 002 버전 code부터 적용 )
- u8g2 라이브러와 인터럽트 충돌로 도저히 방법을 못 찾아 adafruit ssd1306 라이브러리로 변경함 ( 003 버전 code부터 적용 )



^@라이센스 정보
기본 MIT 라이센스 적용이 되나 , 각 라이브러리 , 참고한 코드 출처에서 라이센스 정보를 확인해야 한다.
코드 맨 아래 쓰인 라이브러리 링크 및 라이센스 정보를 확인하면 됩니다.
*/

#include <RTClib.h>
#include <U8g2lib.h>
#include <DHT22.h>
#include <RP2040_SD.h>
#include <SPI.h> // SD카드 사용을 위한 SPI 라이브러리

enum State
{
  INTIALIZE,
  DISPLAY_LOGO,
  CHECK_SYSTEM,
  READ_DHT22,
  WRITE_SD,
  DISPLAY_ROOT_MENU,
  DISPLAY_SUB_01_MENU,
  DISPLAY_SUB_SUB_01_MENU
};
State state = INTIALIZE;

// 변수등 선언 순서는  DS3231 RTC, OLED , SD카드 , DHT22 , RotaryEncorder , millis() 순서로 한다.

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); // 잘 보임

File myFile;
const int chipSelect = 17; // SD카드 CS 핀을 Pico GPIO17에 연결
String fileName = "tata";  // 파일명
int fileNumber = 0;        // 파일번호

const int dht22_dataPin = 2; // DHT22 data pin to Pico GPIO2
DHT22 dht22(dht22_dataPin);

const int rotery_CLK = 15;         // CLK pin to Pico GPIO 15
const int rotery_DT = 14;          // DT pin to Pico GPIO 14
const int rotery_SW = 13;          // SW pin to Pico GPIO 13
volatile int counter = 0;          // 회전 카운터 측정용 변수 , 인터럽트 때문에 volatile 선언
int currentStateCLK;               // CLK의 현재 신호상태 저장용 변수
int lastStateCLK;                  // 직전 CLK의 신호상태 저장용 변수
String currentDir = "";            // 현재 회전 방향 출력용 문자열 저장 변수
unsigned long lastButtonPress = 0; // 버튼 눌림 상태 확인용 변수

unsigned long previousMillis = 0; // millis()를 위한 변수
const int interval = 2000;        // 2초마다 실행

void dateTime(uint16_t *date, uint16_t *time) // setup () dateTime 이라는 함수가 쓰일 예정이므로 일찍??? 선언 하는건가?
{
  DateTime now = rtc.now();
  *date = FAT_DATE(now.year(), now.month(), now.day());
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

void setup_SDcard() // ########
{
  // sd카드 존재 여부 체크
  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card not found");
    return;
    // oled 쪽에도 표현하는 걸 추가해야할 수도 있다.
  }

  // 파일이 존재하는지 확인
  while (SD.exists(fileName + String(fileNumber) + ".txt"))
  {
    fileNumber++;
  }
}

void setup_RTC() // ########
{
  // RTC 연결 체크
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) // RTC가 전원을 잃었는지 확인
  {
    Serial.println("RTC lost power, lets set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    //  following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    // rtc.adjust(DateTime(2023, 2, 13, 20, 7, 0));  // 수동으로 현재 시간을 맞출때   2023년 , 2월 , 13일 , 20시 , 07분 , 00초
  }
}

void write_SDcard(DateTime now) // ########
{
  // 파일 생성 및 열기
  myFile = SD.open(fileName + String(fileNumber) + ".txt", FILE_WRITE);

  // 파일 열기에 성공했다면 데이터 쓰기
  if (myFile)
  {
    Serial.print(F("Writing to tadaX.txt..."));
    myFile.print(now.year());
    myFile.print('/');
    myFile.print(now.month());
    myFile.print('/');
    myFile.print(now.day());
    myFile.print(" (");
    myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    myFile.print(") ");
    myFile.print(now.hour());
    myFile.print(':');
    myFile.print(now.minute());
    myFile.print(':');
    myFile.print(now.second());
    myFile.print(" , ");
    myFile.print(dht22.getTemperature());
    myFile.print(" , ");
    myFile.println(dht22.getHumidity());
    // close the file:
    myFile.close();
    Serial.println(F("done."));
    Serial.print(fileName);
    Serial.print(F("  "));
    Serial.println(fileNumber);
  }
  else
  {
    // 파일 열기 실패
    Serial.println("error opening tadaX.txt");
  }
}

void read_DHT22() // ########
{
  float temperature = dht22.getTemperature();
  float humidity = dht22.getHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void rotery_interrunt()
{
  // 코드 출처 : https://m.blog.naver.com/emperonics/222108739792
  // CLK의 현재 상태를 읽어서
  currentStateCLK = digitalRead(CLK);

  // CLK핀의 신호가 바뀌었고(즉, 로터리엔코더의 회전이 발생했했고), 그 상태가 HIGH이면(최소 회전단위의 회전이 발생했다면)
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
  {

    // DT핀의 신호를 확인해서 엔코더의 회전 방향을 확인함.
    if (digitalRead(DT) != currentStateCLK)
    {            // 신호가 다르다면 시계방향 회전
      counter++; // 카운팅 용 숫자 1 증가
      currentDir = "시계방향 회전";
    }
    else
    {            // 신호가 같다면 반시계방향 회전
      counter--; // 카운팅 용 숫자 1 감소
      currentDir = "반시계 방향 회전";
    }

    Serial.print("회전방향: ");
    Serial.print(currentDir); // 회전방향 출력
    Serial.print(" | Counter: ");
    Serial.println(counter); // 카운팅 출력
  }

  // 마지막 상태 변수 저장
  lastStateCLK = currentStateCLK;
}

// void read_Roteryencoder() // ########
// {
//   currentStateCLK = digitalRead(rotery_CLK);

//   // CLK핀의 신호가 바뀌었고(즉, 로터리엔코더의 회전이 발생했했고), 그 상태가 HIGH이면(최소 회전단위의 회전이 발생했다면)
//   if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
//   {
//     // DT핀의 신호를 확인해서 엔코더의 회전 방향을 확인함.
//     if (digitalRead(rotery_DT) != currentStateCLK)
//     {            // 신호가 다르다면 시계방향 회전
//       counter++; // 카운팅 용 숫자 1 증가
//       currentDir = "시계방향 회전";
//     }
//     else
//     {            // 신호가 같다면 반시계방향 회전
//       counter--; // 카운팅 용 숫자 1 감소
//       currentDir = "반시계 방향 회전";
//     }
//     Serial.print("회전방향: ");
//     Serial.print(currentDir); // 회전방향 출력
//     Serial.print(" | Counter: ");
//     Serial.println(counter); // 회전 카운팅 출력
//   }

//   // 현재의 CLK상태를 저장
//   lastStateCLK = currentStateCLK;

//   // 버튼(스위치)이 눌렸는지 확인
//   int btnState = digitalRead(rotery_SW);

//   // 버튼(스위치)가 눌리면
//   if (btnState == LOW)
//   {
//     // 버튼이 눌린지 50ms가 지났는지 확인, 즉 버튼이 한번 눌린 후 최소 50 ms는 지나야 버튼이 다시 눌린것으로 감지
//     if (millis() - lastButtonPress > 50)
//     {                               // 50ms 이상 지났다면
//       Serial.println("버튼 눌림!"); // 버튼 눌림 메시지 출력
//     }
//     // 마자막 버튼이 눌린 시간 저장
//     lastButtonPress = millis();
//   }
// }

void draw_Display() // ########
{
  u8g2.clearBuffer();                 // 버퍼를 지움
  u8g2.setFont(u8g2_font_ncenB14_tr); // 폰트 설정

  // roteryencoder 의 counter 숫자에 따라서 화면 커서가 이동하고 버튼 스위치 눌러서 하위 메뉴로 들어가기
  if (counter == 0)
  {

    u8g2.setCursor(0, 20);      // 커서 위치 설정
    u8g2.print("1. 현재 시간"); // 현재 시간 출력
  }
  else if (counter == 1)
  {
    u8g2.setCursor(0, 20);      // 커서 위치 설정
    u8g2.print("1. 현재 시간"); // 현재 시간 출력
    u8g2.setCursor(0, 40);      // 커서 위치 설정
    u8g2.print("2. 현재 온도"); // 현재 온도 출력
    u8g2.setCursor(0, 60);      // 커서 위치 설정
    u8g2.print("3. 현재 습도"); // 현재 습도 출력
    u8g2.setCursor(0, 80);      // 커서 위치 설정
    u8g2.print("4. 현재 시간"); // 현재 시간 출력
  }
  else if (counter == 2)
  {
    u8g2.setCursor(0, 20);      // 커서 위치 설정
    u8g2.print("1. 현재 시간"); // 현재 시간 출력
    u8g2.setCursor(0, 40);      // 커서 위치 설정
    u8g2.print("2. 현재 온도"); // 현재 온도 출력
    u8g2.setCursor(0, 60);      // 커서 위치 설정
    u8g2.print("3. 현재 습도"); // 현재 습도 출력
  }
  else if (counter < 0)
  {
    counter = 0;
  }
  else if (counter > 2)
  {
    counter = 2;
  }
  u8g2.sendBuffer(); // 버퍼를 디스플레이에 출력
}

//& RP2040_SD 에 대한 주석을 복사해왔습니다. 저작권에 대한 내용입니다.
/*
chatAI에 의한 번역본
RP2040_SD 라이브러리는 Arduimo-mbed 또는 arduino-pico 코어를 사용하는 RP2040 기반 보드에서 SD 카드를 사용할 수 있게 하는 라이브러리입니다.
이 라이브러리는 GNU 일반 공중 사용 허가서의 조건에 따라 자유 소프트웨어로 배포하거나 수정할 수 있습니다.
이 라이브러리는 일부 보증 없이 유용하게 사용될 수 있도록 배포됩니다.
RP2040_SD 라이브러리는 Arduino SdFat 라이브러리를 기반으로 만들어졌으며, FAT16/FAT32 파일을 SD 카드에 읽고 쓰는 데 사용되는 라이브러리입니다.
이 라이브러리는 William Greiman이 처음 개발했으며, SparkFun Electronics 및 Khoi Hoang이 수정하였습니다.
RP2040_SD 라이브러리는 Khoi Hoang에 의해 만들어졌으며 GPL-3.0 라이선스로 사용이 허가됩니다.
이 라이브러리를 사용하는 경우 GNU 일반 공중 사용 허가서의 사본을 받아야 합니다. 사본을 받지 못한 경우,
라이선스를 확인할 수 있는 http://www.gnu.org/licenses/에서 확인할 수 있습니다.
*/
/****************************************************************************************************************************
  DataLogger.ino

  For all RP2040 boads using Arduimo-mbed or arduino-pico core

  RP2040_SD is a library enable the usage of SD on RP2040-based boards

  This Library is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

  This Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with the Arduino SdFat Library.
  If not, see <http://www.gnu.org/licenses/>.

  Based on and modified from  Arduino SdFat Library (https://github.com/arduino/Arduino)

  (C) Copyright 2009 by William Greiman
  (C) Copyright 2010 SparkFun Electronics
  (C) Copyright 2021 by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RP2040_SD
  Licensed under GPL-3.0 license
 *****************************************************************************************************************************/
/*

/*
& DHT22 라이브러리의 깃허브 주소 입니다. 꼭 라이센스 확인 하세요
Werner Panocha, February 2023
https://github.com/dvarrel/DHT22

*/

void setup() // ########
{
  Serial.begin(115200);

  pinMode(dht22_dataPin, INPUT); // DHT22 data pin to Pico GPIO 2
  pinMode(rotery_CLK, INPUT);    // 로터리 모듈 자체 풀업 저항 사용
  pinMode(rotery_DT, INPUT);     // 로터리 모듈 자체 풀업 저항 사용
  pinMode(rotery_SW, INPUT);     // 로터리 모듈 자체 풀업 저항 사용

  // interrupt 설정
  lastStateCLK = digitalRead(rotery_CLK);                                      // CLK의 초기 신호상태를 저장  // 인터럽트 사용함에따라 주석처리
  attachInterrupt(digitalPinToInterrut(rotery_CLK), rotery_interrunt, CHANGE); // CLK 신호가 바뀔때마다 roteryCLK() 함수를 호출
  attachInterrupt(digitalPinToInterrut(rotery_DT), rotery_interrunt, CHANGE);  // SW 신호가 FALLING일때 roterySW() 함수를 호출

  RP2040_SdFile::dateTimeCallback(dateTime);
  // 기존 SdFile:: ~ 에서 앞에 RP2040_SdFile로 바뀜 나머진 안 바뀜
  // callback funtion for SD card  , 생성파일에 현재 시간을 넣기 위한 함수
  setup_SDcard(); // SD카드 초기화 , 상태 체크 함수
  setup_RTC();    // RTC 초기화 , 상태 체크 함수
  dht22.begin();  // DHT22 초기화
  u8g2.begin();   // u8g2 display 초기화
}

void loop() // ########
{
  DateTime now = rtc.now();

  // 2초마다 한번씩 동작하는 함수들
  if (millis() - previousMillis > interval)
  {
    previousMillis = millis();
    Serial.println("2초마다 한번씩 동작하는 함수모음 시작~");
    read_DHT22();
    write_SDcard(now); // SD카드 파일 생성 및 열기 , 데이터 쓰기 함수 , 나중에 DHT22 와 같이 동작할것
    draw_Display();    // OLED 디스플레이에 데이터 출력 함수
    Serial.println("2초마다 한번씩 동작하는 함수모음 동작 끝~");
    Serial.println();
  }

  // read_Roteryencoder(); // 로터리 인코더 함수

  u8g2.clearBuffer(); // clear the internal memory
  draw_RootMenu(now); // 메인 메뉴 그리기 함수
  u8g2.sendBuffer();  // transfer internal memory to the display
}