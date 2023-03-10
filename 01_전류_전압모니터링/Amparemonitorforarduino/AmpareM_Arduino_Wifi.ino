//아두이노 우노 , 또는 나노에서 5v 에서 쓸수 있는 전류 , 3v 에서 쓸수 있는 전류를 실시간으로 체크 및 경고하는 메세지 및 막대 그래프를 
// 통해서 보기 쉽게 만드는게 목적입니다. 

// 준비물 : oled 한개 , 버튼 1개 , mux 멀티플렉서 , 2개의 ina219센서 ( 만약 물리주소를 바꿀거면 멀티플렉스 필요 없음 ) , led 녹색 * 2 , 노랑 색 * 2 , 적색 * 2 
// 21-09-30 -> 버튼에 따라서  oled 화면 전환 및 값들이 제대로 나오게 설정 완료 
// 21-09-30 #1 -> 센서값 소프트웨어 필터링 방법 추가  -  추가함 
// 21-09-30 #2 - > 2 개의 화면 / 각 그래프 로 표현 완료 함  5v는 다듬어야 함 
// AmpreM_Arduino_esp8266f12.ino - esp로 옮겨서 추가 할거 하고 , 와이파이로 도전 해볼 것 
// 21-10-14 - 경고 LED 값 구간에 따라서 작동하고 , 상대방 채널 LED는 꺼지는 조건 추가 
// 와이파이 전용 보드로 옮기기 전에 회로도( OK) 하고 코드 정렬 해보기  (     )
// 21년 10월 04일 , Nano 버전 및 기판에 올림 최소한 버전 및 모듈활 버전 완료 .
// 그리하여 아래와 같이  Uno버전과 달리 배선 때문에 핀 번호등이 바뀜 
// 21-10-05 숫자 자릿수에 따른 텍스트 위치 if 문을 추가함 
// 21-10-08 Neopixel 적용으로 모자랐던 digitalPin을 확보함으로써 esp8266 nodemcu를 그대로 활용하기로 함 
//21-10-07 네오픽셀 빼고는 동작 완료 했으나 그래프에서 비율이 안 맞음 꼭 수정 해야함 

/* 계획한 핀 번호 부여는

DO -> NONE 
D1 -> SCL
D2 -> SDA
D3 -> PUSH BUTTON
D4 -> NeoPixel control
D5 -> S0
D6 -> S1
D7 -> S2

*/

/* 
8채널 선택  0 = LOW , 1 = HIGH   ** 칩이 CD74HC4051이라도 해도 핀별 출력이 맞는지 꼭 확인 할것  
 000 - Y0 (1 채널 )
 100 - Y1 (2 채널 ) 
 010 - Y2 (3 채널 )
 110 - Y3 (4 채널 )
 001 - Y4 (5 채널 )
 101 - Y5 (6 채널 )
 011 - Y6 (7 채널 )
 111 - Y7 (8 채널 )
 
*/


//#include <U8glib.h>   -> esp8266 nodemcu에서 안 먹힌다.
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_NeoPixel.h>  // led 대신 해서 네오픽셀로 교체 (핀수 부족 대안)



// oled 0.96 ssd1306 드라이버 호출 선언 
//U8GLIB_SSD1306_128X64 u8g2(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
// 만약 u8g2lib 을 쓰고 싶다면 아래의 드라이버 추천 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define PIN D4
#define NUMPIXELS 4
#define DELAYVAL 50 // 이건 밀리함수로 대체할것 !!
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



//밀리 함수 용 변수 
unsigned int pretime=0;
unsigned int pretime2=0;


// mux S0 ~ S2 변수 74HC4051 스위치 모듈에 필요한 변수 
const int S0 =D5;
const int S1 =D6;
const int S2 =D7;

// 디스플레이 화면 넘김용 버튼 변수 선업
const int buttonPin = D3;

// oled 페이지 넘기는데 필요한 변수 
 int oledpageNumber = 1;

// float 변수들은 전류 센서에 필요한 것입니다.

Adafruit_INA219 ina219;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

float sumshuntVoltage = 0;
float sumbusVoltage = 0;
float sumcurrent_mA = 0;
float sumpower_mW = 0;

// 3번째 방식 필터방식에 필요한 변수들 
float filter_3rd_shuntvoltage;  
float filter_3rd_busvoltage;
float filter_3rd_current_mA;
float filter_3rd_loadvoltage;
float filter_3rd_power_mW;

float sensitivity = 0.05;
int maxCurrent = 800;
int maxCurrent_3dot3V = 130;
int maxCurrent_5V = 670;

