/*
 *
 *  Mechatronics Lab #2 template  
 * 
 */


//const char fox_message[45] = "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG";
//const char instruction_message[150] = "On the Arduino IDE, Open 'Tools' 'Serial Port'.  Select 9600 baud and carriage return line endings.";


#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd (8, 7, 4, 3, 12, 17);

#define LED_pin 13 
#define buz_pin 2

#define K1_pin 10
#define K2_pin  11


void setup() {
  //pinMode(K1_pin, OUTPUT);
 //digitalWrite(K1_pin, HIGH);
 // analogWrite(K1_pin, 255);
  //analogWrite(K2_pin, 255);
 //delay(4000);
 
 digitalWrite(13, LOW);
    lcd.begin(16, 2);              // set up the LCD's number of columns and rows: 
    Serial.begin(9600);          // start serial port at 9600 bps:
  
    //relays K1 and K2 de-energized
    analogWrite(K1_pin, 0);
    analogWrite(K2_pin, 0);
    
    //send to USART         
    Serial.println("This lab demonstrates step start control over DC motors.");                    // echo string on serial port
    
    //send to LCD
    lcd.clear(); 
    lcd.print("Step Start Lab");
    lcd.setCursor(0, 1);
    lcd.print("Jordan Lee");
    delay(2000);

/*
    happy_sound();
    happy_sound();
    happy_sound();
    //A2.5 tasks complete
    //LCD_scroll_long_ROM_string(fox_message); 
    //LCD_scroll_long_ROM_string(instruction_message); */
}


void loop() {
  
    //char buf[32];
    /*
    lcd.clear(); 
    lcd.print("Awaiting command");
    lcd.setCursor(0, 1);
    lcd.print("from serial port");
    */
    /*
    serial_gets(buf, 32);                   //
    Serial.println(buf);                    // echo string on serial port
    
    if( strcmp(buf,"start") == 0){          // A zero value indicates that both strings are equal.
        lcd.clear(); 
        lcd.print("You said start!");
        happy_sound();
        delay(2000);
    }
    else if( strcmp(buf,"stop") == 0){      
        
        lcd.clear(); 
        lcd.print("STOP!");
        happy_sound();
        delay(2000);
    }
    else{
        sad_sound();
        LCD_scroll_long_ROM_string("Sorry, I don't understand..."); 
        
    } */
    //A2.6 t=3secs
    delay(1000);
    Serial.println("Standby, a motor start sequence has been initiated");
    //send to LCD
    lcd.clear(); 
    lcd.print("CAUTION!");
    lcd.setCursor(0, 1);
    lcd.print("Motor Starting");
    delay(2000);
    //beep three times alerting the operator that the motor is going to start
    tone(buz_pin, 554);    //C
    delay(500);
    noTone(buz_pin);
    delay(500);
    tone(buz_pin, 554);    //C
    delay(500);
    noTone(buz_pin);
    delay(500);
    tone(buz_pin, 554);    //C
    delay(500);
    noTone(buz_pin);
    delay(500);
    
    
    //A2.7 t=6secs
    Serial.println("Motor is accelerating");
    lcd.clear(); 
    lcd.print("Motor ramping");
    delay(2000);
    //activate relay K2
    analogWrite(K2_pin,255);
    delay(5000);
   
    //A2.8 t=9secs
    Serial.println("Full power engaged");
    lcd.clear();
    lcd.print("Motor running");
    delay(2000);
    //activate relay K1
    analogWrite(K1_pin, 255);
    delay(5000);
    //de-energize relay K2
    analogWrite(K2_pin,0);
    
    //A2.9 t=12 seconds
    Serial.println("Motor secured and coasting to a stop.");
    lcd.clear();
    lcd.print("Motor secured");
    delay(5000);
    //de-energivze relay K1
    analogWrite(K1_pin,0);
    //de-energize relay K2
    analogWrite(K2_pin,0);
    

    
    
    
}





/**
 * @brief This function allows you to send long ROM based text strings to the LCD display.  It automatically performs the scrolling operation.     
 *
 * @param *in_str is a pointer to the string to be sent. 
 *
 * @return void 
 *
 * @note This function depends on the Arduino LCD library. 
 *     
 * @note The LCD has an address for each character displayed on the screen.  It also has memory for characters that are 
 * to the right of the screen.  These “unseen” locations may be revealed by commanding the LCD to scroll right.
 *
 * This function breaks the message into units of 16 characters. 
 * 
 *     1) clear the LCD display
 *     2) load the first 16 characters in the memory location immediately to the right of the LCD
 *     3) scroll left 16 times with a delay between each step allowing the operator to view the message
 *     4) load the first 32 character of the message starting at the first visible LDC position (yes you are 
 *        resending the first 16 characters of the message)
 *     5) scroll left 32 times
 *     6) Load the next 32 characters
 *     7) repeat steps 5 and 6 until complete message has been loaded
 */
 /*
void LCD_scroll_long_ROM_string(const char *in_str) {
 
    char buf[32];
    uint8_t str_length = strlen(in_str);
    uint8_t i, j;
    
    lcd.clear();                                    // Load the first 16 char in the LCD memory to the unseen positions to the right of the display
    lcd.setCursor(16, 0);
    snprintf(buf, 16, "%s", in_str);                // fancy way of parsing 16 characters from string
    lcd.print(buf); 
    for(j = 1; j< 16; j++){
        lcd.scrollDisplayLeft();
        delay(200);
    } 
    
    for (i = 0; i < (str_length / 16) + 1; i++){ 
        lcd.clear();  
        lcd.setCursor(0, 0);
        snprintf(buf, 32, "%s", in_str + 16 * i );
        lcd.print(buf); 
        for(j = 1; j< 16; j++){
            lcd.scrollDisplayLeft();
            delay(200);
        } 
    }    
}




/**
 * @brief This the the quintessential get string function.  It uses a bad combination of C and Arduino commands.  It represents an intermediate step on the way out of Arduino land.     
 *
 * @param *buf is a pointer to the memory location where the string is to be stored. 
 * @param max_char is used to limit the number of characters to be returned thus preventing buffer overflow.  Set this equal to buffer size to prevent overflow.
 *
 * @return void 
 *
 * @note This is an blocking function and should be used with care as the program is stalled (blocked) until the user sends a serial command.   
 *
 * @todo The max_char functionality needs to be tested.
 * 
 * @todo Add a timeout function and return a value indicating if new data is present.  The time out could be based on the Arduino millis() function.      
 *//*
void serial_gets(char *buf, uint8_t max_char){
  
    char c;
    uint8_t cnt = 0;
  
    while(1){ 
        while (Serial.available() <= 0);
        c = Serial.read();
    
        if((c == 0x0D) | (cnt >= max_char - 1)){
            *buf = 0x00;
            break;
        }
        else{
            *buf = c;
            buf ++;
            cnt ++;
        } 
    }      
}




void happy_sound(void){
    tone(buz_pin, 554);    //C
    delay(100);
    tone(buz_pin, 659);    //E
    delay(100);
    tone(buz_pin, 784);    //G
    delay(100);
    tone(buz_pin, 1047);   //C
    delay(100);
    noTone(buz_pin); 
}

void sad_sound(void){
    tone(buz_pin, 1047);    //C
    delay(100);
    tone(buz_pin, 784);    //G
    delay(100);
    tone(buz_pin, 659);    //E
    delay(100);
    tone(buz_pin, 500);   //wrong note!
    delay(300);
    noTone(buz_pin); 
}
*/
