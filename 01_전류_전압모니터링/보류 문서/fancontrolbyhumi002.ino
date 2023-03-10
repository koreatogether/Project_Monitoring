/*
버전 관리
숫자 없는 버전은  dht22 , oled  핀 배정 및 작동 확인 및 millis를 활용까지 한 버전입니다.
버전 002 는 모터 드라이버 + 12 fan 1ea 그리고 2개까지 작동 + 온도 / 습도  5회 평균값 을 비교해서 다음 동작을 하게 하는 버전

RED PCB 의 드라이버는 고전압에 따른 점퍼핀 제거를 안하여 내부손상되었습니다.
VCC 에 12이상! 전압을 넣을경우 반드시 12V GND 글씨위에 있는 점퍼핀을 제거 해야합니다.
7V ~ 11V 사이는 제거 안하고 사용해도 내부적으로 7805가 5V를 발생 , 내부회로에
쓰일수 있게 해줍니다.

GREN PCB의 경우 S1 이라는 하는 푸쉬 버튼이 있는데 이게 위에 언급한 점퍼핀하고
같은 역활을 합니다.
눌려있으면 +5V 에서  5V가 안 나옵니다.
올라와 있으면 +5V에서 5V가 출력 되는데 이를 아두이노 또는 다른 기기의 5V로 
활용이 가능합니다.

정리하자면 외부 입력전압이 12V 이상 ~ L298N의 데이타 시트상의 제원에 맞는 화면글씨뜨게
VCC 라면  점퍼핀 제거 또는 S1이 눌려져 있어야 제대로 작동합니다. 

analogWrite가 되는 디지털핀에  ENA 또는 ENB 핀을 연결해야 모터제어가 됩니다.
예를 들어 ENA에 D12번을연결하면  MAX 255 에서 작동되나 100정도로 낮추면 작동을 안합니다. 
원인은 analogWrite를 표현못하는 디지털핀이라그렇습니다. ( 핀맵 필히 참고 )

그래서 D5 , D6에 ENA , ENB를 연결하면 제대로 100값을 출력해주기에 모터 전압이 제대로 나옵니다.

다만 fan의 경우  255 맥스 출력이 아니고선 고주파음이 발생 합니다.



버전 0004 는 oled 부분에 온도 / 습도 / 팬 1 속도 ? 또는 % 가동 / 팬 2 의 속도 또는 % 가동 표시 하는 버전입니다.
버전 005 는 004버전까자의 데이타를 가지고 온습도에 따른 팬 을 껐다가 켜고  이를 oled에서 확인 할수 있는 버전입니다.
버전 006 은 oled ui 개선 버전 이자 마지막 버전으로 최대한 한글로 표현하는게 목적입니다.
*/

/*
dht22  -  5v -> arduino nano 5v
          Signal -> arduino nano D12          
          nc -> none 
          GND -> arduino nano GND

Oled 0.96 Inch - 5v -> arduino nano 5v
                 gnd -> arduino nano GND
                 SCL -> arduino nano A5
                 SDA  -> arduino nano A4

L298N (GREEN) -  +12V -> VCC
                 GND  -> arduino nano GND
                  < A SIDE >
                 arduino nano+5v  -> 5V
                 arduino nano D3  -> INA Pin
                 arduino nano D4  -> INB Pin
                 arduino nano D5  -> ENA

                < B SIDE >
                 arduino nano+5v  -> 5V
                 arduino nano D7  -> INC Pin
                 arduino nano D8  -> IND Pin
                 arduino nano D6  -> ENB

*/


#include <dht.h>  // By  Rob Tillaart 함수입니다. 타 dht 함수와 사용방법이 살짝 틀립니다.
//#include "U8glib.h"

dht DHT;
#define DHT22_PIN 12  // DHT22 Signal을 D12에 배정

int preTime;
int nowTime;
int intervelTime = 2200;  // dht 함수 실행 간격 msec

int preTime2;
int nowTime2;
int intervelTime2 = 5500;

int sensorCount = 0;

int humiditySum = 0;
int humidityAverage = 0;
int temperatureSum = 0;
int temperatureAverage = 0;

// Oled 0.96 구동 드라이버 호출  , SCL = A5 , SDA = A4
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // Fast I2C / TWI 


//모터 드라이버 모듈  A측 제어핀
#define motorDriver_input1Pin 3
#define motorDriver_input2Pin 4
#define motorDriver_EnbAPin 5  // analogWrite가 되는 디지털핀
//모터 드라이버 모듈  B측 제어핀
#define motorDriver_input3Pin 7
#define motorDriver_input4Pin 8
#define motorDriver_EnbBPin 6 // analogWrite가 되는 디지털핀

