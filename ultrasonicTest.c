/*
 * File:   ultrasonicTest.c
 * Author: Rusty
 *
 * Created on May 10, 2017, 12:13 PM
 */

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 


#include "xc.h"
#include "include/delay.h"
#include "include/lcd_4bit.h"
#include "include/local_rtcc.h"
void getHigh(void);
void getLow(void);
void theresCar(short state);

int parked = 0;//flag if a car is parked
int count; 
rtcc_t test2;
void initTime()
{
     rtcc_t test;
     rtccInit();
    // February 18, 2017 1:13 Saturday
    test.rtcc_hour = 1;
    test.rtcc_mday = 18;
    test.rtcc_min = 13;
    test.rtcc_mon = 2;
    test.rtcc_sec = 35;
    test.rtcc_wday = 6;
    test.rtcc_year = 17;
    setTime(test);
}
int main(void) {
    TRISA = 0xFFFF;
    TRISB = 0;
    AD1PCFG = 0xFDFF;
    lcdInit();
    lcdPrint("helloworld");
    setCursor(0xC0);
    PORTA = 0xFFFF;
    initTime();
    delay(1000);
    /*while(1){
        clearLine2(); 
        getHigh();
        count = 0;
        getLow();
        lcdIntPrint(count);
    }*/
    if (count<2500)//car is parked
    {
        LATBbits.LATB15 = 0; //GREEN LED IS ON
        LATBbits.LATB11 = 1; //RED LED IS OFF
        
        if(press == 0)
        {
            test2 = getTime();
            //write this to i2c
            press = 1;//set flag
        }
    }
    else//space is free
    {
        LATBbits.LATB15 = 1; //GREEN LED IS OFF
        LATBbits.LATB11 = 0; //RED LED IS ON
        
        if(press == 1)
        {
            test2 = getTime(); //get end time
            //write this to i2c
            press = 0; //clear flag
        }
    }
            
    return 0;
}

void getHigh(){
    while(PORTAbits.RA4 == 0){
        
    }
}

void getLow(){
    while(PORTAbits.RA4 == 1){
        count++;
    }
}