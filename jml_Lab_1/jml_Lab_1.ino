/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD and shows the time.
 
The circuit:

    * LCD RS pin to digital pin 8                   
    * LCD Enable pin to digital pin 7
    * LCD D4 pin to digital pin 4
    * LCD D5 pin to digital pin 3
    * LCD D6 pin to digital pin 12
    * LCD D7 pin to digital pin 17
    * LCD R/W pin to ground
    * 10K resistor:
    * ends to +5V and ground
    * wiper to LCD VO pin (pin 3)
     
     Note pins selected to make wire runs short
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified for use with the Arduino mini pro 09 Aug 2013
 by Aaron Dahlen
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd (8, 7, 4, 3, 12, 17);

int LED_pin = 13;
int buz_pin = 2;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
   
  lcd.setCursor(0, 0);
  lcd.print("Jordan Lee");
  
  lcd.setCursor(0, 1);
  lcd.print("San Leandro");
  
  digitalWrite(LED_pin, HIGH);
  delay(1000);
  
  digitalWrite(LED_pin, LOW);
 
  
  tone(buz_pin, 554);    //C
  delay(300);
  tone(buz_pin, 659);    //E
  delay(300);
  tone(buz_pin, 784);    //G
  delay(300);
  tone(buz_pin, 1047);   //C
  delay(500);
  noTone(buz_pin);

  lcd.clear();
  lcd.print("Counting:");

  
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  for (int i = 0; i <= 9 ; i++) {
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(i);
  
  } 
  
  tone(buz_pin, 784);    //G
  delay(500);
  noTone(buz_pin);
  //lcd.print(millis()/1000);
}

