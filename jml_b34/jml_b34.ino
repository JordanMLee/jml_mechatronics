#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd (8, 7, 4, 3, 12, 17);

#define LED_pin 13 
#define buz_pin 2

#define K1_pin 10
#define K2_pin  11

#define tachometer A2


void setup() {
//  while(1)
//  {
//    analogWrite(K1_pin, 128);
//    delay(5000);
//    analogWrite(K1_pin, 255);
//    delay(5000);
//  }
    lcd.begin(16, 2);              // set up the LCD's number of columns and rows: 
    Serial.begin(9600);          // start serial port at 9600 bps:
    pinMode(K2_pin, OUTPUT);
    pinMode(K1_pin, OUTPUT);
    digitalWrite(K2_pin, LOW);
    digitalWrite(K1_pin, LOW);
}


void loop() {
  digitalWrite(K2_pin, HIGH);
  delayMicroseconds(78);
  digitalWrite(K2_pin, LOW);
  delayMicroseconds(50);
  
//  for(int i = 0; i < 101; i++)
//  {
//    digitalWrite(K2_pin,HIGH);
//  delayMicroseconds(i);
//  digitalWrite(K2_pin, LOW);
//  delayMicroseconds(100-i);
//    Serial.println(i);
//    //delay(500);
//  }
   }
