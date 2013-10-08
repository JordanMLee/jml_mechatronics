#include <LiquidCrystal.h>
LiquidCrystal lcd (8,7,4,3,12,17);
void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Hello");
  delay(2000);
}
void loop(){
  lcd.clear();
  int joystickValues = analogRead(0);
  int joystickValues2 = analogRead(1);
  lcd.setCursor(0,1);
  lcd.print(joystickValues);
  lcd.setCursor(0,0);
  lcd.print(joystickValues2);
  delay(50);
}
