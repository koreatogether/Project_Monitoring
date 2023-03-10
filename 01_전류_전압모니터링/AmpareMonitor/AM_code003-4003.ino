//  Adafruit  ina219 기본 예제 
// 가장 정확한 코드라고 보면 된다.  
// AM_code001.ino부터 시작함   ->  1개의 OLED 와 1개의 전류 센서 작동  =>> 성공함   002로 넘어갈것
// AM_code002.ino를 시작함 --> 1개의 OLED + 1개의 전류 센서 작동 + 화면넘어가게 할것 ( 필라멘트 무게 소스 사용)  - 성공함 
// 
// AM_code003.ino를 시작함 -  1개의 OLED +  mux + 3개 전류센서 읽기 + 버튼으로 화면 넘기기 ( 1번 2번 3번 센서 각각 값 )
//  추가 -  전류가 1000 mA넘으면 A로 표시되게 IF문 추가 함   * 자리수에 따라서 단위따라가게 IF 넣어볼것 0
//AM_code003_2001 - 1개의 센서 + 3개의 oled 작동 테스트 , oled가 마음대로 움직이지 않음 
//AM_code003_4001 - 3개의 ina219 센서 + 1개의 oled ( 화면 버튼 넘어감 ) , 2001 버전보단 4001 버전이 중요하므로 4001부터 완성할것 
//AM_code003_4001 #2 - 전체에 대한 구성은 평균값 100회 를 통해서 안정된 값을 제공 에 성공했는데  아두이노 5v에서 소모되는 전류에 대한 계산이 맞지 않는듯하다 .
                    //-  이건 물어볼것
// //AM_code003_4002 - 4001 + 막대 그래프 추가를 한것입니다.       
// u8g2lib 풀버퍼 함수가 우노 메모리를 넘어서는 결과를 가져오기에 버퍼작은 것을 사용하거나 타 보드로 갈아타야 하거나 나노보드로 바꿔야 하겠음 
// 5v에서 나오는 전압은 측정되나 전류는 측정 불가 - 회로에 대한 이해가 필요 
//  아날로그 그래프 / 보기 좋은 막대 그래프까지 추가 되었음 
// 스케치 메모리가 89%이므로 이상의 추가 코드는 우선 제외 하고 전체적인 짜임새를 다듬을 것 
// 짜임새는 AM_code003_4003.ino 파일로 시작합니다.

             
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


#include <Wire.h>
#include <Adafruit_INA219.h>
#include <U8glib.h>  //호환 우노 보드와의 문제로 2로 갈아탐 
//#include <U8g2lib.h>

Adafruit_INA219 ina219;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//u8g2lin용 디스플레이 드라이버 _U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


unsigned long pretime = 0;
unsigned long  nowTime = 0;

const int buttonPin = 2; // 영점 조절 버튼을 위한 핀 번호 설정 

int oledpageNumber = 1;  // oled 페이지 넘기는데 필요한 설정

// float 변수들은 전류 센서에 필요한 것입니다.
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

float avrshuntVoltage = 0;
float avrbusVoltage = 0;
float avrcurrent_mA = 0;
float avrpower_mW = 0;

// int S0 ~ S2 는 74HC4051 스위치 모듈에 필요한 변수 입니다. (핀번호 부여 )
int S0 = 5;
int S1 = 6;
int S2 = 7;

int largPos = 0;  // case 4 의 막대 그리파 관련 변수 입니다.

int Count = 30;  // for 문의 변수입니다.

const int WIDTH=128;
const int HEIGHT=64;
const int LENGTH=WIDTH;

//const int analogInPin = A0;  // -> 이게 변수이므로 이걸 잘 활용할것 
//int analogInValue = 0;

int x;
int y[LENGTH];


// 페이지 6용 변수들 
int val; 
 
//그래픽 처리 변수   // 페이지 6용 변수들 
int level; 

//아놀로그 게이지를 위한 변수 

int px1=64, py1=50;   // 바늘침의 시작 위치 
int px2, py2;         // 바늘침의 그려지는 위치 
int circle, pin;      // 게이지 큰원, 바늘침 길이 
int angle;            // 변수값에 따라 바늘침이 움직여야 하는 각도값 
double rad;           // px2, py2 위치값을 알아내기 위한 라디언값 
 










