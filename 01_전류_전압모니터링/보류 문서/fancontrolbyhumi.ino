/*
버전 관리
숫자 없는 버전은  dht22 , oled  핀 배정 및 작동 확인 및 millis를 활용까지 한 버전입니다.
버전 002 는 모터 드라이버 + 12 fan 1ea 붙여보는 버전입니다.
버전 003 은 팬 1ea -> 2ea로 늘리는 버전입니다. 
*/



#include <dht.h>  // By  Rob Tillaart 함수입니다. 타 dht 함수와 사용방법이 살짝 틀립니다.
#include "U8glib.h"

dht DHT;
#define DHT22_PIN 12  // 나노 D8 번을 DHT22_PIN에 대입한다.

int preTime;
int nowTime;
int intervelTime = 5500;  // dht 함수 실행 간격 msec

int preTime2;
int nowTime2;
int intervelTime2 = 5500;

// Oled 0.96 구동 드라이버 호출  , SCL = A5 , SDA = A4
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);	// Fast I2C / TWI 


//모터 드라이버 모듈  A측 제어핀
int motorDriver_input1Pin = 3;
int motorDriver_input2Pin = 4;
int motorDriver_EnbAPin = 5;  // analogWrite가 되는 디지털핀
//모터 드라이버 모듈  B측 제어핀
int motorDriver_input3Pin = 7;
int motorDriver_input4Pin = 8;
int motorDriver_EnbBPin = 6; // analogWrite가 되는 디지털핀


void setup()
{
    Serial.begin(9600);    
}

void loop()
{
    
    //  *** dht ***
    nowTime = millis();
    if((nowTime - preTime) >= intervelTime ) 
    {
        
        DHT.read22(DHT22_PIN);  // DHT11이면 22 -> 11로 변경 하면 됨  , 이 구문이 없으면 값을 못 읽음    
        Serial.println(DHT.humidity , 1);  // 시리얼 모니터링 필요없으면 주석처리 할것
        Serial.println(DHT.temperature , 1); // 시리얼 모니터링 필요없으면 주석처리 할것       
        preTime = nowTime;
    }
    //  *** dht end ***

    // *** oled ***
    u8g.firstPage();  
    do {
        draw();
       } while( u8g.nextPage() );


    
    

}

void draw(void) {
  
  u8g.setFont(u8g_font_unifont);// graphic commands to redraw the complete screen should be placed here    
  u8g.drawStr( 0, 22, "Hello World!");//u8g.setFont(u8g_font_osb21);
}
