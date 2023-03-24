/*


//& RP2040_SD 에 대한 주석을 복사해왔습니다. 저작권에 대한 내용입니다.

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
/*
***************************************************************************************************************************
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
  - 2023.03.20 #1 : 라이브러리들 다시 하나씩 깨워서 충돌 여부 체크 하고 있음 , 충돌 나는게 있는 것같음
  - 2023.03.20 #2 : 충돌아는 라이브러리 RTClib.h -> RP2040_RTC.h 로 변경 예정 으로 잡았으나 이 라이브러리는 ds3231 용이 아닌 pico 내부 rtc 제어용이다.
  - 2023.03.21 #1 : adafruit ssd1306 라이브러리 버그가 있음 , 고치는 링크 : https://forum.arduino.cc/t/library-pgmspace/1077639/9
  - 2023.03.22 #1 : RTClib.h  => 대체 1 라이브러리 "uRTCLib.h" 를 테스트중  => 작동 이상없이됨 , 다만 SD 카드에 날짜 넘기는 부분???
  - 2023.03.22 #2 : 현재 ROTERY , OLED , DHT22 , RTC , SD ( 파일 날짜 기입빼곤 )까지 동작 확인함
  - 2023.03.22 #3 : 003 -> 004 버전을 바꾸면서  SD 카드 파일 생성시 현재 날짜가 들어가게 해주는 부분 + 코드 정리 까지 예정 

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

- //!피코보드 딥클린 시켜주는 uf2 파일 링크 : https://learn.adafruit.com/raspberry-pi-pico-led-arcade-button-midi-controller-fighter/installing-circuitpython

^@ 중요변경 사항
- U8glib -> U8g2 로 변경 ( 002 버전 code부터 적용 )
- simple dht 라이브러리 -> DHT22 라이브러리 ( 002 버전 code부터 적용 )
- 아두이노 SD 라이브러리 -> RP2040_SD 라이브러리 ( 002 버전 code부터 적용 )
- setup() 과  loop() 의 위치를 맨 아래로 수정함 ( 002 버전 code부터 적용 )
- u8g2 라이브러와 인터럽트 충돌로 도저히 방법을 못 찾아 adafruit ssd1306 라이브러리로 변경함 ( 003 버전 code부터 적용 )
- RTClib 라이브러리 먹통되는 현상으로 uRTClib 으로 바꿈 

^@라이센스 정보
기본 MIT 라이센스 적용이 되나 , 각 라이브러리 , 참고한 코드 출처에서 라이센스 정보를 확인해야 한다.
코드 맨 아래 쓰인 라이브러리 링크 및 라이센스 정보를 확인하면 됩니다.
*/

// #include <RTClib.h>  // 도저히 동작 먹통되는 것에 대한 해결방법을 못 찾아 우선 제외  , 뭘 써야 하나 ?

#include "Arduino.h"
#include <SPI.h> // SD카드 사용을 위한 SPI 라이브러리
#include <Wire.h>
#include <DHT22.h>
#include "uRTCLib.h" // PICO 보드 이상없음 
#include <RP2040_SD.h>

#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold9pt7b.h>

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

uRTCLib rtc(0x68); // DS3231 RTC 모듈을 사용하기 위한 객체 생성 , 0x68 은 DS3231 RTC 모듈의 I2C 주소

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES 10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

// miso = 16 , mosi = 19 , sck = 18 , cs = 17  => pico보드의 SPI(0) 기본 핀
File myFile;
const int chipSelect = 17; // SD카드 CS 핀을 Pico GPIO17에 연결
String fileName = "tata";  // 파일명
int fileNumber = 0;        // 파일번호

const int dht22_dataPin = 2; // DHT22 data pin to Pico GPIO2
DHT22 dht22(dht22_dataPin);

const int encoder0PinA = 15; // setup() 안의 인터럽트 구문에 들어가는 핀이 clk이다.
const int encoder0PinB = 14; // setup() 안의 인터럽트 구문에 들어가는 핀이 clk이다.
const int encoder0PinBtn = 13;
volatile long encoder0Pos = 0;
int newEncoder0Pos = 0;
volatile int8_t encoder0PinALast;
int encoder0BtnPinState;

unsigned long previousMillis = 0; // millis()를 위한 변수
const int interval = 2000;        // 2초마다 실행

// void dateTime(uint16_t *date, uint16_t *time) // setup () dateTime 이라는 함수가 쓰일 예정이므로 일찍??? 선언 하는건가?
// {
//   DateTime now = rtc.now();
//   *date = FAT_DATE(now.year(), now.month(), now.day());
//   *time = FAT_TIME(now.hour(), now.minute(), now.second());
// }

void setup_SDcard() // ########
{
  // sd카드 존재 여부 체크
  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card not found");
    return;
    // oled 쪽에도 표현하는 걸 추가해야할 수도 있다.
  }

  //파일이 존재하는지 확인
  while (SD.exists(fileName + String(fileNumber) + ".txt"))
  {
    fileNumber++;
  }
}