void setup(void) 
{
     //u8g2.begin();  // u8g2lib 시작 선언
     
     Wire.begin(); 
     pinMode(2, INPUT_PULLUP);  // 화면전환 버튼용 
     pinMode(S0, OUTPUT);
     pinMode(S1, OUTPUT);
     pinMode(S2, OUTPUT);

  Serial.begin(115200);
    while (!Serial) 
    {
        // will pause Zero, Leonardo, etc until serial console opens
        delay(1);
    }

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
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");
  
    // oled 화면 초기화 
    //u8g2.clearBuffer();
  delay(100);
// x , y 그래프 관련 setup 선언입니다.
  x = 0;
  clearY();

// 아날로그 그래프를 위한 setup 선언 
 circle = 40;          // px1, py1을 기준으로 큰 원을 63으로 설정 
  pin = 44;             // 바늘침 크기 설정 


}

void loop(void) 
{
  
        for ( int i= 0 ; i < Count ; i++)   
        {
          shuntvoltage = ina219.getShuntVoltage_mV();
          busvoltage = ina219.getBusVoltage_V();
          current_mA = ina219.getCurrent_mA();
          power_mW = ina219.getPower_mW();
          
          avrshuntVoltage = avrshuntVoltage + shuntvoltage;
          avrbusVoltage = avrbusVoltage + busvoltage;
          avrcurrent_mA = avrcurrent_mA + current_mA;
          avrpower_mW = avrpower_mW + power_mW;
        }
        
        avrshuntVoltage = avrshuntVoltage / Count;
        avrbusVoltage = avrbusVoltage / Count;
        avrcurrent_mA = avrcurrent_mA / Count;
        avrpower_mW = avrpower_mW / Count;
        
        loadvoltage = busvoltage + (shuntvoltage / 1000);
        
       
        
        Serial.print("Shunt Voltage: "); Serial.print(avrshuntVoltage); Serial.println(" mV");
        Serial.print("Bus Voltage:   "); Serial.print(avrbusVoltage); Serial.println(" V");
        
        Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");       
        Serial.print("Current:       "); Serial.print(avrcurrent_mA); Serial.println(" mA");
        Serial.print("Power:         "); Serial.print(avrpower_mW); Serial.println(" mW");
        
        Serial.println("");
    
   


     
      if (!digitalRead(2))
    {
        //Serial.println(digitalRead(2)); // 체크용 
        long long pushtime = millis();
        while (!digitalRead(2));
       if (millis() - pushtime >= 300)
        {
            u8g.firstPage();
                do
                {
                //u8g2.clearBuffer();
                //u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"Ampare");
                u8g.drawStr(40,40,"Meter");
                //u8g2.sendBuffer();    
                delay(2000);
                 } while (u8g.nextPage());
                


        }
        else
            oledpageNumber = (oledpageNumber <= 6) ? oledpageNumber + 1 : 1;  // 끝에 1번은 switch  번호 외에 있을 화면 (아무런 동작이 없는 화면 )
    }
    
   

    // **** oled 화면 넘김 구동 코드 시작 ****
    switch (oledpageNumber)
    {
        case 1:
        
       
        digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,LOW); //채널 1 선택(Y0 또는 S0)    
         
        Serial.println("switch 1 com in ~~~");
        Serial.print(digitalRead(5));
        Serial.print(digitalRead(6));  
        Serial.println(digitalRead(7)); 
        

         u8g.firstPage();
                do
                {
                //u8g2.clearBuffer();
                //u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"LED");
                u8g.drawStr(40,40,"monitor");
                  if(avrcurrent_mA >= 1000)
                  {
                  float current_A = avrcurrent_mA * 1000;
                  //u8g2.setCursor(10 , 60);
                  u8g.setPrintPos(10,60)  ;
                  u8g.print(current_A);
                  u8g.drawStr(65,60,"A");      
                  }
                    if(avrcurrent_mA <1000)
                    {
                    u8g.setPrintPos(10 , 60);
                    u8g.print(avrcurrent_mA);
                    u8g.drawStr(65,60,"mA");   
                    }


                     } while (u8g.nextPage());
                //u8g2.sendBuffer();

                
                    
                   
               
          
        
        break;

        case 2:  // Oled 0.96 전류 감시 

        digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,HIGH); //채널 2 선택(Y1 또는 S1)
        Serial.print(digitalRead(5));
        Serial.print(digitalRead(6));  
        Serial.println(digitalRead(7)); 
        Serial.println("switch 2 com in ~~~");
            

             u8g.firstPage();
                do
                {
                  //u8g2.clearBuffer();
                //u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"Oled");
                u8g.drawStr(40,40,"Ampare");
                 if(avrcurrent_mA >= 1000)
                  {
                  float current_A = avrcurrent_mA * 1000;
                  //u8g.setCursor(10 , 60);
                  u8g.setPrintPos(10,60);
                  u8g.print(current_A);
                  u8g.drawStr(65,60,"A");      
                  }
                    if(avrcurrent_mA <1000)
                    {
                    //u8g2.setCursor(10 , 60);
                    u8g.setPrintPos(10,60);
                    u8g.print(avrcurrent_mA);
                    u8g.drawStr(65,60,"mA");   
                    }

                     } while (u8g.nextPage());
                   //u8g2.sendBuffer();           
      
        break;

        case 3:  // 아두이노 5볼트 전류 감시  , 변동 폭이 크므로 평균으로 처리 할것 기본 10회부터 테스트 
        
        digitalWrite(S0,HIGH); digitalWrite(S1,LOW); digitalWrite(S2,LOW); //채널 5 선택 ( Y4 OR S4)
        Serial.print(digitalRead(5));
        Serial.print(digitalRead(6));  
        Serial.println(digitalRead(7)); 
         Serial.println("switch 3 com in ~~~");
         


          u8g.firstPage();
                do
                {
                //u8g2.clearBuffer();
                //u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"5v");
                u8g.drawStr(40,40,"Ampare");
                 if(avrcurrent_mA >= 1000)
                  {
                  float current_A = avrcurrent_mA * 1000;
                  //u8g2.setCursor(10 , 60);
                  u8g.setPrintPos(10 , 60);
                  u8g.print(current_A);
                  u8g.drawStr(65,60,"A");      
                  }
                    if(avrcurrent_mA <1000)
                    {
                    //u8g2.setCursor(10 , 60);
                    u8g.setPrintPos(10 , 60);
                    u8g.print(avrcurrent_mA);
                    u8g.drawStr(65,60,"mA");   
                    }

                     } while (u8g.nextPage());
                //u8g2.sendBuffer();
                  
                
            break;


         case 4:  //  u8glib  막대바 그래프 테스트 

        
         Serial.println("switch 4 com in ~~~");
         

          u8g.firstPage();
                do
                {
              //u8g2.clearBuffer();
                draw();
              //u8g2.sendBuffer();
             } while (u8g.nextPage());

                
            break;

          case 5:  // x,y 그래프 테스트 
              // 대략 파악 했긴 했는데 
             //analogInValue = analogRead(analogInPin);  
             //y[x] = map(analogInValue, 0, 1023, HEIGHT-1, 0);;

             //y[x] = avrcurrent_mA;
              Serial.println("switch 5 com in ~~~");
              Serial.println(avrcurrent_mA);
             //y[x] = map(avrcurrent_mA , 0, 19 , HEIGHT-1, 0);  // 값이 위에서 아래로 배치된다. 즉 17mA는 위쪽에서부터 보인다.
             y[x] = map(avrcurrent_mA , 0, 50 , HEIGHT-1 , 0);   // 값이 아래에서 위로 배치된다. 
             //y[x] = map(random(0 ,200) , 0 , 200 , HEIGHT-1 , 0);  // 그래프 테스트용 랜덤 함수 
              

             u8g.firstPage();  
              do {
                drawY();
                  } while( u8g.nextPage() );
              
              //delay(10);

              x++;
              if(x >= WIDTH)
              {
                x = 0;
                clearY();
              }
            
            break;


            case 6:

             val = random(0, 100); 
            Disp(); 
            break;
            
            case 7:
            val = random(100, 3000); 
            DispAnalog(); 
  
            break;

        default:
        break;

    }

    if(largPos < 108){
        largPos++;  } 
  else{
    largPos = 0;
  }  

}