// 3.3 과 5v 에서의  기존 전류를 빼기 위한 추가 변수
float oledforCurrent3dot3V ;
float oledforCurrent5V ;


int largPos = 0;  // case 4 의 막대 그리파 관련 변수 입니다.

int Count = 2000;  // for 문의 변수입니다.

const int WIDTH=128;
const int HEIGHT=64;
const int LENGTH=WIDTH;

// x,y 좌표 그래프에 필요한 변수 
int x;
int y[LENGTH];


// 그래프쪽에서 필요한 변수이긴한데 쓸지 안 쓸지 미정 
int val;
 
 
//그래프 처리 변수
int level; 

//아놀로그 게이지 그래프 를 위한 변수  추후 맞게 수정하면 됨 

int px1=64, py1=50;   // 바늘침의 시작 위치 
int px2, py2;         // 바늘침의 그려지는 위치 
int circle, pin;      // 게이지 큰원, 바늘침 길이                            
int angle;            // 변수값에 따라 바늘침이 움직여야 하는 각도값 
double rad;           // px2, py2 위치값을 알아내기 위한 라디언값 
 




void setup()
{
 
    Wire.begin(); 
    u8g2.begin();    

    // 네오픽셀 시작 하겠다는 선언 
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)    
    pixels.clear();
    pixels.show();
    // 화면전환 버튼용 플로팅 현상 방지 풀업 쓰는 것 
    pinMode(D3, INPUT_PULLUP);  
    
    // mux 용 핀들  출력 선언 
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    digitalWrite(S0,LOW);
    digitalWrite(S1,LOW);
    digitalWrite(S2,LOW);
        
    
    
    //시리얼 통신 화면열때  화면시작을 화면열때까지 기다리는 구문 
    Serial.begin(115200);
    while (!Serial) 
    {
        // will pause Zero, Leonardo, etc until serial console opens
        delay(1);
    }

  // ina219 함수 기본 선언 
  uint32_t currentFrequency;
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) 
  {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):  // 이걸로 쓴 이유는 전류 한계치는 800으로 해놓을 것이기 때문입니다.
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");
  
    // oled 화면 초기화 
    //u8g2.clearBuffer();
  delay(100);

// x , y 그래프 관련 setup 선언입니다.
 // x = 0;
 // clearY();

// 아날로그 게이지 그래프를 위한 setup 선언 
 circle = 40;          // px1, py1을 기준으로 큰 원을 63으로 설정 
  pin = 44;             // 바늘침 크기 설정 


 shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        power_mW = ina219.getPower_mW();
        loadvoltage = busvoltage + (shuntvoltage / 1000);

        filter_3rd_shuntvoltage = filter_3rd_shuntvoltage ;
        filter_3rd_busvoltage = filter_3rd_busvoltage ;
        filter_3rd_current_mA = filter_3rd_current_mA ;
        filter_3rd_power_mW = filter_3rd_power_mW ;
        filter_3rd_loadvoltage = filter_3rd_loadvoltage ;

        


}



