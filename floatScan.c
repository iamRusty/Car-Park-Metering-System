/*
 * File:   floatScan.c
 * Author: Rusty
 *
 * Created on May 13, 2017, 1:55 PM
 */


#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 

#include "xc.h"
#include "include/delay.h"
#include "include/lcd_4bit.h"
#include "include/keypad.h"

typedef union fi_t{
    float f;
    long i;
} fi_t;

int main(void) {
    TRISA = 0xFFFF;
    TRISB = 0x9FE0;
    AD1PCFG = 0xFDFF;
    lcdInit();
    keypadInit();
    
    lcdPrint("Rate:     ___.__");
    setCursor(0x80 + 10);
    while(1){}
    return 1;
}
