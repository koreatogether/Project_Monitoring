/*
^@배선도  // ! 수정 필요
Hardware  ||  pico   ||  DHT22( 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP9    ||   DATA

Hardware  ||  pico   ||  OLED 128x64 SSD1306 ( 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP8    ||   SDA
              GP7    ||   SCL

Hardware  ||  pico   ||  SDCARD@YUROBOT ( 3.3V  , 5V 그낭 )
              3.3V   ||   VCC
              GND    ||   GND
              GP6    ||   CS
              GP5    ||   MOSI
              GP4    ||   MISO
              GP3    ||   SCK

Hardware  ||  pico   ||  DS3231RTC ( 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP2    ||   SDA
              GP1    ||   SCL

Hardware  ||  pico   ||  RotaryEncorder ( 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP0    ||   SW
              GP10   ||   DT
              GP11   ||   CLK

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
  - 2023.03.12 #2 : 파이피코 전원 공급 방법 공부 ( 왠만하면  40핀에 연결하지 말것 )
                    VSYS(39번핀) 에 다이오드로 연결해서 외부전원공급할것 최소 2볼트 필요
                    이렇게하면 코딩도 가능하고 업로드도 가능하다. - 이건 유튜브 내용잘살펴볼것 
                    36번핀  3.3V OUTPUT MAX 300mA 출력이 가능하다(공식문서 참고)

^@참고 사이트
- 파이피코 변형보드 만드신분  : https://sira-engineer.tistory.com/6?category=966430
- 파이피코 핀맵 깔끔한 거 : https://blog.naver.com/yunc26/222590881823
- 로터리엔코더 사용법 안내 및코드 : https://cafe.naver.com/dgarduino/9173
- 파이피코 전원 공급에 대한 안내 : https://remnant24c.tistory.com/268
- 파이피코 전원 공급 안내 유튜브 (해외 ) : https://youtu.be/3PH9jzRsb5E
- 파이피코 상제 내역 블로그 ( 한글 ) : https://fishpoint.tistory.com/7369
- 파이피코 제원 상세 : https://www.etechnophiles.com/raspberry-pi-pico-pinout-specifications-datasheet-in-detail/

- 파이피코 + 로터리엔더코 기초코드 : https://cafe.naver.com/dgarduino/9173

^@ 중요변경 사항
- U8glib -> U8g2 로 변경 ( 002 버전 code부터 적용 )
*/

#include "RTClib.h"
#include <U8glib.h>
#include "DHT.h"
#include <SD.h>
#include <SPI.h>

enum State  // 수정 필요
{
  INTIALIZE,
  READ_DHT22,
  WRITE_SD,
  DISPLAY_OFF_1SEC,
  DISPLAY_ON_3SEC
};
State state = INTIALIZE;

RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);

File myFile;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned long previousMillis_OLED = 0;
unsigned long previousMillis_DHT22 = 0;
unsigned long previousMillis_WRITESD = 0;
const int interval_ON = 3000;      // 3초 주기 , ON
const int interval_OFF = 1000;     // 1초 주기 , OFF
const int interval_DHT22 = 2000;   // 2초 주기 , DHT22 센서 값 읽어오기
const int interval_WRITESD = 2000; // 2초 주기 ,  SD에 값쓰기
bool onOffFlag = 0;                // 1이면 화면이 켜져있고 0이면 화면이 꺼져있다.

const int DHTPIN = 9;      // what digital pin we're connected to
const int DHTTYPE = DHT22; // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float int_Humidity = 0;
float int_Temperature = 0;

const int sdCardSelect = 4; // SD 카드의 CS 핀을 ?? 핀에 연결한다.
String fileName = "data";
int fileNumber = 1;

