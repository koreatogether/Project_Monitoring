/*
저렴이 중국 버전 엔코더를 가지고 정확한 동작을 읽는 코드를 해보려고 했는데 
좀만 빨라지만 값이 누락되거나 2-3개씩 읽히는 현상이 있다.

여러가지 코드를 테스트 해봤고 
아래와 같은 폴링방식의 코드에서 마무리 해야 할 것같습니다. 

코드는  모노 104 소자 연결된 브레드보드 상에서테스트 했습니다. 
고로 채터링에 대한 코드는 없습니다. 

그리고 어디선가 본것같은데 
로터리엔코더가 원상 복귀하면서 정확하지 않는 동작때문에 값이 튄다라는글을 본적이 있습니다. 

*/


#include <Arduino.h>

#define PIN_A   14
#define PIN_B   15

int32_t position = 0;   
volatile int32_t encoderCount = 0;

void encoderISR()
{
    if(digitalRead(PIN_B))
      encoderCount++;
    else
      encoderCount--;
}

void setup()
{
  pinMode(PIN_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_A),  encoderISR, FALLING);

  Serial.begin(115200);
}

void loop()
{
  if (position != encoderCount)
  {
    position = encoderCount;
    Serial.println(position);
  }
}

/*
만약  // ^encounter를 메뉴구성을 위해서 숫자를 제한 하려고 한다면 
아래 같이 ISR 내부에서 IF문을 사용해서 제한을 걸어주면 됩니다. 

#include <Arduino.h>

#define PIN_A   14
#define PIN_B   15

int32_t position = 0;
volatile int32_t encoderCount = 0;

void encoderISR()
{
    if(digitalRead(PIN_B))
      encoderCount++;
    else
      encoderCount--;
  if ( encoderCount <= 0)
  {
    encoderCount = 0;
  }

  if ( encoderCount >= 2 )
  {
    encoderCount = 2;
  }
}

void setup()
{
  pinMode(PIN_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_A),  encoderISR, FALLING);

  Serial.begin(115200);
}

void loop()
{
  if (position != encoderCount)
  {
    position = encoderCount;
    Serial.println(position);
  }

}
*/
