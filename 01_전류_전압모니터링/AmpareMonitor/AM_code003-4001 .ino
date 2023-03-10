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
                    -  이건 물어볼것
                    
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
#include <U8glib.h>

Adafruit_INA219 ina219;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);

unsigned long pretime = 0;
unsigned long  nowTime = 0;

const int buttonPin = 2; // 영점 조절 버튼을 위한 핀 번호 설정 

int oledpageNumber = 1;  // oled 페이지 넘기는데 필요한 설정

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

float avrshuntVoltage = 0;
float avrbusVoltage = 0;
float avrcurrent_mA = 0;
float avrpower_mW = 0;


//int signal_Pin= A4;
int S0 = 5;
int S1 = 6;
int S2 = 7;


void setup(void) 
{
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
    u8g.firstPage();
                do
                {
                 } while (u8g.nextPage());
  delay(100);
  

}

void loop(void) 
{
  
        for ( int i= 0 ; i <100 ; i++)   
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
        
        avrshuntVoltage = avrshuntVoltage / 100;
        avrbusVoltage = avrbusVoltage / 100;
        avrcurrent_mA = avrcurrent_mA / 100;
        avrpower_mW = avrpower_mW / 100;
        
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
                u8g.setFont(u8g_font_fub14);
                //u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"Ampare");
                u8g.drawStr(40,40,"Meter");
               
                } while (u8g.nextPage());
                delay(2000);
                


        }
        else
            oledpageNumber = (oledpageNumber <= 3) ? oledpageNumber + 1 : 1;  // 끝에 1번은 switch  번호 외에 있을 화면 (아무런 동작이 없는 화면 )
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
                u8g.setFont(u8g_font_fub14);
                //u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"LED");
                u8g.drawStr(40,40,"monitor");
                  if(avrcurrent_mA >= 1000)
                  {
                  float current_A = avrcurrent_mA * 1000;
                  u8g.setPrintPos(10 , 60);
                  u8g.print(current_A);
                  u8g.drawStr(65,60,"A");      
                  }
                    if(avrcurrent_mA <1000)
                    {
                    u8g.setPrintPos(10 , 60);
                    u8g.print(avrcurrent_mA);
                    u8g.drawStr(65,60,"mA");   
                    }
                
                    
                    //u8g.drawStr(0,50,"Do not Touch!!");
                    } while (u8g.nextPage());
          
        
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
                u8g.setFont(u8g_font_fub14);
                //u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"Oled");
                u8g.drawStr(40,40,"Ampare");
                 if(avrcurrent_mA >= 1000)
                  {
                  float current_A = avrcurrent_mA * 1000;
                  u8g.setPrintPos(10 , 60);
                  u8g.print(current_A);
                  u8g.drawStr(65,60,"A");      
                  }
                    if(avrcurrent_mA <1000)
                    {
                    u8g.setPrintPos(10 , 60);
                    u8g.print(avrcurrent_mA);
                    u8g.drawStr(65,60,"mA");   
                    }
                
                    
                    //u8g.drawStr(0,50,"Do not Touch!!");
                    } while (u8g.nextPage());
            
      
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
                u8g.setFont(u8g_font_fub14);
                //u8g.setFont(u8g_font_unifont);    
                u8g.drawStr(0,20,"5v");
                u8g.drawStr(40,40,"Ampare");
                 if(avrcurrent_mA >= 1000)
                  {
                  float current_A = avrcurrent_mA * 1000;
                  u8g.setPrintPos(10 , 60);
                  u8g.print(current_A);
                  u8g.drawStr(65,60,"A");      
                  }
                    if(avrcurrent_mA <1000)
                    {
                    u8g.setPrintPos(10 , 60);
                    u8g.print(avrcurrent_mA);
                    u8g.drawStr(65,60,"mA");   
                    }
                
                    
                    //u8g.drawStr(0,50,"Do not Touch!!");
                    } while (u8g.nextPage());
            
                
            break;


         case 4:  //  u8glib 그래프 테스트 

        
         Serial.println("switch 4 com in ~~~");
         
         u8g.firstPage();
                do
                {
                //u8g.setFont(u8g_font_fub14);
                //u8g.setFont(u8g_font_unifont);    
               




                } while (u8g.nextPage());
            
                
            break;

        default:
        break;

    }

}