void loop()
{
      
      
      Serial.println(digitalRead(D3));
        pretime = millis();
        //pretime2 = millis();
        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        power_mW = ina219.getPower_mW();
        loadvoltage = busvoltage + (shuntvoltage / 1000);

        filter_3rd_shuntvoltage = filter_3rd_shuntvoltage * ( 1 - sensitivity) + shuntvoltage * sensitivity;
        filter_3rd_busvoltage = filter_3rd_busvoltage * ( 1 - sensitivity) + busvoltage * sensitivity;
        filter_3rd_current_mA = filter_3rd_current_mA * ( 1 - sensitivity) + current_mA * sensitivity;
        filter_3rd_power_mW = filter_3rd_power_mW * ( 1 - sensitivity) + power_mW * sensitivity;
        filter_3rd_loadvoltage = filter_3rd_loadvoltage * ( 1 - sensitivity) + loadvoltage * sensitivity;

        +

        // 여기 시리얼 출력 명령어는 회로에 흐르는 잔 전류까지 다 포함 된 것 입니다. 
        Serial.print("Shunt Voltage: "); Serial.print(filter_3rd_shuntvoltage); Serial.println(" mV");
                Serial.print("Bus Voltage:   "); Serial.print(filter_3rd_busvoltage); Serial.println(" V");
                Serial.print("Load Voltage:  "); Serial.print(filter_3rd_loadvoltage); Serial.println(" V");       
                Serial.print("Current:       "); Serial.print(filter_3rd_current_mA); Serial.println(" mA");
                Serial.print("Power:         "); Serial.print(filter_3rd_power_mW); Serial.println(" mW");
                
                Serial.println("");
            
        

        // 이 방식은  90 % 과 10%를 계속해서 더해주는 과정으로 된 방식으로  for 일반 평균 , 배열 평균 보다 더 나은 방식으로 소개되어 있다. 
        // 필터링 방식을 여러가지 이므로 공부 해볼 것 

       // 여기는 측정 전류 값에 따라서 led 를  1개 , 2개 , 3개 작동하는 구간입니다.
        
        if (!digitalRead(D3))
        {
            //Serial.println(digitalRead(2)); // 체크용 
            long long pushtime = millis();
            while (!digitalRead(D3));
          /*
        if (millis() - pushtime >= 300)
            {
                u8g2.firstPage();
                    do
                    {
                    //u8g2.clearBuffer();
                    //u8g2.setFont(u8g2_font_ncenB08_tr);
                    u8g2.setFont(u8g_font_unifont);    
                    u8g2.drawStr(0,20,"Ampare");
                    u8g2.drawStr(40,40,"Meter");
                    //u8g2.sendBuffer();    
                    delay(2000);
                    } while (u8g2.nextPage());
            }*/
        //else        
           oledpageNumber = (oledpageNumber <= 1) ? oledpageNumber + 1 : 1;  // 끝에 1번은 switch  번호 외에 있을 화면 (아무런 동작이 없는 화면 ) , 숫자 2는  0 , 1, 2 총 3페지를 구성하겠다는 뜻 
        
    }


    switch (oledpageNumber)
    {
       
        
        case 1:// 3.3V 채널   (NANO 에선 Y3 하고 )

        

        oledforCurrent3dot3V = filter_3rd_current_mA;
        digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,LOW); //Y3  선택(  )   -> 만약 이걸 안할경우 신호가 제대로 안 먹혀 이상 동작을 보인다.
        Serial.println(digitalRead(D5));
        Serial.println(digitalRead(D6));
        Serial.println(digitalRead(D7)); 
        Serial.println("switch 1 com in ~~~");
        
        // 경고 LED 작동 추가  // 돌아가는 방식을잘 알아둘것 많이 쓰일듯 하다 .  특히 가운데 조건이  && && ||를 쓰인 것에대한 고찰을 해볼것 

        pixels.clear();
        
        pixels.show();

         if(oledforCurrent3dot3V >= 0 && oledforCurrent3dot3V <= 12 )  // 녹색 LED 구간   0번
       {
             pixels.clear();
        pixels.show();
            pixels.setBrightness(64);
            pixels.setPixelColor(0 ,pixels.Color( 0 ,150 ,0));            
            pixels.show();
       }
              
    if (oledforCurrent3dot3V > 12 && oledforCurrent3dot3V <= 16 )   // 황색 LED 구간
        {
          pixels.clear();
        pixels.show();
        pixels.setBrightness(64);
            pixels.setPixelColor(1 ,pixels.Color( 255 ,255 ,0));
            pixels.setPixelColor(0 ,pixels.Color( 0 ,150 ,0));                        
            pixels.show();
        
        }
         else if( oledforCurrent3dot3V > 16)
        {
          pixels.clear();
        pixels.show();
        pixels.setBrightness(64);
        pixels.setPixelColor(1 ,pixels.Color( 255 ,255 ,0));
            pixels.setPixelColor(0 ,pixels.Color( 0 ,150 ,0));  
            pixels.setPixelColor(2 ,pixels.Color( 255 ,0 ,0));            
            pixels.show();
        }
    



    
         
         
         // 네오 픽셀 동작 구문 넣을 것 
      
      u8g2.clearBuffer();
      
      Disp_3dot3V();               

      u8g2.sendBuffer();
      
          
        
        break;

        case 2:  // 5V 채널      

        
        oledforCurrent5V = filter_3rd_current_mA;
        digitalWrite(S0,HIGH); digitalWrite(S1,HIGH); digitalWrite(S2,HIGH); // Y4 (  ) 
        Serial.println("switch 2 com in ~~~");
        Serial.println(digitalRead(D5));
        Serial.println(digitalRead(D6));
        Serial.println(digitalRead(D7));
            
          // 경고 LED 작동 추가  // 돌아가는 방식을잘 알아둘것 많이 쓰일듯 하다 .  특히 가운데 조건이  && && ||를 쓰인 것에대한 고찰을 해볼것 
    if(oledforCurrent5V >= 0 && oledforCurrent5V <= 40 ) // 녹색 LED 구간 조건
       {
            pixels.clear();
            pixels.show();
            pixels.setBrightness(64);        
            pixels.setPixelColor(0 ,pixels.Color( 0 ,150 ,0));              
            pixels.show();
       }
              
      if (oledforCurrent5V > 40 && oledforCurrent5V <=60 )  // 황색  LED 구간 조건 
        {
                    pixels.clear();
                pixels.show();
                pixels.setBrightness(64);
                pixels.setPixelColor(1 ,pixels.Color( 255 ,255 ,0));
                pixels.setPixelColor(0 ,pixels.Color( 0 ,150 ,0));          
                pixels.show();
        }
        else if (oledforCurrent5V > 60 )
        {
                    pixels.clear();
                pixels.show();
                pixels.setBrightness(64);
                pixels.setPixelColor(2 ,pixels.Color( 255 ,0 ,0));
                pixels.setPixelColor(1 ,pixels.Color( 255 ,255 ,0));
                pixels.setPixelColor(0 ,pixels.Color( 0 ,150 ,0));          
                pixels.show();

        }        
               
               
                
                pixels.setBrightness(64);
               
                pixels.setPixelColor(3 ,pixels.Color( 100 ,150 ,0));          
                pixels.show();
              
      u8g2.clearBuffer();

      Disp_5V();
      
      u8g2.sendBuffer();
      
        break;
        
               
        default:
        break;    


}
}





