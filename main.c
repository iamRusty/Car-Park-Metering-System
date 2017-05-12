/*
 * File:   main.c
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
#include "include/keypad.h"
#include <stdio.h>
#include "include/i2c.h"

void getHigh(void);
void getLow(void);
void theresCar(short state);
void eeTime(long data, int address);
void intToFloat(long i);
void floatToInt(float f);

union fi_t{
    float f;
    long i;
};

int count; 
int main(void) {
    TRISA = 0xFFFF;
    TRISB = 0;
    AD1PCFG = 0xFDFF;
    lcdInit();
    keypadInit();
    lcdPrint("helloworld");
    setCursor(0xC0);
    PORTA = 0xFFFF;

#if 0
    while(1){
        LATBbits.LATB11 = 1;
        LATBbits.LATB15 = 0;
        delay(2000);
        LATBbits.LATB11 = 0;
        LATBbits.LATB15 = 1;
        delay(2000);        
    }
#endif 
    
    delay(1000);
    while(1){
        clearLine2(); 
        getHigh();
        count = 0;
        getLow();
        lcdIntPrint(count);
        delay(1000);
        
        if (count < 2500)
            theresCar(1);
        else
            theresCar(0);
        
        delay(2000);
        
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

void getHigh(){
    while(PORTAbits.RA4 == 0){
        
    }
}

void getLow(){
    while(PORTAbits.RA4 == 1){
        count++;
    }
}

void theresCar(short state){
    if (!state){
        LATBbits.LATB11 = 1;
        LATBbits.LATB15 = 0;
    }
    else {
        LATBbits.LATB11 = 0;
        LATBbits.LATB15 = 1;
    }
}

void eeData(long data, int address){
    char *ad_ptr = &address;           // Address to be written at
    i2c_start();                       // Start I2C
    i2c_tx(0b10100000);                // Send EEPROM CTRL byte with Write enabled
    i2c_tx(*(ad_ptr+1));               // Send EEPROM High Address
    i2c_tx(*ad_ptr);                   // Send EEPROM Low Address
    
    char *d_ptr = &data;
    int count2 = 0;
    while (count2 < 4){
        i2c_tx(*(d_ptr+count2));
        count2++;
    }
    
    i2c_stop();
    delay(10);
}

float intToFloat(long i){
    fi_t fi;
    fi.i = i;
    return fi.f;
}

long floatToInt(float f){
    fi_t fi;
    fi.f = f;
    return fi.i;
}