void write_SDcard(/*DateTime now*/) // ########
{
  // 파일 생성 및 열기
  myFile = SD.open(fileName + String(fileNumber) + ".txt", FILE_WRITE);

  // 파일 열기에 성공했다면 데이터 쓰기
  if (myFile)
  {
    Serial.print(F("Writing to tadaX.txt..."));
    // myFile.print(now.year());
    // myFile.print('/');
    // myFile.print(now.month());
    // myFile.print('/');
    // myFile.print(now.day());
    // myFile.print(" (");
    // myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    // myFile.print(") ");
    // myFile.print(now.hour());
    // myFile.print(':');
    // myFile.print(now.minute());
    // myFile.print(':');
    // myFile.print(now.second());
    // myFile.print(" , ");
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

void rtc_ds3231()
{
  rtc.refresh();
  Serial.print("RTC DateTime: ");

  Serial.print(rtc.year());
  Serial.print('/');
  Serial.print(rtc.month());
  Serial.print('/');
  Serial.print(rtc.day());

  Serial.print(' ');

  Serial.print(rtc.hour());
  Serial.print(':');
  Serial.print(rtc.minute());
  Serial.print(':');
  Serial.print(rtc.second());

  Serial.print(" DOW: ");
  Serial.print(rtc.dayOfWeek());

  Serial.print(" - Temp: ");
  Serial.print((float)rtc.temp() / 100);
  Serial.println();
}

void doEncoderA()
{
  int8_t encoder0PinAValue = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (encoder0PinAValue == HIGH))
  {
    if (digitalRead(encoder0PinB) == LOW)
    {

      encoder0Pos++;
    }
    else
    {
      encoder0Pos--;
    }
  }
  encoder0PinALast = encoder0PinAValue;
}

void menu_selectNumber()
{
  newEncoder0Pos = encoder0Pos;
  if (newEncoder0Pos <= 0)
  {
    newEncoder0Pos = 0;
    encoder0Pos = newEncoder0Pos;
  }
  if (newEncoder0Pos >= 2)
  {
    newEncoder0Pos = 2;
    encoder0Pos = newEncoder0Pos;
  }
}

void firstPage_LOOP(void)
{
  display.clearDisplay(); // clear to buffer
  display.setTextColor(SSD1306_WHITE);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(0, 12);
  display.println(F("23/03/03"));
  display.setCursor(0, 24);
  display.println(F("14 : 17"));

  firstPage_menuName();
  firstPage_cursor();
  display.display(); // Show initial Wtext
}

void firstPage_menuName(void)
{
  display.setCursor(36, 40);
  display.println(F("DHT22")); // F 메크로 를 사용하는걸 잊지 말것
  display.setCursor(36, 52);
  display.println(F("PICO")); // F 메크로 를 사용하는걸 잊지 말것
  display.setCursor(36, 64);
  display.println(F("ETC ..")); // F 메크로 를 사용하는걸 잊지 말것
  //  이 함수처럼    다른 페이지에 클리어디스플레이 와 디스플레이 함수 구간안에 넣으면 작동한다.
  //  즉 여러개의  클리어디스플레이 와 디스플레이 함수를 쓸 필요가 없다.
}

void firstPage_cursor() // play in loop
{
  if (newEncoder0Pos == 0)
    display.setCursor(12, 38);

  else if (newEncoder0Pos == 1)
    display.setCursor(12, 50);

  else if (newEncoder0Pos == 2)
    display.setCursor(12, 62);

  display.print(F(">>"));
}

void serialMonitor()
{
  Serial.print("encoder0Pos = ");
  Serial.print(encoder0Pos);
  Serial.print("  newEncoder0Pos = ");
  Serial.print(newEncoder0Pos);
  Serial.print(" , encoder0BtnPinState = ");
  Serial.print(encoder0BtnPinState);

  Serial.print("  Temperature: ");
  Serial.print(dht22.getTemperature());
  Serial.print(" C, Humidity: ");
  Serial.print(dht22.getHumidity());
  Serial.println(" %");
}

void setup() // ########
{
  delay(1000);
  Serial.begin(9600);

#ifdef ARDUINO_ARCH_ESP8266
  URTCLIB_WIRE.begin(4, 5); // D3 and D4 on ESP8266
#else
  URTCLIB_WIRE.begin();
#endif

  if (rtc.enableBattery())
  {
    Serial.println("Battery activated correctly.");
  }
  else
  {
    Serial.println("ERROR activating battery.");
  }

  Serial.print("Lost power status: ");
  if (rtc.lostPower())
  {
    Serial.print("POWER FAILED. Clearing flag...");
    rtc.lostPowerClear();
    Serial.println(" done.");
  }
  else
  {
    Serial.println("POWER OK");
  }

  // rtc.set( 0 , 16 , 15 , 3, 22, 3 , 23);  //  기본 예제로 미리 입력 해놓아서 주석처리함 , 안하면 같은 세팅반복됨

  pinMode(dht22_dataPin, INPUT); // DHT22 data pin to Pico GPIO 2
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0PinBtn, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, CHANGE);  // 다른 엔코더 사용하면 필요해질지 몰라 남김 .
  encoder0PinALast = digitalRead(encoder0PinA);

   //RP2040_SdFile::dateTimeCallback(dateTime);
  // 기존 SdFile:: ~ 에서 앞에 RP2040_SdFile로 바뀜 나머진 안 바뀜
  // callback funtion for SD card  , 생성파일에 현재 시간을 넣기 위한 함수
  setup_SDcard(); // SD카드 초기화 , 상태 체크 함수  

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
}

void loop() // ########
{
  encoder0BtnPinState = digitalRead(encoder0PinBtn);

  //2초마다 한번씩 동작하는 함수들
   if (millis() - previousMillis > interval)
  {
    previousMillis = millis();
    Serial.println("2초마다 한번씩 동작하는 함수모음 시작~");

    rtc_ds3231(); // RTC 시간 읽어오는 함수
    write_SDcard(/*now*/); // SD카드 파일 생성 및 열기 , 데이터 쓰기 함수 , 나중에 DHT22 와 같이 동작할것

    Serial.println("2초마다 한번씩 동작하는 함수모음 동작 끝~");
    Serial.println();
  }

  menu_selectNumber();
  firstPage_LOOP();  
  serialMonitor(); // 디버깅용 시리얼 출력 함수
}