//막대 그래프 그리는 함수입니다.

/*void draw()
{
    u8g2.drawBox(10,15,largPos,10);   
}
*/
//  x,y 그래프 그리는 함수  지우는 함수와 그리는 함수 2개입니다.

/*
void clearY()
{
  for(int i=0; i<LENGTH; i++)
  {
    y[i] = -1;
  }
}

void drawY()
{
  u8g2.drawPixel(0, y[0]);
  for(int i=1; i<LENGTH; i++)
  {
    if(y[i]!=-1)
    {
      //u8g2.drawPixel(i, y[i]);
      u8g2.drawLine(i-1, y[i-1], i, y[i]);
    }
    else{
      break;
        }
  }

}
*/
void Disp_3dot3V()  //  막대그래프 함수 좀더 세련된 것 
{ 
  level = map(oledforCurrent3dot3V, 0, maxCurrent_3dot3V, 0, 120);       // 레벨박스크기 
  float percent_val_3dot3V = (oledforCurrent3dot3V/maxCurrent_3dot3V)*100;
  
  
    u8g2.drawLine(3, 40, 123, 40); 
    u8g2.drawLine(3, 63, 123, 63); 
    u8g2.drawLine(3, 38, 3, 63); 
    u8g2.drawLine(123, 38, 123, 63); 
    u8g2.drawLine(92, 38, 92, 42); 
    u8g2.drawLine(62, 38, 62, 42); 
    u8g2.drawLine(32, 38, 32, 42); 
     
    u8g2.setFont(u8g2_font_profont12_tr);    // 6*10폰트 
    u8g2.drawStr(107, 36, "130"); 
    u8g2.drawStr(79, 36, "97.5"); 
    u8g2.drawStr(56, 36, "65"); 
    u8g2.drawStr(19, 36, "32.5"); 
    u8g2.drawStr(0, 36, "0"); 
    
    //u8g2.setFont(u8g_font_timB14);     
    
    /*
    u8g2.setCursor(110, 20);     
    u8g2.print("\x25");
    */
    
    
    u8g2.setCursor(15, 20);     
    u8g2.print(filter_3rd_loadvoltage);   
    u8g2.drawStr(0,20,"V");
    //u8g2.setFont(u8g_font_timB12);
    if(oledforCurrent3dot3V <= 74)
    {         
    u8g2.setCursor(level+10 , 57);     
    u8g2.print(oledforCurrent3dot3V,1);
    //u8g2.setFont(u8g_font_profont11r);    // 6*10폰트 
    u8g2.drawStr(level+39,59,"mA");
    }
    else
    {
    u8g2.setCursor(70 , 20);     
    u8g2.print(oledforCurrent3dot3V,1);
    //u8g2.setFont(u8g_font_profont11r);    // 6*10폰트 
    u8g2.drawStr(110,20,"mA");
    }   
     
    
    /* 
    u8g2.setFont(u8g_font_timB14);      
    if ( percent_val_3dot3V < 10) 
      u8g2.setCursor(85, 20); 
    else if ( percent_val_3dot3V >= 10 &&  percent_val_3dot3V <= 99) 
      u8g2.setCursor(69, 20); 
    else 
      u8g2.setCursor(53, 20); 
    u8g2.print( percent_val_3dot3V,1); 
    */
    u8g2.drawBox(3, 45, level, 14); 
  
  
}

