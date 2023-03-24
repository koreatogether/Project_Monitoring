
/*
@ 코드상의 주석 말고는 모든 사항은 README.md 에 기록되어 있습니다.
*/

#include <Arduino.h>           // 기본 라이브러리 , 전체동작 테스트 이상없음
#include <SPI.h>               // SPI 사용을 위한 SPI 라이브러리
#include <Wire.h>              // I2C 통신을 위한 라이브러리 , 전체동작 테스트 이상없음
#include <DHT22.h>             // DHT22 온습도 센서 라이브러리 , 전체동작 테스트 이상없음
#include <uRTCLib.h>           // DS3231 RTC 라이브러리 , 전체동작 테스트 이상없음
#include <RP2040_SD.h>         // RP2040용 SD카드 라이브러리 , ARDUINO SD 랑 동일
#include <LiquidCrystal_I2C.h> // LCD 용 라이브러리 , 에러는 뜨지만 동작에는 상관 없음 ( 짧은 시간 테스트 기준 )

// 변수 선언 순서는  DS3231 RTC, LCD , SD카드 , DHT22 , millis()관련 순서로 한다.

// DS3231 RTC 모듈을 사용하기 위한 객체 생성 , 0x68 은 DS3231 RTC 모듈의 I2C 주소 ,  MISO = 16 , MOSI = 19 , SCK = 18 , CS = 17  => pico보드의 SPI(0) 기본 핀
uRTCLib rtc(); // DS3231 RTC 모듈을 사용하기 위한 객체 생성 , 0x68 은 DS3231 RTC 모듈의 I2C 주소

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD 모듈을 사용하기 위한 객체 생성 , 0x27 은 I2C LCD 모듈의 I2C 주소 , 16은 칸 , 2는 줄을 의미

File myFile;                        // SD카드 파일 객체 생성
const int chipSelect = 17;          // SD카드 CS 핀을 Pico GPIO17에 연결
String fileName = "tata";           // 파일명
int fileNumber = 0;                 // 파일번호

const int dht22_dataPin = 2;        // DHT22 data pin to Pico GPIO2
DHT22 dht22(dht22_dataPin);

unsigned long previousMillis = 0;   // millis()를 위한 변수 1
const int interval_2s = 2000;       // 2초마다 실행

unsigned long previousMillis_2 = 0; // millis()를 위한 변수 2
const int interval_1s = 1000;       // 1초마다 실행

//########################################################################################

/*
dateTIME ( 인수1 , 인수2) => RTC 객체를 이용 FAT 파일의 메타속성에 시간과 날짜를 넘기기위한 함수
상세한것은  Readme.md 에 기록되어 있습니다.
*/
void dateTime(uint16_t *date, uint16_t *time) // setup () dateTime 이라는 함수가 쓰일 예정이므로 일찍??? 선언 하는건가?
{
  rtc.refresh();
  int years_4 = rtc.year() + 2000; // rtc.year() 의 값이 2자리 정수라서 , FAT_DTAE 의 Y 인수에서 요구하는 4자리 정수가 아니라 2000을 더했습니다.
  *date = FAT_DATE(years_4, rtc.month(), rtc.day());
  *time = FAT_TIME(rtc.hour(), rtc.minute(), rtc.second());
}

// SD카드 초기화 , 파일 생성 및 기존 파일 존재시 파일명의 숫자 +1 증가
void setup_RTC_DS3231()
{
#ifdef ARDUINO_ARCH_ESP8266
  URTCLIB_WIRE.begin(0, 2); // D3 and D4 on ESP8266
#else
  URTCLIB_WIRE.begin(); // 보드의 기본 4 , 5 핀 사용 I2C(0)
#endif
  rtc.set_rtc_address(0x68);           // DS3231 RTC의 I2C 주소는 0x68
  rtc.set_model(URTCLIB_MODEL_DS3232); // 사용하는 모듈의 종류 설정

  // rtc.set 구문은 RTC 초기 시간만 맞추고선 2차 펌웨어 업로드시에는 주석처리할것! 안그럼 RTC 시간이 매번 초기화됨
  // rtc.set(0, 55, 10, 6, 24, 3, 23);
  //  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

  // 배터리 체크
  if (rtc.enableBattery())
  {
    Serial.println("Battery activated correctly.");
  }
  else
  {
    Serial.println("ERROR activating battery.");
  }

  // 전원 체크
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
}

