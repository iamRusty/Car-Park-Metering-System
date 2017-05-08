/*  
 * File:  rtcc_trial.c
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
#include "include/rtcc.h"

int main(void) {
    TRISB = 0;
    AD1PCFG=0xFDFF;
    
    lcdInit();
    rtccInit();
    
    rtcc_t test, test2;
    // February 18, 2017 1:13 Saturday
    test.rtcc_hour = 1;
    test.rtcc_mday = 18;
    test.rtcc_min = 13;
    test.rtcc_mon = 2;
    test.rtcc_sec = 35;
    test.rtcc_wday = 6;
    test.rtcc_year = 17;
    
    setTime(test);
    test2 = getTime();
    lcdPrint("MM:DD:YYYY HH:mm");
    setCursor(0xC0);
    lcdTimePrint(bcdToDec(test2.rtcc_mon)); lcdPrint("/");
    lcdTimePrint(bcdToDec(test2.rtcc_mday)); lcdPrint("/");
    lcdTimePrint(bcdToDec(test2.rtcc_year) + 2000); lcdPrint(" ");
    lcdTimePrint(bcdToDec(test2.rtcc_hour)); lcdPrint(":");
    lcdTimePrint(bcdToDec(test2.rtcc_min)); 
    while(1);
    return 0;
}
