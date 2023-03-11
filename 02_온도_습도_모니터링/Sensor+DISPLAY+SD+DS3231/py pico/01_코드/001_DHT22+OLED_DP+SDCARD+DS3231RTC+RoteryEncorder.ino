/*
@배선도
Hardware  ||  pico   ||  DHT22( 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP9    ||   DATA

Hardware  ||  pico   ||  OLED 128x64 SSD1306 ( 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP8    ||   SDA
              GP7    ||   SCL

Hardware  ||  pico   ||  SDCARD@YUROBOT ( ONLY 5V , 3.3V 모두 가능)
              3.3V   ||   VCC
              GND    ||   GND
              GP6    ||   CS
              GP5    ||   MOSI
              GP4    ||   MISO
              GP3    ||   SCK

Hardware  ||  pico   ||  DS3231RTC
              3.3V   ||   VCC
              GND    ||   GND
              GP2    ||   SDA
              GP1    ||   SCL

Hardware  ||  pico   ||  RotaryEncorder
              3.3V   ||   VCC
              GND    ||   GND
              GP0    ||   SW
              GP10   ||   DT
              GP11   ||   CLK

 @부품 : DHT22, OLED, SD카드, DS3231RTC, RotaryEncorder , TTL LEVEL CONVERTER 5V <-> 3.3V , 브레드보드용 전원 공급 보드
 @보드 : raspberry pi pico( no wifi version)
 @기능 : 온습도, 시간, 날짜, SD카드에 저장, RotaryEncorder로 메뉴 선택
 @베이스 코드 : arduino nano 로 만든 008 버전
 @주의사항 : 5V <-> 3.3V 변환기 필요
 @원가 분석( 23.03.11 기준 , 네이버 쇼핑)
  - DHT22 : 3,500원 (PCB 형태)
  - OLED : 3,000원
  - SDCARD@YUROBOT : 2,000원
  - DS3231RTC : 3,000원 
  - RotaryEncorder : 700원
  - TTL LEVEL CONVERTER 5V <-> 3.3V(4채널기준 ) : 500원
  - 브레드보드용 전원 공급 보드 : 3,000원
  - raspberry pi pico( no wifi version) : 5,000원
  - 총합 : 20,700원
 
 @진행 사항(글이 길어질경우 맨 아래로 이동)
  - 2023.03.11 : 001 버전 시작
*/