// check make file current date and time
void dateTime(uint16_t *date, uint16_t *time)
{
  DateTime now = rtc.now();
  *date = FAT_DATE(now.year(), now.month(), now.day());
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

void setup()
{
  Serial.begin(9600);
  // u8g font setup
  u8g.setFont(u8g_font_unifont);
  dht.begin();

  // callback function for SD card
  SdFile::dateTimeCallback(dateTime);

  if (!rtc.begin()) // RTC 모듈이 연결되어있는지 확인하는 구간
  {
    Serial.println(F("Couldn't find RTC"));
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) // cr2032 배터리가 없을때에 현재시간을 입력하는 구간
  {                    // 배터리가 없을때에 현재시간을 입력하는 조건문
    Serial.println(F("RTC lost power, let's set the time!"));
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2023, 2, 13, 20, 7, 0));  // 수동으로 현재 시간을 맞출때   2023년 , 2월 , 13일 , 20시 , 07분 , 00초
  }

  if (!SD.begin(sdCardSelect)) // SD 카드가 연결되어있는지 확인하는 구간
  {
    Serial.println(F("SD Card not found"));

    u8g.firstPage();
    do
    {
      u8g.drawStr(30, 40, "SD Card not found");
    } while (u8g.nextPage());
  }
  Serial.println(F("initialization done."));

  // SD 카드에 기존 파일이 존재한다면 파일 이름의 숫자부분을+1 을 계속한다.
  // 만약 파일이름 file01, file02 , file03 까지 검색이 된다면 file04 를 만들기 위한 fileNumber 변수를 +1 해준다.
  while (SD.exists(fileName + String(fileNumber) + ".txt"))
  {
    fileNumber++;
  }
}

void loop()
{

  DateTime now = rtc.now();

  switch (state)
  {
  case INTIALIZE:
    state = READ_DHT22;
    // Serial.print(" INTIALIZE");
    break;

  case READ_DHT22:
    // 2초마다 DHT22 센서의 값을 읽어오는코드 추가할것
    if (millis() - previousMillis_DHT22 >= interval_DHT22)
    {
      previousMillis_DHT22 = millis();
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      int_Temperature = dht.readTemperature();
      int_Humidity = dht.readHumidity();
      if (isnan(h) || isnan(t))
      {
        Serial.println(F("Failed to read from DHT sensor!"));
      }
    }
    state = WRITE_SD;
    // Serial.print("   READ_DHT22   ");
    break;

  case WRITE_SD:
    if (millis() - previousMillis_WRITESD >= interval_WRITESD)
    {
      previousMillis_WRITESD = millis();

      myFile = SD.open(fileName + String(fileNumber) + ".txt", FILE_WRITE);
      // if the file opened okay, write to it:
      if (myFile)
      {
        Serial.print(F("Writing to datalog.txt..."));
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
        myFile.print(dht.readHumidity());
        myFile.print(" , ");
        myFile.println(dht.readTemperature());
        // close the file:
        myFile.close();
        Serial.println(F("done."));
        Serial.print(fileName);
        Serial.print(F("  "));
        Serial.println(fileNumber);
      }
      else
      {
        // if the file didn't open, print an error:
        Serial.println(F("error opening datalog.txt"));
      }
    }
    state = DISPLAY_OFF_1SEC;
    // Serial.print("  WRITE_SD   ");
    break;

  case DISPLAY_OFF_1SEC:
    if (millis() - previousMillis_OLED >= interval_ON && onOffFlag == 0) // 1초마다 돌아가는 구간
    {
      previousMillis_OLED = millis();
      u8g.firstPage();
      do
      {

        // 검은화면용
      } while (u8g.nextPage());
      onOffFlag = 1;
    }
    state = DISPLAY_ON_3SEC;
    // Serial.print("   DISPLAY_OFF_1SEC   ");
    break;

  case DISPLAY_ON_3SEC:
    if (millis() - previousMillis_OLED >= interval_OFF && onOffFlag == 1) // 3초마다 돌아가는 구간
    {
      previousMillis_OLED = millis();
      if (!SD.begin(sdCardSelect)) // SD 카드가 연결되어있는지 확인하는 구간
      {
        Serial.println(F("SD Card not found"));

        u8g.firstPage();
        do
        {
          u8g.drawStr(30, 20, "SD Card ");
          u8g.drawStr(30, 40, "not found ");
        } while (u8g.nextPage());
      }
      else
      {
        oledDisplay_days(now); // oled 날짜 ,시간 표시 하는 함수 , 너무 길어서 사용자 정의 함수로 빼 놓음
      }
      onOffFlag = 0;
    }
    // Serial.println("   DISPLAY_ON_3SEC   ");
    state = READ_DHT22;
    break;

  default:
    state = INTIALIZE;
    break;
  }
}

