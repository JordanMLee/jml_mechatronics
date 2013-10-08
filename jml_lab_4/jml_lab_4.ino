/*
 * Finite State Machine (FSM) and Interrupt Service Routine (ISR) template.
 *
 * Copyright 2013 Aaron P. Dahlen
 *
 *     APDahlen@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

// AVR Libc includes
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <stdint.h>
    #include <string.h>

// Project specific includes
    #include "configuration.h"
    #include "USART.c"
    #include <LiquidCrystal.h>


// Global variables

    volatile uint8_t start_flag = 0;
    volatile uint8_t engage_flag = 0;

    typedef enum{idle, starting, ramping, running, pwm, stopping, waiting, fault} e_states;

    volatile static e_states state = idle;

    LiquidCrystal lcd (8, 7, 4, 3, 12, A3);


void setup(){

    pinMode(LED_PIN, OUTPUT);
   

    analogWrite(K1_PIN, 0);
    analogWrite(K2_PIN, 0);
//    pinMode(K1_PIN, OUTPUT);
//    pinMode(K2_PIN, OUTPUT);
//******************************************
    init_timer_1_CTC(100);         // Enable the ISR
    init_timer_1_CTC(4);            //Enable the ISR for 4 times a second
    init_USART( );                 // Enable USART ISR

    lcd.begin(16, 2);              // Initial LCD display
    lcd.setCursor(0, 0);
    lcd.print("Mecha FSM lab");
    lcd.setCursor(0, 1);
    lcd.print("  17 Sep 13");
    delay(1000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("19200 8N1");
    lcd.setCursor(0, 1);
    lcd.print("line feed term");
    delay(2000);
    lcd.clear();
}



/*********************************************************************************
 *  ______  ____   _____   ______  _____  _____    ____   _    _  _   _  _____
 * |  ____|/ __ \ |  __ \ |  ____|/ ____||  __ \  / __ \ | |  | || \ | ||  __ \
 * | |__  | |  | || |__) || |__  | |  __ | |__) || |  | || |  | ||  \| || |  | |
 * |  __| | |  | ||  _  / |  __| | | |_ ||  _  / | |  | || |  | || . ` || |  | |
 * | |    | |__| || | \ \ | |____| |__| || | \ \ | |__| || |__| || |\  || |__| |
 * |_|     \____/ |_|  \_\|______|\_____||_|  \_\ \____/  \____/ |_| \_||_____/
 *
 ********************************************************************************/

ISR(USART_RX_vect){
/*
 * @brief This Interrupt Service Routine is called when a new character is received 
 * on the serial port.  As quickly as possible, the AVR transfers the character to 
 * a circular buffer.  The main loop code then retrieves data from this buffer.  
 * Observe that this mechanism allows data to be received at a high rate of speed 
 * independent of the main loop.
 * 
 * @todo This function should be in the USART.c file but there is a error 
 * "multiple definition of `  vector_18".  Apparently Arduino detects when an ISR
 * is in the main sketch.  If you place it somewhere else it is missed and replaced 
 * with the Arduino handler.  This is the source of the multiple definitions
 * error -  * see discussion @ http://forum.arduino.cc/index.php?topic=42153.0
 * Note that the three variables used in this function are all global is scope.  
 * This was done so that this function could be included in the projects main page.
 * 
 * @note The vector name is not "USART_RXC" as indicated in the data sheet.  
 * Instead, the ATMega328p "iom328p.h" file identified the name as "USART_RX_vect".

 * @note From the ATMEL data sheet "When interrupt driven data reception is used, the 
 * receive complete routine must read the received data from UDRn in order to
 * clear the RXCn Flag, otherwise a new interrupt will occur once the interrupt
 * routine terminates.
 *
 */

   ISR_main_link_circ_bufer[circ_buf_head] = UDR0;
   circ_buf_head++;
   circ_buf_head &= 0b00011111;
   //circ_buf_head &= 0b000111111;
}



