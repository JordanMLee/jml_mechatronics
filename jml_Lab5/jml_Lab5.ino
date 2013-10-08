#define joystick A0   //joystick connected to pin A0
#define base A1       //base voltage connected to pin A1
#define collector A2  //collector voltage connected to pin A2
#define basesrc 10    //square wave at pin 10

#include "configuration.h"
#include "USART.c"
void setup()
{
  init_USART( );                 // Enable USART ISR
  
}
void loop()
{
  float input = analogRead(joystick);
  
  analogWrite(basesrc, (int)input /4 ); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  
  analogRead(base);
  float Ib = (5(DC%) - base * (5/1023))/(100000);
  
  analogRead(collector);
  float Vc = collector*(5/1023); //voltage at collector scaled
  
  float Ic = (5 - Vc) / 100; // current through collector equals (5V - Vc) / 100 Ohms
  float time = millis();
  float B = Ic/Ib;
  Serial.println("Millis = ",time, ", Joystick = ",input, ", Ib = ", Ib, " uA, Ic = ", Ic, " mA, B = ", B, "\n");
  
  
  
}

