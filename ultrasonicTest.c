/*
 * File:   ultrasonicTest.c
 * Author: Rusty
 *
 * Created on May 10, 2017, 12:13 PM
 */

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 


#include "xc.h"
#include "include/delay.h"
#include "include/lcd_4bit.h"

void getHigh(void);
void getLow(void);
void theresCar(short state);

int count; 

int main(void) {
    TRISA = 0xFFFF;
    TRISB = 0;
    AD1PCFG = 0xFDFF;
    lcdInit();
    lcdPrint("helloworld");
    setCursor(0xC0);
    PORTA = 0xFFFF;

    delay(1000);
    while(1){
        clearLine2(); 
        getHigh();
        count = 0;
        getLow();
        lcdIntPrint(count);
    }
            
    return 0;
}

void getHigh(){
    while(PORTAbits.RA4 == 0){
        
    }
}

void getLow(){
    while(PORTAbits.RA4 == 1){
        count++;
    }
}