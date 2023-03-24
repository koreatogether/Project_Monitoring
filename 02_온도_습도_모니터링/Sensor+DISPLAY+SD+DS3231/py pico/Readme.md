
# Arduino IDE + RP2040 PICO + 온습도 로거 코드

## 코드 2차 생성 및 테스트 한 사람  :
 - koreatogetherfactory

## 소스코드
 - 1개의 ino 파일로 되어 있음
  
## 프로젝트 저장소 폴더 구성

보드별로 폴더가 만들어졌으며  2023.03.24 현재는 아두이노 와 파이 피코의 내용만 있음
- 코드
  - 01_LCD ==> 파이피코 + LCD + 온습도 로거
  - 02_TESING_OLED_ROTERYENCORDER ==> 앞으로도 테스트해야할 코드
  - 03_example_Module ==> 각 모듈별 파이피코랑 연결해서 테스트해본 코드
- Readme.md  ==> 프로젝트 설명 및 기록
- 핀맵.png ==> 핀맵 이미지
- 
## 배선도

```c++
Hardware  ||  External VCC   ||  DHT22( 5V , 3.3V 모두 가능)
                      3.3V   ||   VCC
                      GND    ||   GND
                     GPIO 2  ||   DATA


Hardware  ||  External VCC   ||  LCD 16x2 ( 5V 로만 표기되어 있으나 3.3V 테스트 안해봄 )
                      5V     ||   VCC
                      GND    ||   GND
                     GPIO 4  ||   SDA
                     GPIO 5  ||   SCL

Hardware  ||  External VCC   ||  SDCARD @ YUROBOT ( 3.3V  , 5V 가능 )
                      3.3V   ||   VCC
                      GND    ||   GND
                     GPIO 17 ||   CS
                     GPIO 19 ||   MOSI
                     GPIO 16 ||   MISO
                     GPIO 18 ||   SCK

Hardware  ||  External VCC   ||  RTC_DS3231 ( 5V , 3.3V 모두 가능)
                      3.3V   ||   VCC
                      GND    ||   GND
                      GPIO 4 ||   SDA
                      GPIO 5 ||   SCL

```

