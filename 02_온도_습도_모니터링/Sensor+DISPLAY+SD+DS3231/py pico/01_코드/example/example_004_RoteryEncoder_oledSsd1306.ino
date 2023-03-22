/*
예제  interrupt_falling_RoteryEncode.ino 파일과
예제  Oledssd1306.ino파일을 합해서 메뉴 이동에 대한 테스트 예제입니다.
u8g2 폰트 사이트에서  1234567 + 폰트 이름 되어 있는 것은 숫자만 지원되는 폰트
                     abcde~ 123 + 폰트 이름 된것은  문자 + 숫자를 지원한다는 폰트

^@작업 내역
  - 23/03/19  #1 : 날짜 , 시간 의 폰트 , 위치 확정
  - 23/03/19  #2 : 움직이는 커서 위치 확정
  - 23/03/19  #3 : 메뉴 폰트 , 위치 확정
  - 23/03/19  #4 : 로터리엔코더 코딩 확정 및 움직임 제한 코드 추가됨
  - 23/03/19  #5 : u8g2 라이브러리와 로터리엔코더 충돌로 인하여 adafriut 라이브러리로 변경함
  - 23/03/20  #1 : 로터리엔코더 값 제한 및 값 변동 확인함
  - 23/03/20  #2 : adafruit는 폰트 색상에 대한 구문이 빠지면 글씨가 안 보인다. 그래서 BLACK 이라는 상수도 있다.


**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 *************************************************************************
 */


#include <Wire.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>



const int encoder0PinA = 15; // setup() 안의 인터럽트 구문에 들어가는 핀이 clk이다.
const int encoder0PinB = 14; // setup() 안의 인터럽트 구문에 들어가는 핀이 clk이다.
const int encoder0PinBtn = 13;

volatile long encoder0Pos = 0;
int newEncoder0Pos = 0;
volatile int8_t encoder0PinALast;
int encoder0BtnPinState;

void doEncoderA() // 인터럽트 주의사항을 잘 읽어보자. 안그럼 오차가 심해지고 부정확해진다.
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
  Serial.println(encoder0BtnPinState);
}

void setup()
{
  Serial.begin(9600);
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0PinBtn, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, CHANGE);  // 다른 엔코더 사용하면 필요해질지 몰라 남김 .
  encoder0PinALast = digitalRead(encoder0PinA);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
}

void loop()
{
  encoder0BtnPinState = digitalRead(encoder0PinBtn);
  menu_selectNumber();
  firstPage_LOOP();
  serialMonitor(); // 디버깅용 시리얼 출력 함수
}