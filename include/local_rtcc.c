#include "xc.h"
#include "local_rtcc.h"
#include "lcd_4bit.h"

static void rtccUnlock(void){
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
    return;
}
void rtccInit(void){
    OSCCONbits.COSC = 0b101;
    RCFGCAL = 0;
    rtccUnlock();
    RCFGCALbits.RTCEN = 1; //start RTCC
    return;
}
rtcc_t getTime(void){
    int minsec, wkdhr, mthdy, yr;
    rtcc_t cur_time;
    
    RCFGCALbits.RTCPTR = 3;
    yr = RTCVAL;
    mthdy = RTCVAL;
    wkdhr = RTCVAL;
    minsec = RTCVAL;
   
    cur_time.rtcc_sec   = minsec & 0x00FF;
    cur_time.rtcc_min   = (minsec & 0xFF00) >> 8;
    cur_time.rtcc_year  = yr & 0x00FF;
    cur_time.rtcc_mon   = (mthdy & 0xFF00) >> 8;
    cur_time.rtcc_mday  = mthdy & 0x00FF;
    cur_time.rtcc_hour  = wkdhr & 0x00FF;
    
    return cur_time;
}

void setTime(rtcc_t input_time){
    int minsec, wkdhr, mthdy, yr;
    
    // Year
    yr = decToBCD(input_time.rtcc_year);
    
    // Month and Calendar Day
    mthdy = decToBCD(input_time.rtcc_mon);
    mthdy = mthdy << 8;
    mthdy = mthdy + decToBCD(input_time.rtcc_mday);
    
    // Weekday and Hour
    wkdhr = decToBCD(input_time.rtcc_wday);
    wkdhr = wkdhr << 8;
    wkdhr = wkdhr + decToBCD(input_time.rtcc_hour);
    
    // Minute and Seconds
    minsec = decToBCD(input_time.rtcc_min);
    minsec = minsec << 8;
    minsec = minsec + decToBCD(input_time.rtcc_sec);
    
    RCFGCALbits.RTCWREN = 1;    
    RCFGCALbits.RTCPTR = 3;
    RTCVAL = yr;
    RTCVAL = mthdy;
    RTCVAL = wkdhr;
    RTCVAL = minsec;
    RCFGCALbits.RTCWREN = 0;    

    return;
}

int decToBCD(int value){
    return ((value/10)<<4) + (value%10);
}

int bcdToDec(int value){
    return (value >> 4) * 10 + (value & 0x0F);
}