/*
 * File:   lcdTest.c
 * Author: Rusty
 *
 * Created on May 5, 2017, 11:13 PM
 */


#include "xc.h"
#include "include/lcd_4bit.h"

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 

int main(void) {
    TRISB = 0;
    TRISA = 0;
    AD1PCFG = 0xFDFF;
    
    lcdInit();
    lcdPrint("helloworld");
    setCursor(0xC0);
    lcdIntPrint(10);
    lcdPrint(" ");
    lcdFloatPrint(3.25);
    
    while(1){}
    return 0;
}