void setup_LCD()
{
  // Crystal lcd
  lcd.init();
  lcd.init();
  lcd.backlight();
  // print a message to the LCD.
}

void setup_SDcard()
{
  // sd카드 존재 여부 체크
  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card not found");
    return;
  }

  // 기존에 파일이 존재하는지 확인
  while (SD.exists(fileName + String(fileNumber) + ".txt"))
  {
    fileNumber++;
  }
}

//#######################################################################################

// LOOP() 에서 millis() 를 이용한 2초마다 실행되는 SD카드 파일에 데이터 쓰기 함수
void loop_write_SDcard() // ########
{
  // 파일 생성 및 열기
  myFile = SD.open(fileName + String(fileNumber) + ".txt", FILE_WRITE);

  // 파일 열기에 성공했다면 데이터 쓰기
  if (myFile)
  {

    Serial.print(F("Writing to tadaX.txt..."));
    myFile.print(rtc.year());
    myFile.print('/');
    myFile.print(rtc.month());
    myFile.print('/');
    myFile.print(rtc.day());
    myFile.print(" (");
    myFile.print(rtc.dayOfWeek());
    myFile.print(") ");
    myFile.print(rtc.hour());
    myFile.print(':');
    myFile.print(rtc.minute());
    myFile.print(':');
    myFile.print(rtc.second());
    myFile.print(" , ");
    myFile.print(dht22.getTemperature());
    myFile.print(" , ");
    myFile.println(dht22.getHumidity());
    myFile.close(); // close the file:

    /* 추후 테스트 할때 활성화
    Serial.println(F("done."));
    Serial.print(fileName);
    Serial.print(F("  "));
    Serial.println(fileNumber);
    */
  }
  else
  {
    // 파일 열기 실패시
    Serial.println("error opening tadaX.txt");
  }
}

void loop_LCD()
{
  lcd.setCursor(0, 0);
  lcd.print(" ");
  lcd.print(rtc.month());
  lcd.print("/");
  lcd.print(rtc.day());
  lcd.print("   ");
  lcd.print(rtc.hour());
  lcd.print(":");
  lcd.print(rtc.minute());
  lcd.print(":");
  lcd.print(rtc.second());

  lcd.setCursor(0, 1);
  lcd.print(dht22.getTemperature(), 1);
  lcd.print(" C ");
  lcd.print(dht22.getHumidity(), 1);
  lcd.print(" %  ");
}

void loop__serialMonitor_rtc_ds3231()
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

void serialMonitor()
{
  Serial.print("  Temperature: ");
  Serial.print(dht22.getTemperature());
  Serial.print(" C, Humidity: ");
  Serial.print(dht22.getHumidity());
  Serial.println(" %");
}

//######################################################################################
void setup()
{
  Serial.begin(9600);
  pinMode(dht22_dataPin, INPUT);            // DHT22 data pin to Pico GPIO 2

  RP2040_SdFile::dateTimeCallback(dateTime);
  setup_RTC_DS3231();                       // RTC 초기화
  setup_LCD();                              // LCD 초기화
  setup_SDcard();                           // SD카드 초기화
}

void loop() // ########
{

  // 2초마다 한번씩 동작하는 함수들
  if (millis() - previousMillis > interval_2s)
  {
    previousMillis = millis();

    loop_write_SDcard();
  }

  // 1초마다 한번씩 동작하는 함수들
  if (millis() - previousMillis_2 >= interval_1s)
  {
    previousMillis_2 = millis();

    loop_LCD(); // LCD 출력 함수
  }

  /*  필요에 따라 활성화 
  loop__serialMonitor_rtc_ds3231(); // 디버깅용 시리얼 출력 함수
  serialMonitor();               // 디버깅용 시리얼 출력 함수
  */
}