void Disp_5V()  //  막대그래프 함수 좀더 세련된 것 
{ 
  level = map(oledforCurrent5V, 0, maxCurrent_5V, 0, 120);       // 레벨박스크기 
  float percent_val_5V = (oledforCurrent5V/maxCurrent_5V)*100;
  
    Serial.println( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" );
    Serial.println( level );
  Serial.println( oledforCurrent5V );
    u8g2.drawLine(3, 40, 123, 40); 
    u8g2.drawLine(3, 63, 123, 63); 
    u8g2.drawLine(3, 38, 3, 63); 
    u8g2.drawLine(123, 38, 123, 63); 
    u8g2.drawLine(92, 38, 92, 42); 
    u8g2.drawLine(62, 38, 62, 42); 
    u8g2.drawLine(32, 38, 32, 42); 
     
    u8g2.setFont(u8g2_font_profont12_tr);    // 6*10폰트 
    u8g2.drawStr(107, 36,"800"); 
    u8g2.drawStr(84, 36, "600"); 
    u8g2.drawStr(54, 36, "400"); 
    u8g2.drawStr(24, 36, "200"); 
    u8g2.drawStr(0, 36, "0"); 
    
    //u8g2.setFont(u8g_font_timB14);        
    /*
    u8g2.setCursor(110, 20);     
    u8g2.print("\x25");
    */
    
    u8g2.drawStr(0,20,"V");
    u8g2.setCursor(15, 20);     
    u8g2.print(filter_3rd_loadvoltage);   
    //u8g2.setFont(u8g_font_timB12);
    if(oledforCurrent5V <= 420 )
    {

    
    u8g2.setCursor(level+10 , 57);     
    u8g2.print(oledforCurrent5V,1);
      if (oledforCurrent5V < 100)
      {
     // u8g2.setFont(u8g_font_profont11r);    // 6*10폰트 
      u8g2.drawStr(level+39,59,"mA");   
      }
      else 
        {
        //  u8g2.setFont(u8g_font_profont11r);    // 6*10폰트 
      u8g2.drawStr(level+48,59,"mA");   
        }
    }
    else
    {
      u8g2.setCursor(70 , 20);     
    u8g2.print(oledforCurrent5V,1);
   // u8g2.setFont(u8g_font_profont11r);    // 6*10폰트 
    u8g2.drawStr(110,20,"mA");   
    }

    
    /* 
    u8g2.setFont(u8g_font_timB14);      
    if ( percent_val_3dot3V < 10) 
      u8g2.setCursor(85, 20); 
    else if ( percent_val_3dot3V >= 10 &&  percent_val_3dot3V <= 99) 
      u8g2.setCursor(69, 20); 
    else 
      u8g2.setCursor(53, 20); 
    u8g2.print( percent_val_3dot3V,1); 
    */
    u8g2.drawBox(3, 45, level, 14); 
  
}


void neopixel_Move(){

  
}

/*
void DispAnalog() // 아날로그 그래프 함수 입니다.
{ 
  angle = map(val, 0, 3000, 180, 360);    // 180~360구간만 사용 (****중요) 
  rad = angle * 3.14 / 180;               // 각도를 라디안값으로 변환 
  px2 = px1+(cos(rad)*pin);               // 바늘침 x좌표 위치 계산 
  py2 = py1+(sin(rad)*pin);               // 바늘침 y좌표 위치 계산 
   
  u8g2.firstPage();  
  do {              
    // 겉에 가장 큰 원 2겹으로 
    u8g2.drawCircle(px1, py1, circle, U8G_DRAW_UPPER_RIGHT); 
    u8g2.drawCircle(px1, py1, circle, U8G_DRAW_UPPER_LEFT); 
    u8g2.drawCircle(px1, py1, circle-2, U8G_DRAW_UPPER_RIGHT); 
    u8g2.drawCircle(px1, py1, circle-2, U8G_DRAW_UPPER_LEFT); 
     
    u8g2.drawLine(px1, py1, px2, py2);                      // 바늘침 그리기 
    u8g2.drawDisc(px1, py1, 5, U8G_DRAW_UPPER_RIGHT);       // 바늘침 시작점 반원 그리기 
    u8g2.drawDisc(px1, py1, 5, U8G_DRAW_UPPER_LEFT);         
 
    u8g2.setFont(u8g_font_profont11r);                     // 6*10크기의 폰트를 사용 
    u8g2.setCursor(50,30); 
    u8g2.print(val); 
  } 
  while( u8g2.nextPage() ); 
} 
*/