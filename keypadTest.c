/*
 * File:   keypadTest.c
 * Author: Rusty
 *
 * Created on May 7, 2017, 12:00 AM
 */
#include "xc.h"
#include "include/keypad.h"
#include "include/lcd_4bit.h"

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 
        
#define DEB_MAX 10

int main(void) {
    
    AD1PCFG = 0xffff;
    TRISA = 0xFFFF;  //All input
    TRISB = 0x0000;  //All output
    LATB = 0xffff;
    
    lcdInit();
    CNPU1 = CNPU1 | 0x000C; // Bit 2 and 3
    CNPU2 = CNPU2 | 0x6000; // Bit 13 and 14
    
    CNEN1 = CNEN1 | 0x000C;
    CNEN2 = CNEN2 | 0x6000; 
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    lcdPrint("Hello");
    
    while(1){
        // Reset Pulldown
        keyOff();
        resetPullup();
        keyOn(); 
        
        if(getIsPressed()) {
            setCursor(0xc0);
        lcdPrint("he");
        
            lcdIntPrint(getKeyValue());
            setIsPressed(0);
        }
        else
            continue;
    }
    return 0;
}