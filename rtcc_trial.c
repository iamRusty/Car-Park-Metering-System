/*
 * File:   trial.c
 * Author: John
 *
 * Created on May 6, 2017, 11:24 PM
 */

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 
#pragma config RTCOSC = LPRC //trial
#include "xc.h"
#include "include/lcd_4bit.h"

int rtcc_init()
{
    RCFGCAL = 0;
    RCFGCALbits.RTCWREN = 1;
    RCFGCAL = RCFGCAL | 0b0000011100000000;
    
    
    
    
    RTCVAL = 0x0017; //year
    RTCVAL = 0x0507; //month and day
    RTCVAL = 0x0610; //wkday and hr
    RTCVAL = 0x0510; //mins secs
    
    RCFGCALbits.RTCEN = 1; //start RTCC
    RCFGCALbits.RTCWREN = 0;
    
    return 0;
}
int get_time()
{
    int month, day;
    
    RCFGCALbits.RTCPTR = 2;
    month = RTCVAL >> 8;
    day = RTCVAL & 0x00FF;
    
    lcdIntPrint(month);
    lcdIntPrint(day);
    
    return 0;
}
int main(void) {
    lcdInit();
    AD1PCFG=0xFDFF;
    
    rtcc_init();
    
    get_time();
    
}
