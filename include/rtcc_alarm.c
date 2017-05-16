/*
 * File:   rtcc_alarm.c
 * Author: John
 *
 * Created on May 16, 2017, 10:43 PM
 */


#include "xc.h"
#include  "rtcc_alarm.h"
#include "local_rtcc.h"





void alarmInit()
{
    ALCFGRPTbits.AMASK = 3; //alarm every minute
    
    //two lines below allow for indefinite alarm
    ALCFGRPTbits.CHIME = 1;
    ALCFGRPTbits.ARPT = 0xFF; 
    
    ALCFGRPTbits.ALRMPTR = 2; //set pointer
    
    //lines below set alarm registers be equal to time registers
//    alrm_rtcc_t.alrm_rtcc_mon = rtcc_t.rtcc_mon;
//    alrm_rtcc_t.alrm_rtcc_mday = rtcc_t.rtcc_mday;
//    alrm_rtcc_t.alrm_rtcc_wday = rtcc_t.rtcc_wday;
//    alrm_rtcc_t.alrm_rtcc_hour = rtcc_t.rtcc_hour;
//    alrm_rtcc_t.alrm_rtcc_min = rtcc_t.rtcc_min;
//    alrm_rtcc_t.alrm_rtcc_min = rtcc_t.rtcc_min;
//    alrm_rtcc_t.alrm_rtcc_sec = rtcc_t.rtcc_sec;
    ///////////////////////////////////////////////
    
    
    IEC3bits.RTCIE = 1; //enable rtcc interrupt
    IFS3bits.RTCIF = 0; //clear rtcc interrupt flag
    
    RCFGCALbits.RTCEN = 1; //enable rtcc
    ALCFGRPTbits.ALRMEN = 1; //enable alrm
         
    
}
//void __attribute__((interrupt,auto_psv)) _RTCCInterrupt(void)
//{
//    rtcc_flag=1; //set flag
//    IFS3bits.RTCIF = 0; //clear flag
//    
//}

void disableAlarm(){
    ALCFGRPTbits.ALRMEN = 0; //disable alarm
}

void enableAlarm(){
    ALCFGRPTbits.ALRMEN = 1; //enable alarm
}