//막대 그래프 그리는 함수입니다.
void draw()
{
    u8g.drawBox(10,15,largPos,10);   
}

//  x,y 그래프 그리는 함수  지우는 함수와 그리는 함수 2개입니다.


void clearY()
{
  for(int i=0; i<LENGTH; i++)
  {
    y[i] = -1;
  }
}

void drawY()
{
  u8g.drawPixel(0, y[0]);
  for(int i=1; i<LENGTH; i++)
  {
    if(y[i]!=-1)
    {
      //u8g.drawPixel(i, y[i]);
      u8g.drawLine(i-1, y[i-1], i, y[i]);
    }
    else{
      break;
        }
  }

}


void Disp()   // 페이지 6용  막대그래프 함수 
{ 
  level = map(val, 0, 100, 0, 120);       // 레벨박스크기 
   
  u8g.firstPage();  
  do {              
    u8g.drawLine(3, 40, 123, 40); 
    u8g.drawLine(3, 63, 123, 63); 
    u8g.drawLine(3, 38, 3, 63); 
    u8g.drawLine(123, 38, 123, 63); 
    u8g.drawLine(92, 38, 92, 42); 
    u8g.drawLine(62, 38, 62, 42); 
    u8g.drawLine(32, 38, 32, 42); 
     
    u8g.setFont(u8g_font_profont11r);    // 6*10폰트 
    u8g.drawStr(110, 36, "100"); 
    u8g.drawStr(87, 36, "75"); 
    u8g.drawStr(57, 36, "50"); 
    u8g.drawStr(27, 36, "25"); 
    u8g.drawStr(0, 36, "0"); 
    u8g.drawStr(120, 20, "%"); 
 
    u8g.drawStr(0, 10, "VAL"); 
 
    u8g.setFont(u8g_font_profont29r);      //16*28폰트 
    if (val < 10) 
      u8g.setPrintPos(100, 20); 
    else if (val >= 10 && val <= 99) 
      u8g.setPrintPos(84, 20); 
    else 
      u8g.setPrintPos(68, 20); 
    u8g.print(val); 
 
    u8g.drawBox(3, 45, level, 14); 
  } 
  while( u8g.nextPage() ); 
}



