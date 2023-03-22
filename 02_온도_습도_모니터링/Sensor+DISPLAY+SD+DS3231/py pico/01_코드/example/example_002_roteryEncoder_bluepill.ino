
#include <Arduino.h>

#define encoder0PinA 15
#define encoder0PinB 14
#define encoder0PinBtn 13

volatile long encoder0Pos = 0;
volatile int8_t encoder0PinALast;
int encoder0BtnPinState;

void setup() {
  Serial.begin(9600);
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0PinBtn, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, CHANGE);
  encoder0PinALast = digitalRead(encoder0PinA);
}

void loop() {
  encoder0BtnPinState = digitalRead(encoder0PinBtn);
  Serial.print("encoder0Pos = ");
  Serial.print(encoder0Pos);
  Serial.print(" , encoder0BtnPinState = ");
  Serial.println(encoder0BtnPinState);  
}

void doEncoderA() {
  int8_t encoder0PinAValue = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (encoder0PinAValue == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos++;        
    } else {
      encoder0Pos--;      
    }
  }
  encoder0PinALast = encoder0PinAValue;
}

// void doEncoderB() {
//   int8_t encoder0PinBValue = digitalRead(encoder0PinB);
//   if ((encoder0PinALast == HIGH) && (encoder0PinBValue == LOW)) {
//     if (digitalRead(encoder0PinA) == LOW) {
//       encoder0Pos--;      
//     } else {
//       encoder0Pos++;      
//     }
//   }
//   encoder0PinALast = encoder0PinBValue;
// }