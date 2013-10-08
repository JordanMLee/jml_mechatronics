/*
==========================
 Instructor: LT Dahlen
 Class: Mechatronics
 Assignment: Homework 1
 Date Due: 06SEP13
 ==========================
 */

int buz_pin = 2;    
long vel = 20000;
int LED_pin = 13;
int led = 13;

/**
 * @note: The code for the mario theme song was copied from
 * an example found at Gala, Tiago. "Arduino + Piezo Speaker
 * = Super Mario!" The Random Bit. WordPress.com, 21 Nov. 
 * 2011. Web. 5 Sept. 2013. <Arduino + Piezo Speaker = Super 
 * Mario!>.
 */

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd (8, 7, 4, 3, 12, 17);
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.

  lcd.setCursor(0, 0);
  //task 1
  lcd.print("Jordan Lee");

    #define C       1911
    #define C1      1804
    #define D       1703
    #define Eb      1607
    #define E       1517
    #define F       1432
    #define F1      1352
    #define G       1276
    #define Ab      1204
    #define A       1136
    #define Bb      1073
    #define B       1012
    #define c       955
    #define c1      902
    #define d       851
    #define eb      803
    #define e       758
    #define f       716
    #define f1      676
    #define g       638
    #define ab      602
    #define a       568
    #define bb      536
    #define b       506
    
    #define p       0  //pause


  pinMode(buz_pin, OUTPUT);
  delay(2000);

  //====task3====
  /**@note
   *code was taken from the blink example on arduino
   */
  pinMode(led, OUTPUT);     
  for (int counter = 0; counter < 9; counter++){

    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(170);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(170);               // wait for a second
  }
  //====task3end ====



} // end setup loop

//int melod[] = {
//  e, e, e, c, e, g, G, c, G, E, A, B, Bb, A, G, e, g, a, f, g, e, c, d, B, c};
//int ritmo[] = {
//  6, 12, 12, 6, 12, 24, 24, 18, 18, 18, 12, 12, 6, 12, 8, 8, 8, 12, 6, 12, 12, 6, 6, 6, 12};

void loop() {
int melod[] = {
  e, e, e, c, e, g, G, c, G, E, A, B, Bb, A, G, e, g, a, f, g, e, c, d, B, c};
int ritmo[] = {
  6, 12, 12, 6, 12, 24, 24, 18, 18, 18, 12, 12, 6, 12, 8, 8, 8, 12, 6, 12, 12, 6, 6, 6, 12};

  
    //=======
  /**
   * Code snippet was obtained from this tutorial on arduino
   * http://arduino.cc/en/Tutorial/LiquidCrystalScroll
   */
  //task 2
  for (int positionCounter = 0; positionCounter < 17; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight(); 
    // wait a bit:
    delay(200);
  } // end for loop


  //=====

  lcd.clear();

//task 6
//  lcd.clear();
//  int Values = analogRead(0);
//  lcd.setCursor(0,1);
//  lcd.print(Values);
//  delay(100);



  //task 4
  //=== continuation of mario code
  for (int i=0; i<42; i++) {
    int tom = melod[i];
    int tempo = ritmo[i];

    long tvalue = tempo * vel;

    tocar(tom, tvalue);

    delayMicroseconds(1000); //pausa entre notas!
  } // end for loop
  delay(1000);
} //end loop loop

void tocar(int tom, long tempo_value) {
  long tempo_gasto = 0;
  while (tempo_gasto < tempo_value) {
    digitalWrite(buz_pin, HIGH);
    delayMicroseconds(tom / 2);

    digitalWrite(buz_pin, LOW);
    delayMicroseconds(tom/2);

    tempo_gasto += tom;
  } // end while loop
} // end tocar loop