int motor_Delay = 10000; //10초


void setup()
{
    Serial.begin(9600);
  pinMode(motorDriver_input1Pin, OUTPUT);             // 제어 1번핀 출력모드 설정
  pinMode(motorDriver_input2Pin, OUTPUT);             // 제어 2번핀 출력모드 설정
  pinMode(motorDriver_EnbAPin, OUTPUT);               // A측 PWM 출력 설정
  pinMode(motorDriver_input3Pin, OUTPUT);             // 제어 3번핀 출력모드 설정
  pinMode(motorDriver_input4Pin, OUTPUT);             // 제어 4번핀 출력모드 설정
  pinMode(motorDriver_EnbBPin, OUTPUT);               // B측 PWM 출력 설정
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
        sensorCount = sensorCount + 1;  // 밑에 if( sensor == 5)를 위한 + 입니다.
        humiditySum = humiditySum + DHT.humidity; // 밑에 if( sensor == 5)를 위한 + 입니다.
        temperatureSum = temperatureSum + DHT.temperature; // 밑에 if( sensor == 5)를 위한 + 입니다.       

        if( sensorCount == 5)  // 위 if문에 의해서 sensorCount 값이 5에 도달하면
        {
         Serial.println(" sensorCount = 5 if문에 들어왔습니다.");
         delay(1000);
         humidityAverage = humiditySum / 5;  // 밑에 if( humidityAverage > 40 || temperatureAverage > 30)를 위한 평균내는 것입니다.
         temperatureAverage = temperatureSum / 5; // 밑에 if( humidityAverage > 40 || temperatureAverage > 30)를 위한 평균내는 것입니다.
         sensorCount = 0;  // 0으로 처리안하면 sensorCount값이 계속 ++ 해져 if조건문에 못 넘어옵니다.
         humiditySum = 0;  // 0으로 처리안하면 평균값이 계속 커져 if문의 조건을 넘기게 됩니다.
         temperatureSum = 0; // 0으로 처리안하면 평균값이 계속 커져 if문의 조건을 넘기게 됩니다.
         Serial.print("평균온도값은 = ");     // 확인용 구문 
         Serial.println(temperatureAverage); // 확인용 구문   
         Serial.print("평균습도값은 = ");  // 확인용 구문
         Serial.println(humidityAverage); // 확인용 구문
            if( humidityAverage > 40 || temperatureAverage > 30) //  습도 또는 온도값이 정해진 값을 넘으면 
            {
             Serial.println(" 평균 습도/ 온도 값이 제한수치 40％ / 30℃ 을 넘었습니다.");
             Serial.println(" 팬을 10초 동안 작동하겠습니다.");
             Serial.println(" 팬 작동하는 동안 온습도 측정은 멈춥니다.");
             motorDriver_forward();
             delay(motor_Delay);
             motorDriver_stop();
             humidityAverage = 0;
             temperatureAverage = 0;
            }

        }
    }
    //  *** dht end ***

    // *** oled ***
    /*
    u8g.firstPage();  
    do {
        draw();
       } while( u8g.nextPage() );
    */
  
 
}     


/*
void draw(void) 
{  
  u8g.setFont(u8g_font_unifont);// graphic commands to redraw the complete screen should be placed here    
  u8g.drawStr( 0, 22, "Hello World!");//u8g.setFont(u8g_font_osb21);
}
*/

void motorDriver_forward() //FAN 시계 방향으로 회전 ( 보통팬은 역회전 없습니다)
{
  digitalWrite(motorDriver_input1Pin, HIGH); 
  digitalWrite(motorDriver_input2Pin, LOW);
  analogWrite(motorDriver_EnbAPin, 255);  //A 측 모터 속도를 최대로 설정
  digitalWrite(motorDriver_input3Pin, HIGH); 
  digitalWrite(motorDriver_input4Pin, LOW);
  analogWrite(motorDriver_EnbBPin, 255);  // B측모터 속도를 최대로 설정
}

void motorDriver_stop()
{
  digitalWrite(motorDriver_input1Pin, HIGH);         //모터가 시계 방향으로 회전
  digitalWrite(motorDriver_input2Pin, HIGH);
  digitalWrite(motorDriver_input3Pin, HIGH);         //모터가 시계 방향으로 회전
  digitalWrite(motorDriver_input4Pin, HIGH);
  
}

  