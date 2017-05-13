#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 
#pragma config RTCOSC = LPRC

#define TRUE 1
#define FALSE 0

#include "xc.h"
#include <stdio.h>
#include "include/delay.h"
#include "include/lcd_4bit.h"
#include "include/keypad.h"
#include "include/local_rtcc.h"
#include "include/i2c.h"

// Main Functions
void getHigh(void);
void getLow(void);
float askRate(void);
void convTime(long *data, rtcc_t input);
rtcc_t deconvTime(long input);
void getStartTime(void);
rtcc_t parseStartTime(void);
void displayEETime(long input);
void displayRTCCTime(rtcc_t input);
long floatToInt(float f);
float intToFloat(long i);


// EEPROM Functions
void eeWrite(long data, int address);
void eeRead(long *data, int address);
void eeClean(void);

// Main Data types
typedef union fi_t{
    float f;
    long i;
} fi_t;


// Main Variables
int counter;
char start_time[17] = "__:__ __/__/20__";

int main(void) {

    TRISA = 0xFFFF;
    TRISB = 0x9B40;
    AD1PCFG = 0xFDFF;
    lcdInit();
    i2c_init();
    keypadInit();
    rtccInit();

#if 1
    getStartTime();
    setTime(parseStartTime());
    rtcc_t trial_time;
    while(1){
        trial_time = getTime();
        setCursor(0x80);
        displayRTCCTime(trial_time);
        setCursor(0xC0);
        lcdTimePrint(trial_time.rtcc_sec);
        delay(70000);
    }
    trial_time = getTime();
    setCursor(0x80);
    lcdTimePrint(trial_time.rtcc_sec);
    //displayRTCCTime(trial_time);    
    while(1){}
#endif    
    
    
    
#if 0    
    float asd;
    eeWrite(0, 0x1FFA);
    asd = askRate();
    while(1){}
#endif     
    long hello2 = 67;
    eeWrite(70, 0);
    eeWrite(69, 1);
    eeWrite(70, 2);
    eeWrite(70, 3);
    eeRead(&hello2, 0);
    lcdPrint(&hello2);
    while(1){}
    float f = 3.5;
    fi_t temp, temp2;
    temp.f = f;
    eeWrite(temp.i, 0x1000);
    eeRead(&hello2, 0x1000);
    temp2.i = hello2;
    lcdFloatPrint(temp2.f);
    
    lcdPrint(&hello2);
    while(1){}
    return 0;
} 

void getHigh(){
    while(PORTAbits.RA4 == 0){
        
    }
}