//******************  사용자 정의 함수  #001  ***********************
void oledDisplay_days(DateTime now) // DateTime now 이렇게 안하면 전역 , 지역 범위때문에 에러난다.
{

  u8g.firstPage();
  do
  {
    // 날짜 출력 부분
    u8g.drawStr(0, 10, "Date:");
    u8g.setPrintPos(40, 10);
    u8g.print(now.year());    // , DEC를 제거하면 제대로 출력된다.  연도 출력
    u8g.drawStr(75, 10, "/"); // 구별 기호
    u8g.setPrintPos(90, 10);
    u8g.print(now.month());
    u8g.drawStr(100, 10, "/"); // 구별 기호
    u8g.setPrintPos(110, 10);
    u8g.print(now.day());
    // 요일 출력 부분
    u8g.setPrintPos(40, 25);
    u8g.print(daysOfTheWeek[now.dayOfTheWeek()]);
    //  시 , 분 , 초 출력 부분
    u8g.setPrintPos(10, 40);
    u8g.print(now.hour());    // , DEC를 제거하면 제대로 출력된다.  연도 출력
    u8g.drawStr(28, 40, "h"); // 구별 기호
    u8g.setPrintPos(45, 40);
    u8g.print(now.minute());
    u8g.drawStr(65, 40, "m"); // 구별 기호a
    u8g.setPrintPos(85, 40);
    u8g.print(now.second());
    u8g.drawStr(105, 40, "s");

    // 온도 / 습도 출력부분

    u8g.setPrintPos(10, 55);
    u8g.print(int_Temperature, 1);

    u8g.drawStr(45, 55, "\xB0"
                        "C");

    u8g.setPrintPos(70, 55);
    u8g.print(int_Humidity, 1);
    u8g.drawStr(105, 55, "%");

  } while (u8g.nextPage());
}

// *** 하드웨어 정보  ***

// YUrobot SD 카드 링크
// https://www.devicemart.co.kr/goods/view?no=1311842
// 작동 전압 : 5V


/*
참고한 블로그글
1. https://m.blog.naver.com/yjsem/221230396967

2. 배터리 손상 방지
   https://rudalskim.tistory.com/224
   * 저항을 제거 해서 쓰는 편이 좋겠다.

3. 다른 예제
https://chocoball.tistory.com/entry/Hardware-RTC-usning-DS3231-1
*/

/*
기본 코드에  oled에 시간 표시를 해주는 코드를 추가했다.
*/

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
/*
참고한 블로그글
1. https://m.blog.naver.com/yjsem/221230396967

2. 배터리 손상 방지
   https://rudalskim.tistory.com/224
   * 저항을 제거 해서 쓰는 편이 좋겠다.
   * 충전 방식의 건전지를 위한 회로라고 하는데 보호 회로가 없다시피해서
    추천 못한다고 해외 글이 올라옴

3. 다른 예제
https://chocoball.tistory.com/entry/Hardware-RTC-usning-DS3231-1
*/

