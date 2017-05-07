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

void rtcc_unlock()
{
    asm volatile("push w7");
    asm volatile("push w8");
    asm volatile("disi #5");
    asm volatile("mov #0x55, w7");
    asm volatile("mov w7, _NVMKEY");
    asm volatile("mov #0xAA, w8");
    asm volatile("mov w8, _NVMKEY");
    asm volatile("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile("pop w8");
    asm volatile("pop w7");
    asm volatile("nop");
    asm volatile("nop");
}

int rtcc_init()
{
    OSCCONbits.COSC = 0b101;
    
    RCFGCAL = 0;
    rtcc_unlock();
    
    RCFGCALbits.RTCEN = 1; //start RTCC
    RCFGCALbits.RTCOE = 1;
    RCFGCALbits.RTCPTR = 3;
    
    
    RTCVAL = 0x0017; //year
    RTCVAL = 0x0507; //month and day
    RTCVAL = 0x0610; //wkday and hr
    RTCVAL = 0x0510; //mins secs
    
    
    RCFGCALbits.RTCWREN = 0;
    
    return 0;
}
int get_time()
{
    int year, year_10, year_1;
    int month, month_r;
    int day, day_10, day_1;
    
    RCFGCALbits.RTCPTR = 3;
    year = RTCVAL;
    month = RTCVAL;
    //day = RTCVAL & 0x00FF;
    year_10 = ((year & 0x00F0) >> 4)*10;
    year_1 = (year & 0xF);
    year = 2000 + year_10 + year_1;
    
    month_r = (month &0xFF00) >> 8;
    day_1 = month & 0x00FF;
    day_10 = ((month & 0x00F0)>>4) * 10;
    day = day_10 + day_1;
    lcdPrint("Year: "); lcdIntPrint(year); setCursor(0xC0);
    lcdPrint("Mo: ");
    lcdIntPrint(month_r);
    lcdPrint(" Day: ");
//    //setCursor(0xc0);
    lcdIntPrint(day);
    
    
    while(1)
    {
        
    }
    
    return 0;
}

int main(void) {
    TRISB = 0;
    AD1PCFG=0xFDFF;
    
    //LATB = 0xffff;
    //LATB = 0xffff;
    lcdInit();
    //lcdPrint("Hello");
    delay(1500);
    
    rtcc_init();
    
    get_time();
    
   
    
}