void getLow(){
    while(PORTAbits.RA4 == 1){
        counter++;
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

void getStartTime(void){
    int key, cpos = 0x80, done = 0, asterisk = 0;
    
    while(!done){
        
        setCursor(0x80);
        lcdPrint(start_time);
        setCursor(0xC0);
        lcdPrint("HH:mm MM/DD/YYYY");        
        
        setCursor(0x80);
        while(1){
            resetPullup();
            if (!getIsPressed())
                continue;
            else {
                // Cursor Navigation
                key = getKeyValue();
                cpos = getCursorPos();
                if (key == 10){
                    if ((cpos == 0x83)||(cpos == 0x86)||(cpos == 0x89))
                        setCursor(cpos - 2);
                    else if (cpos == 0x8E)
                        setCursor(0x8A);                    
                    else if (cpos != 0x80)
                        setCursor(cpos - 1);
                }
                else if (key == 11){
                    if (cpos >= (0x80 + 15)){
                        setIsPressed(0);
                        break;
                    }
                    else if ((cpos == 0x81)||(cpos == 0x84)||(cpos == 0x87))
                        setCursor(cpos + 2);
                    else if (cpos == 0x8A)
                        setCursor(0x8E);
                    else
                        setCursor(cpos + 1);
                }
                else {
                    lcdIntPrint(key);
                    start_time[cpos - 0x80] = key + '0';
                    if ((cpos == 0x81)||(cpos == 0x84)||(cpos == 0x87))
                        setCursor(cpos + 2);
                    else if (cpos == 0x8A)
                        setCursor(0x8E);
                }
                setIsPressed(0);
            }
        }

        clearLine1();
        lcdPrint("Are you sure?");
        clearLine2();
        lcdPrint("# = Yes   * = No");     
        while(1){
            resetPullup();
            if (!getIsPressed())
                continue;
            else {
                key = getKeyValue();
                if (key == 10){
                    setIsPressed(0);
                    asterisk = 1;
                    break;
                }
                else if (key == 11){
                    setIsPressed(0);
                    done = 1;
                    break;
                }
            }
        }
    }
}

rtcc_t parseStartTime(void){
    rtcc_t input_time;
    input_time.rtcc_min = (start_time[3] - '0') * 10 + start_time[4] - '0';
    input_time.rtcc_hour = (start_time[0] - '0') * 10 + start_time[1] - '0';
    input_time.rtcc_mday = (start_time[9] - '0') * 10 + start_time[10] - '0';
    input_time.rtcc_mon = (start_time[6] - '0') * 10 + start_time[7] - '0';
    input_time.rtcc_year = (start_time[14] - '0') * 10 + start_time[15] - '0'; 
#if 1    
    input_time.rtcc_wday = 0;
    input_time.rtcc_sec = 0;
#endif
    return input_time;
}

void displayEETime(long input){
    displayRTCCTime(deconvTime(input));
}

void displayRTCCTime(rtcc_t input){
    lcdTimePrint(input.rtcc_mon); lcdPrint("/");
    lcdTimePrint(input.rtcc_mday); lcdPrint("/");
    lcdTimePrint(input.rtcc_year + 2000); lcdPrint(" ");
    lcdTimePrint(input.rtcc_hour); lcdPrint(":");
    lcdTimePrint(input.rtcc_min);
}
float askRate(void){
    long i_price = 0;
    eeRead(&i_price, 0x1FFA);
    fi_t fi_price;
    fi_price.i = i_price;      
    
    if ((i_price == 0)||(i_price == 0xFFFFFFFF)){
        setCursor(0x80);
        lcdPrint("Enter Rate:");
        setCursor(0xC0);
        while(1){}
        return 3.25;
    }
    else {
        return fi_price.f;
    }
}

void eeWrite(long data, int address){
    char *ad_ptr = &address;
    i2c_start();                    // send I2C start sequence
    i2c_tx(0b10100000);             // send EEPROM CTRL byte with Write enabled
    i2c_tx(*(ad_ptr + 1));          // Send EEPROM High Address
    i2c_tx(*ad_ptr);                // Send EEPROM Low Address
        
    char *d_ptr = &data;
    int count = 0;
    while (count < 4){
        i2c_tx(*(d_ptr+count));
        count++;
    }
    
    i2c_stop();
    delay(10);
}

void eeRead(long *data, int address){
    char* data_ptr = data;
    char *ad_ptr = &address;
    i2c_start();                    // send I2C start sequence
    i2c_tx(0b10100000);             // send EEPROM CTRL byte with Write enabled
    i2c_tx(*(ad_ptr + 1));          // Send EEPROM High Address
    i2c_tx(*ad_ptr);                // Send EEPROM Low Address
    i2c_start();                // send I2C restart sequence
    i2c_tx(0b10100001);         // send EEPROM CTRL byte with Read enabled
    
    int count = 0;
    while (count < 4){
        *(data_ptr + count) = i2c_rx();  
        if (count < 3)
            send_ack();
        
        count++;
    }
    delay(10);   // THIS DELAY IS IMPORTANT
    i2c_stop();
}

void eeClean(void){
    i2c_start();            // send I2C start sequence
    i2c_tx(0b10100000);     // send EEPROM CTRL byte with Write enabled
    i2c_tx(0);              // Send EEPROM High Address
    i2c_tx(0);              // Send EEPROM Low Address

    int count = 0;
    while (count < 4){
        i2c_tx(0);
        count++;
    }
    
    i2c_stop();
    delay(10);
}

void convTime(long *data, rtcc_t input_time){
    /* Convert rtcc_t time to a eeWrite-ready format
     * yr - 12 | mon - 4 | mday - 5 | hour - 5 | minute - 6
     */
    *data = 0;
    long temp;
    
    // Year
    *data = input_time.rtcc_year;
    *data = *data << 20;
    
    // Month
    temp = input_time.rtcc_mon;
    *data = *data + (temp << 16);
    
    // Day
    temp = input_time.rtcc_mday;
    *data = *data + (temp << 11);

    // Hour
    temp = input_time.rtcc_hour;
    *data = *data + (temp << 6);

    // Minute
    *data = *data + input_time.rtcc_min;    
}

rtcc_t deconvTime(long input){
    /* Convert eeprom format time to a rtcc_t format
     * yr - 12 | mon - 4 | mday - 5 | hour - 5 | minute - 6
     */
    rtcc_t temp;
    temp.rtcc_min  =   input & 0x3F;
    temp.rtcc_hour =   (input & 0x7C0) >> 6;
    temp.rtcc_mday =   (input & 0xF800) >> 11;
    temp.rtcc_mon  =   (input & 0xF0000) >> 16;
    temp.rtcc_year =   input >> 20; 
    temp.rtcc_wday = 0;
    temp.rtcc_sec = 0;
    return temp;
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