ISR(TIMER1_COMPA_vect){

/* @brief This Interrupt Service Routine (ISR) serves as the 100 Hz heartbeat 
 * for the Arduino.  See the companion function init_timer_1_CTC for additional 
 * information.
 *
 * @ Note:
 *    1) Compiler generated code pushes status register and any used registers to stack.
 *    2) Calling a subroutine from the ISR causes compiler to save all 32 registers; a 
 *       slow operation (fact check). 
 *    3) Status and used registers are popped by compiler generated code.
 */

    static uint8_t cnt;
    static uint8_t LED_flag;
    static uint16_t time_in_state;
    static uint16_t cti;


     
       
    //  INSERT one second LED blink code here...
    if (++cti > 250)
    {
      digitalWrite(LED_PIN, HIGH);
      cti = 0; // reset timer
    } //end if
    else if(++cti < 250)    {
      digitalWrite(LED_PIN, LOW);
    }
      

    switch(state){

        case idle:

            if (start_flag){
                start_flag = 0;
                state = starting;
                time_in_state = 0;
                analogWrite(K1_PIN, 0);
                analogWrite(K2_PIN, 0);
             }
            break;


        case starting:

            
              if(++time_in_state > 400){
                state = ramping;
                time_in_state = 0;
                analogWrite(K1_PIN, 0);
                analogWrite(K2_PIN, 255);
            
            break;
            
        case ramping:
              time_in_state = 0;
              if (analogRead( mot_feedback) > 200){
                state = waiting;
                analogWrite(K1_PIN, 0);
                analogWrite(K2_PIN, 255);
                break;
              }
              else if (++time_in_state > 400)
              {
                state = fault;
                 analogWrite(K1_PIN, 0);
                analogWrite(K2_PIN, 0);
               break;
              }
              break;
                
              
        case waiting:
        
            if (engage_flag){
                engage_flag = 0;
                state = running;
                time_in_state = 0;
                analogWrite(K1_PIN, 0);
                //activate relay K2
                analogWrite(K2_PIN, 255);
            }
              }
            break;
        
        case running:

            if (++time_in_state > 400){
                state = pwm;
                time_in_state = 0;
                analogWrite(K1_PIN, 255);
                analogWrite(K2_PIN, 0);
            }
            break;
            
      case pwm:
       
            if (++time_in_state > 400){
                state = stopping;
                time_in_state = 0;
                //analogWrite(K1_PIN, 128);
                analogWrite(K2_PIN, 0);
            }
            
            break;    


        case stopping:

            if (++time_in_state > 400){
                state = idle;
                time_in_state = 0;
            }
            break;
          
        case fault:
        if (++time_in_state > 400){
            state = idle;
            time_in_state = 0;
            analogWrite(K1_PIN, 0);
            analogWrite(K2_PIN, 0);
        }
          break;  



        default:
            state = idle;
            break; 
    }
} // end of interupt service routine



/*********************************************************************************
 *  ____            _____  _  __ _____  _____    ____   _    _  _   _  _____
 * |  _ \    /\    / ____|| |/ // ____||  __ \  / __ \ | |  | || \ | ||  __ \
 * | |_) |  /  \  | |     | ' /| |  __ | |__) || |  | || |  | ||  \| || |  | |
 * |  _ <  / /\ \ | |     |  < | | |_ ||  _  / | |  | || |  | || . ` || |  | |
 * | |_) |/ ____ \| |____ | . \| |__| || | \ \ | |__| || |__| || |\  || |__| |
 * |____//_/    \_\\_____||_|\_\\_____||_|  \_\ \____/  \____/ |_| \_||_____/
 *
 ********************************************************************************/