void DispAnalog() // 아날로그 그래프 함수 입니다.
{ 
  angle = map(val, 0, 3000, 180, 360);    // 180~360구간만 사용 (****중요) 
  rad = angle * 3.14 / 180;               // 각도를 라디안값으로 변환 
  px2 = px1+(cos(rad)*pin);               // 바늘침 x좌표 위치 계산 
  py2 = py1+(sin(rad)*pin);               // 바늘침 y좌표 위치 계산 
   
  u8g.firstPage();  
  do {              
    // 겉에 가장 큰 원 2겹으로 
    u8g.drawCircle(px1, py1, circle, U8G_DRAW_UPPER_RIGHT); 
    u8g.drawCircle(px1, py1, circle, U8G_DRAW_UPPER_LEFT); 
    u8g.drawCircle(px1, py1, circle-2, U8G_DRAW_UPPER_RIGHT); 
    u8g.drawCircle(px1, py1, circle-2, U8G_DRAW_UPPER_LEFT); 
     
    u8g.drawLine(px1, py1, px2, py2);                      // 바늘침 그리기 
    u8g.drawDisc(px1, py1, 5, U8G_DRAW_UPPER_RIGHT);       // 바늘침 시작점 반원 그리기 
    u8g.drawDisc(px1, py1, 5, U8G_DRAW_UPPER_LEFT);         
 
    u8g.setFont(u8g_font_profont11r);                     // 6*10크기의 폰트를 사용 
    u8g.setPrintPos(50,30); 
    u8g.print(val); 
  } 
  while( u8g.nextPage() ); 
} 
