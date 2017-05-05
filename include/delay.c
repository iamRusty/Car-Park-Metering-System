/* 
 * File: delay.c  
 * Author: Rusty Mina (rusty.mina@eee.upd.edu.ph)
 * Comments:
 * Revision history: 1
 * Note: If you want to change the clock frequency
 * change it in delay_cfg.h
 */

#include "delay.h"

void delay(int delay_constant);
void delay_ms(int ms);
void delay_us(int us);

// delay()
void delay(int delay_constant){
    int count, count2;   
    for (count2 = 0; count2 < delay_constant; count2++){
        for (count = 0; count < DELAY_ITERATION; count++){
        Nop();
        Nop();
        }
    }
    return;
}

// delay_ms()
void delay_ms(int ms){
    __delay32(CYCLES_PER_MS * ((unsigned long) ms));   //__delay32 is provided by the compiler, delay some # of milliseconds
    return;
}

// delay_us();
void delay_us(int us){
    __delay32(CYCLES_PER_US * ((unsigned long) us));    //delay some number of microseconds
    return;
}