/*
기본 코드에  oled에 시간 표시를 해주는 코드를 추가했다.
+Oled 코드에 디스플레이 화면 보호기처럼 주기적으로 나오게 하는 코드를 추가한다.
delay 함수 대신 millis ()를 이용한다.
+dht22 온습도 센서를 추가한다. 23-02-16
센서만 있었기에 V5 AND SIGNAL 사이에 10K 저항을 추가했다.
23-02-17 SD카드에 저장할 데이타 종류를 정리해봤는데 센서ID를 넣으면 좋을 것같다.
23-02-17 SD카드안에 파일이 있는지 확인 , 있다면 +1 을 해서 파일명을 만들어주는 코드를 넣는다.
만들긴 했는데
시스템이 불안해서 멈추는 현상이 나온다.

다만 sd 카드만 구동했을땐 실시간으로 열고 쓰고 데이타를 저장하는데 문제 없다
메모리 문제인가 ?  - SRAM 메모리 문제가 맞다. 적어도 477바이트는 확보할것
이때 F() 함수를 사용하면 메모리를 절약할 수 있다.
도움되는 링크 : https://postpop.tistory.com/92
                https://juahnpop.tistory.com/159

부팅 , 도는 전원을 끄고 켤때 파일 넘버를 체크해서 기존에 있는 파일이 있으면 +1을 해서 파일을 만들어준다.
- 리셋 버튼 과 전원 자체를 껐다 켜도 잘 동작하는거 를 10회씩에 걸쳐서 확인함

- 메모리 파일 기록수 파일이름은 8자리벗어나지 않게 한다.
예 > 2021-02-17_01.csv -> 에러난다.  2210217#1.csv 로 변경

23-02-18 나노의 메모리가 부족해서 첫번째 페이지에 날짜 및 온도 / 습도 표시하는것으로 마무리하고
라즈베리파이 피코 일반 버전으로 옮겨서 회로 구성 및 메모리상태 보고선 나머지 구현할것들 하기로함

23-02-20 008 버전은 코드를 FSM 버전으로 바꾸는 것이다. 비공개용
공개용은 007버전까지만

유니폰트 링크 : https://github.com/olikraus/u8glib/wiki/fontgroupunifont
사용 요령  "0xb32""han" 식으로 하면 된다.

추후 메뉴 구성 만들때 참고할 링크 : https://rottenstar.tistory.com/35


[구현미정] 측정 날짜 시간  , 01_ 센서ID , 온도 , 습도 , 02_센서ID , 온도 ,습도 , 03_센서ID ~ 식으로

코드 출처 : DHT 라이브러리 , SD 카드 기본 코드 , RTC 라이브러리 , U8GLIB 라이브러리 입니다.

아두이노 나노 기준  98 % , 71% 의 각 메모리를 사용하고 있습니다.

마지막으로 동작에 대한 간단한 서술입니다.

1. 전원이 켜진다.
2. RTC 시간을 가져온다.
3. DHT22 센서를 통해 온도와 습도를 가져온다.
4. SD 카드에 기존 파일이 있는지 체크한다 있다면 +1 을 해서 새로운 파일을 만든다.
5. SD카드에 데이터를 기록한다.
6. 1초간 대기한다. ( OLED 꺼짐 )
7. 3초간 대기한다. ( OLED 켜짐 )

이를 반복하는 과정입니다.
SD 카드엔 전원이 켜지고 꺼질때마다 기존 파일 + 1 을 해서 새로운 파일을 생성하게 했습니다.
테스트는 10회를 전원을 껐다 켜봤는데 파일 생성엔 문제가 없었습니다.
기타 에러에 대한 주의는 필요해 보입니다.

원가 계산표( 23- 02 -20 기준)
arduino nano 호환 보드  4,200원
DS3231 RTC  3,000원
DHT22 센서  3,500원
OLED 128x64 SSD1306  3,000원
SD 카드 모듈  500원
기타 1,500원
배송비 3,000원
총합계 18,700원

23-02-28 해야 할일

우선 나노 , 우노 보드 기준으로  센서만 바꾸어서 쓸 수 있게 코드를 수정해야 한다.
DHT22
TMP36
BME280
DS18B20

23-03-10
추가한 코드 내역
SD 파일 생성 날짜를 DS3231 RTC 로부터 읽어와서 생성하도록 수정

23-03-11
보드 변경
nano -> raspberry pi pico
이유 : 메모리 부족

출처 : https://forum.arduino.cc/t/add-date-and-time-to-your-sd-card-files/318901
*/