## 사용된 라이브러리

  - DHT22 [저장소로 이동하기](https://github.com/dvarrel/DHT22)
  - LiquidCrystal_I2C  [저장소로 이동하기](https://github.com/johnrickman/LiquidCrystal_I2C)
  - uRTClib             [저장소로 이동하기](https://github.com/Naguissa/uRTCLib)
  - RP2040_SD          [저장소로 이동하기](https://github.com/khoih-prog/RP2040_SD)
  - 1차 권리는  각 소스 저작자의 라이센스를 따름
  - 2차 편집에 대한 것은 [MIT License](https://choosealicense.com/licenses/mit/)

## 예정된 진행사항

- OLED + 로터리엔코더 또는 버튼으로  메뉴 구성 및 온도 그래프 추가하기
-
## 라이센스

- MIT License [라이센스 설명 페이지가기](https://opensource.org/licenses/MIT)
- 1차 권리는 각 소스 저작자의 라이센스를 따름

## 프로젝트 목적
- 파이피코 보드를 가지고 로그 + 메뉴 구성 + 그래프 출력 + 그외 센서들도 들어갈 자리 만드는 것 

## 기능

- 1초마다 DHT22 로부터 온도 / 습도 데이타 받고 이를 LCD에 출력함
- 2초마다 SD 카드에 온도 / 습도 데이타 저장
- 전원 리셋시 SD 생성 파일은 기존 파일의 존재 여부를 따져 , 존재한다면 파일명 뒤에 숫자를 증가 및 신규 파일 생성

## 중요변경 사항
- U8glib -> U8g2 로 변경 ( 002 버전 code부터 적용 )
- simple dht 라이브러리 -> DHT22 라이브러리 ( 002 버전 code부터 적용 )
- 아두이노 SD 라이브러리 -> RP2040_SD 라이브러리 ( 002 버전 code부터 적용 )
- setup() 과  loop() 의 위치를 맨 아래로 수정함 ( 002 버전 code부터 적용 )
- u8g2 라이브러와 인터럽트 충돌로 도저히 방법을 못 찾아 adafruit ssd1306 라이브러리로 변경함 ( 003 버전 code부터 적용 )
- RTClib 라이브러리 먹통되는 현상으로 uRTClib 으로 바꿈

## 참고 자료

- 아래 기록들은 정리 되지 않은 프로젝트 진행 기록들임

- 부품 : DHT22, OLED, SD카드, DS3231RTC, RotaryEncorder , 브레드보드용 전원 공급 보드
  - DS3231 RTC 상세 설명 : https://components101.com/modules/ds3231-rtc-module-pinout-circuit-datasheet
  - YUROBOT SDCARD 상세 설명 : https://www.devicemart.co.kr/goods/view?no=1311842

- 보드 : raspberry pi pico( no wifi version)
- 기능 : 온습도, 시간, 날짜, SD카드에 저장, RotaryEncorder로 메뉴 선택
- 베이스 코드 : arduino nano 로 만든 008 버전
- 주의사항 :  파이피코 보드에서 전원 끌어다 쓰지 말것 ( 브레드보드용 전원 공급 보드 사용할것 )
- 원가 분석( 23.03.11 기준 , 네이버 쇼핑)
  - DHT22 : 3,500원 (PCB 형태)
  - OLED : 3,000원
  - SDCARD@YUROBOT : 2,000원
  - DS3231RTC : 3,000원
  - RotaryEncorder : 700원
  - TTL LEVEL CONVERTER 5V <-> 3.3V(4채널기준 ) : 500원
  - 브레드보드용 전원 공급 보드 : 3,000원
  - raspberry pi pico( no wifi version) : 5,000원
  - 총합 : 20,700원

- 피코보드 업로드 방법
  - [업로드 안내 사이트 1 가기 ](https://blog.naver.com/PostView.naver?blogId=jcosmoss&logNo=222457510932&categoryNo=0&parentCategoryNo=56&currentPage=1)
  - [업로드 안내 사이트 2 가기 ](https://cafe.naver.com/dgarduino/14716)

  - 최초 업로드 하는 동안만 계속 부트버튼 누르면 되고 wrote ~ 라는 빨강색 메세지가 뜨면 부트버튼 떼도 된다.
    아두이노 연결 usb를 뺐다 다시 끼고 포트선택란에 보면 파이피코 com 번호가 보일 것이다 .선택
    이젠 부트 버튼 안 눌러도 업로드가 가능해진다.

- 진행 사항(글이 길어질경우 맨 아래로 이동)
  - 2023.03.11 : 001 버전 시작
  - 2023.03.12 : 로터리 엔코더 기초 코드 공부
  - 2023.03.12 #2 :
    - 파이피코 전원 공급 방법 공부 ( 왠만하면  GPIO 40에 연결하지 말것 )
    - VSYS(GPIO 39) 에 다이오드로 연결해서 외부전원공급할것 최소 2볼트 필요
    - 이렇게하면 코딩도 가능하고 업로드도 가능하다. - 이건 유튜브 내용잘살펴볼것
    - 36번핀  3.3V OUTPUT MAX 300mA 출력이 가능하다(공식문서 참고)
  - 2023.03.16 : pico 보드와 개별 모듈 연결 및 테스트
  - 2023.03.16 #1 : SD카드 동작 확인함 , 새로운 파일이 있으면 +1을 생성해서 겹치지 않는 것 확인함
  - 2023.03.16 #2 :
    - SD카드 로그파일 생성시 현재날짜 생성되도록 수정함
    - 관련 힌트는  1) C:\Users\User\Documents\Arduino\libraries\SD\src\utility\SdFile.cpp
    - 1) C:\Users\User\Documents\Arduino\libraries\RP2040_SD\src\utility\SdFile.cpp
      를 비교 및 검색해서 callback 함수를 찾아서 수정된 부분만 코드에 적용함
  - 2023.03.17 #1 : DHT 동작 및 SD 기록 확인함
  - 2023.03.17 #2 : 로터리 엔코더 동작 확인함
  - 2023.03.17 #3 :
    - u8g2 풀버퍼 코드의 첫 ~ 끝 실행까지 지연이 약 40ms 생기면서 나머지 변수에 의한 동작을
                    제어하는 코드들에 영향을 주고 있다.
    - 이를 해결 하려면 로터리 부분 코드를 인터럽트로 바꿔야 한다.
                    나와 같은 고민을 이미 하신 분의 [링크](https://m.blog.naver.com/ysahn2k/221865584318)
    - u8x8 모드로 할경우 3줄의 글자줄을 쓰는데 15ms이 걸린다.
                    firstPage() 모드로 할 경우 풀버퍼 모드랑 비슷하게 40ms의 지연이 생긴다.
  - 2023.03.19 #1 :
    - 로터리 엔코더 + u8g2( full buffer , page buffer , u8x8 ) 동작에서 문제가 있음을 발견함
    - 해외 포럼에 이에 대한 토론이 있었으나 질문자는 펄스 카운팅으로 회피했다고 하는데 이걸 ???
  - 2023.03.20 #1 : 라이브러리들 다시 하나씩 깨워서 충돌 여부 체크 하고 있음 , 충돌 나는게 있는 것같음
  - 2023.03.20 #2 : 충돌아는 라이브러리 RTClib.h -> RP2040_RTC.h 로 변경 예정 으로 잡았으나 이 라이브러리는 ds3231 용이 아닌 pico 내부 rtc 제어용이다.
  - 2023.03.21 #1 : adafruit ssd1306 라이브러리 버그가 있음  [토론 페이지 링크](https://forum.arduino.cc/t/library-pgmspace/1077639/9)
  - 2023.03.22 #1 : RTClib.h  => 대체 1 라이브러리 "uRTCLib.h" 를 테스트중  => 작동 이상없이됨 , 다만 SD 카드에 날짜 넘기는 부분???
  - 2023.03.22 #2 : 현재 ROTERY , OLED , DHT22 , RTC , SD ( 파일 날짜 기입빼곤 )까지 동작 확인함
  - 2023.03.22 #3 : 003 -> 004 버전을 바꾸면서  SD 카드 파일 생성시 현재 날짜가 들어가게 해주는 부분 + 코드 정리 까지 예정
  - 2023.03.23 #1 : 
    - 마지막에서 rtc.minute()에 영향을 끼치는 라이브러리가존재 하기에 방법 찾는중
    - adafruit ssd1306 라이브러리 가 uRTCLib 라이브러리에 영향을 끼침 , 수정 할 수준이 안되어 포기 
```C++
                    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
                        {
                          Serial.println(F("SSD1306 allocation failed"));
                          for (;;)
                            ; // Don't proceed, loop forever
                        }
```


 - 2023.03.24 #1 : adafruit를 유지할지 , crystal lcd로 가야 할지 고민 중 ....
 - 2023.03.24 #2 : crystal lcd로 가기로 결정함 , 16x2 i2c lcd 모듈을 사용할 예정 , oled, and rotery encoder는 제외함
 - 2023.03.24 #3 : 에러는 뜨는데 컴파일 되고 업로드 이상 없고 단기 동작에는 이상없음 , 장기 동작 테스트 들어감
 - 2023.03.24 #4 : 코드 정리 및 주석 단순 화 및 readme.md 이전할것들 분리하기  , 코드들은 최대한 사용자 정의 함수로 뺄 것

---

## 추후 살펴봐야 할것들
  - u8g2 + 인터럽트 충돌 
  - adafruit ssd1306  와 uRTCLib 충돌

## 참고 사이트
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
- crystal lcd 설명 1 : https://blog.naver.com/yunc26/222622178769


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


