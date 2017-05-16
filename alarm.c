/*
 * File:   alarm.c
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
#include "include/rtcc_alarm.h"




int rtcc_flag=0;

//typedef  struct {
//    unsigned short alrm_rtcc_sec;
//    unsigned short alrm_rtcc_min;
//    unsigned short alrm_rtcc_hour;
//    unsigned short alrm_rtcc_wday;
//    unsigned short alrm_rtcc_mday;
//    unsigned short alrm_rtcc_mon;
//    unsigned short alrm_rtcc_year;
//} alrm_rtcc_t;

//void alrmInit()
//{
//    ALCFGRPTbits.AMASK = 3; //alarm every minute
//    
//    //two lines below allow for indefinite alarm
//    ALCFGRPTbits.CHIME = 1;
//    ALCFGRPTbits.ARPT = 0xFF; 
//    
//    ALCFGRPTbits.ALRMPTR = 2; //set pointer
//    
//    //lines below set alarm registers be equal to time registers
//    alrm_rtcc_t.alrm_rtcc_mon = rtcc_t.rtcc_mon;
//    alrm_rtcc_t.alrm_rtcc_mday = rtcc_t.rtcc_mday;
//    alrm_rtcc_t.alrm_rtcc_wday = rtcc_t.rtcc_wday;
//    alrm_rtcc_t.alrm_rtcc_hour = rtcc_t.rtcc_hour;
//    alrm_rtcc_t.alrm_rtcc_min = rtcc_t.rtcc_min;
//    alrm_rtcc_t.alrm_rtcc_min = rtcc_t.rtcc_min;
//    alrm_rtcc_t.alrm_rtcc_sec = rtcc_t.rtcc_sec;
//    ///////////////////////////////////////////////
//    
//    
//    IEC3bits.RTCIE = 1; //enable rtcc interrupt
//    IFS3bits.RTCIF = 0; //clear rtcc interrupt flag
//    
//    ALCFGRPTbits.ALRMEN = 1; //enable alrm
//         
//    
//}
void __attribute__((interrupt,auto_psv)) _RTCCInterrupt(void)
{
    lcdPrint("Interrupt!");
    rtcc_flag=1; //set flag
    IFS3bits.RTCIF = 0; //clear flag
    
}
int main(void) {
    int time = 0;
    
    TRISB = 0;
    AD1PCFG=0xFDFF;
    lcdInit();
    rtccInit();
    
    
    alrm_rtcc_t test, test2;
    // February 18, 2017 1:13 Saturday
//    test.rtcc_hour = 1;
//    test.rtcc_mday = 18;
//    test.rtcc_min = 13;
//    test.rtcc_mon = 2;
//    test.rtcc_sec = 35;
//    test.rtcc_wday = 6;
//    test.rtcc_year = 17;
    
    test.alrm_rtcc_mon = 1;
    test.alrm_rtcc_mday = 18;
    test.alrm_rtcc_wday = 13;
    test.alrm_rtcc_hour = 2;
    test.alrm_rtcc_min = 35;
    test.alrm_rtcc_min = 6;
    test.alrm_rtcc_sec = 17;
    
    alrmInit();
    
    while(1)
    {
        if(rtcc_flag)
        {
            time=time+1;
            
            setCursor(0xC0);
            lcdIntPrint(time);
            setCursor(0xA0);
            rtcc_flag=0;
        }
        
    }
    
}