void loop(){

    char buf[50];
    static uint8_t last_state = stopping;

/* This section of code is used to receive messages from the USART.  If a messages
 * is received a flag is set signaling the ISR to perform the operation.  This construct
 * was used to keep the ISR code small and fast.
 */

    switch(state){
        case idle:
           if(USART_is_string()){
                USART_gets(buf);
                if(strcmp(buf,"start") == 0){       // Zero indicates both strings are equal
                    start_flag = 1;
                    USART_puts(strcat(buf, "\n"));  //echo
                }
            }
            break;

        case starting:
        
            break;
            
        case ramping:
        if(USART_is_string()){
                USART_gets(buf);
                if(strcmp(buf,"engage") == 0){       // Zero indicates both strings are equal
                    engage_flag = 1;
                    USART_puts(strcat(buf, "\n"));  //echo
                }
            }
            break;
        
            

        case running:
            
            break;
            

            
        case pwm:
            break;

        case stopping:
            break;
            
        case waiting:
            break;
       
            
        case fault:
            break;

        default:
            break;
    }


/* This section of code executes once on change of states.  It is useful for
 * the LCD display and to send messages via the USART.
 */
    if(state != last_state){
        lcd.clear();
        last_state = state;

        switch(state){

            case idle:
                lcd.print("Awaiting user");
                USART_puts("Type \"start\" to initiate a startup sequence.\n");
                break;

            case starting:
                lcd.print("CAUTION!");
                lcd.setCursor(0, 1);
                lcd.print("Motor Starting");
                //USART_puts("Joystick up for next state.\n");
                USART_puts("Standby, a motor start sequence has been initiated\n");
                //beeping
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
                //delay(500);
                break;
                //add input checking
            case ramping:
                USART_puts("Motor is accelerating.");
                lcd.print("Motor ramping");
                delay(2000);
                USART_puts(" Full power at your command. Type \"engage\" to initiate\n");
                lcd.clear();
                lcd.print("type engage");
                lcd.setCursor(0,1);
                lcd.print("to continue");
                break;
                
            case running:
                
                 USART_puts("By your command - full power engage\n");
                lcd.print("Full power!");
                break;
               
             
            case pwm:
                USART_puts("Relay K1 duty cycle reduced to save power.\n");
                lcd.print("PWM K1");
                break;
                
              

            case stopping:
                USART_puts("Motor secured and coasting to a stop\n");
                lcd.print("Motor secured");
                break;
                
            case fault:
                lcd.print("FAULT STATE");
                USART_puts("\nEntered Fault State\n");
                break;
            case waiting:
            lcd.print("WAIT STATE");
                USART_puts("\nEntered Waiting State\n");
                break;

            default:
                lcd.print("unknown state");
                break;
        }
    }
}



void init_timer_1_CTC(long desired_ISR_freq){
/*
 * @brief Configure timer #1 to operate in Clear Timer on Capture Match (CTC Mode)
 *
 *      desired_ISR_freq = (f_clk / prescale value) /  Output Compare Registers
 *
 *   For example:
 *        Given a Arduino Uno: F_clk = 16 MHz
 *        let prescale = 64
 *        let desired ISR heartbeat = 100 Hz
 *
 *        if follows that OCR1A = 2500
 *
 * @param desired_ISR_freq is the desired operating frequency of the ISR
 * @param f_clk must be defined globally e.g., #define f_clk 16000000L
 *
 * @return void
 *
 * @note The prescale value is set manually in this function.  Refer to ATMEL ATmega48A/PA/88A/PA/168A/PA/328/P datasheet for specific settings.
 *
 * @warning There are no checks on the desired_ISR_freq parameter.  Use this function with caution.
 *
 * @warning Use of this code will break the Arduino Servo() library.
 */
    cli();                                          // Disable global
    TCCR1A = 0;                                     // Clear timer counter control registers.  The initial value is zero but it appears Arduino code modifies them on startup...
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12);                         // Timer #1 using CTC mode  
    TIMSK1 |= (1 << OCIE1A);                        // Enable CTC interrupt
    TCCR1B |= (1 << CS10)|(1 << CS11);              // Prescale: divide by f_clk by 64.  Note SC12 cleared earlier
    OCR1A = (f_clk / 64L) / desired_ISR_freq;       // Interrupt when TCNT1 equals the top value of the counter specified by OCR
    sei();                                          // Enable global
}
