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
    //RCFGCALbits.RTCOE = 1;
    RCFGCALbits.RTCPTR = 3;
    
    
    //RTCVAL = 0x0017; //year
    //RTCVAL = 0x0507; //month and day
    //RTCVAL = 0x0610; //wkday and hr
    //RTCVAL = 0x0500; //mins secs
    
    
    RCFGCALbits.RTCWREN = 0;
    
    return 0;
}
int get_time()
{
    int year;
    int month;
    int day;
    int hr;
    int min;
    int sec;
    int yr, mthdy, wkdhyr, minsec;
    
    RCFGCALbits.RTCPTR = 3;
    yr = RTCVAL;
    mthdy = RTCVAL;
    wkdhyr = RTCVAL;
    minsec = RTCVAL;
    
    sec     = bcdToDec(minsec & 0x00FF);
    min     = bcdToDec((minsec & 0xFF00) >> 8);
    year    = bcdToDec(yr & 0x00FF);
    month   = bcdToDec((mthdy & 0xFF00) >> 8);
    day     = bcdToDec(mthdy & 0x00FF);
    hr      = bcdToDec(wkdhyr & 0x00FF);
   
    lcdPrint("YYDDMM HH:mm:ss"); setCursor(0xC0);
    //lcdTimePrint(year); lcdTimePrint(day); lcdTimePrint(month);
    //lcdPrint(" "); lcdTimePrint(hr);
    lcdPrint(":"); lcdTimePrint(min);
    lcdPrint(":"); lcdTimePrint(sec);
    
    
    
    return 0;
}

int main(void) {
    TRISB = 0;
    AD1PCFG=0xFDFF;
    
    //LATB = 0xffff;
    //LATB = 0xffff;
    lcdInit();
    //lcdPrint("Hello");
    
    rtcc_init();
    
    get_time();
    delay(5000);
    setCursor(0x80);
    get_time();
    while(1){}
   
}
