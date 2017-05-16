/*
 * File:   InputCapture.c
 * Author: John
 *
 * Created on May 14, 2017, 12:08 PM
 */


#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 
#pragma config RTCOSC = LPRC //trial

#include "xc.h"
#include "include/lcd_4bit.h"
#include "include/local_rtcc.h"

unsigned int Capture1, Capture2;

void __attribute__ ((interrupt, auto_psv)) _IC1Interrupt(void);

void setup()
{
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));
    RPINR7bits.IC1R = 15; //set RB15 as input for InCap
     __builtin_write_OSCCONL(OSCCON|(1<<6));
}
void incapInit()
{
    int clear;
    TRISB = 0x8000; //set RB15 as input pin
    AD1PCFG = 0xFDFF; //set all pins to digital except for pin an9(rb15)
    setup();
    IC1CON1 = 0;
    
    //ensure that IC1BUF is empty
    while(IC1CON1bits.ICBNE){
        clear = IC1BUF;
    }
    
    
    
    IC1CON2bits.SYNCSEL = 0b10100; //select InCap1 as trigger/sync
    
    IC1CON1bits.ICSIDL = 1;
    IC1CON1bits.ICTSEL = 4; //Timer 1 as timer sync
    IC1CON1bits.ICI = 1; //interrupt on every 2nd capture event
    
    IC1CON2bits.ICTRIG = 0;
    
    IC1CON1bits.ICM = 1; //detect on every edge (rising and falling)
    
    IFS0bits.IC1IF = 0; //clear flag
    IEC0bits.IC1IE = 1; //enable InCap interrupt
    
}

void timerInit()
{
    T1CON = 0;
    TMR1 = 0;
    PR1 = 0xFFFF;
    
    T1CONbits.TON = 1; //start timer
}

int main(void) {
    
    unsigned int width, width2;
    
    timerInit();
    incapInit();
    
    while(1)
    {
        width = Capture2 - Capture1;
        width2 = Capture1 - Capture2;
        
        lcdIntPrint(width);
        setCursor(0xC0);
        lcdIntPrint(width2);
        setCursor(0xA0);
    }
    
    
    
    
}

void __attribute__ ((interrupt, auto_psv)) _IC1Interrupt(void)
{
    
    Capture1 = IC1BUF; // Read and save off first capture entry
    Capture2 = IC1BUF; // Read and save off second capture entry
    IFS0bits.IC1IF = 0; // Reset respective interrupt flag
}
