#include <dht.h>  // By  Rob Tillaart 함수입니다. 타 dht 함수와 사용방법이 살짝 틀립니다.

dht DHT;
#define DHT22_PIN 12  // DHT22 Signal을 D12에 배정


int preTime;
int nowTime;
int intervelTime = 2100;  // dht 함수 실행 간격 msec

void setup() {
  Serial.begin(9600);
}

void loop() {
  
 if(int i = 0 ; i < 5 ; i ++)
 {
 
    nowTime = millis();
        if((nowTime - preTime) >= intervelTime ) 
        {
            
            DHT.read22(DHT22_PIN);  // DHT11이면 22 -> 11로 변경 하면 됨  , 이 구문이 없으면 값을 못 읽음    
            Serial.println(DHT.humidity , 1);  // 시리얼 모니터링 필요없으면 주석처리 할것
            Serial.println(DHT.temperature , 1); // 시리얼 모니터링 필요없으면 주석처리 할것       
            preTime = nowTime;          

        }
        Serial.println(" 2초 마다 실행 되는것 확인 구문");
 }

        int h = DHT.humidity;
        Serial.print(" 이건 습도 int 값:");
        Serial.println(h  ,1);
        delay(3000);
}


