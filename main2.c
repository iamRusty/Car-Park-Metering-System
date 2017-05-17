#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 
#pragma config RTCOSC = LPRC

#define _INTEGER 0
#define _DECIMAL 1

#include "xc.h"
#include <stdio.h>
#include "include/delay.h"
#include "include/lcd_4bit.h"
#include "include/keypad.h"
#include "include/local_rtcc.h"
#include "include/i2c.h"
#include "include/rtcc_alarm.h"

// Main Functions
void meteringMode(void);
void dataDispMode(void);
void printDataPoint(int addr, int half);
void countData(void);
void findOldNew(void);
void getHigh(void);
void getLow(void);
void askRate(void);
void scanFloat(float *addr);
long parseNumber(char *string, int d_type);
long convTime(rtcc_t input);
rtcc_t deconvTime(long input);
void getStartTime(void);
rtcc_t parseStartTime(void);
void displayEETime(long input);
void displayRTCCTime(rtcc_t input);
int minusTime(rtcc_t input1, rtcc_t input2);
long floatToInt(float f);
float intToFloat(long i);
void getPulseWidth(void);
void getHigh(void);
void getLow(void);

// EEPROM Functions
void eeWrite(long data, int address);
void eeRead(long *data, int address);
void eeClean(int count);

// Main Data types
typedef union fi_t{
    float f;
    long i;
} fi_t;


// Main Variables
int counter, newest_addr, oldest_addr;
char start_time[17] = "__:__ __/__/20__";
int data_count = 0; // must be 50
int pulse_width = 0;
int car_minute = 0;
int rtcc_flag=0;

void __attribute__((interrupt,auto_psv)) _RTCCInterrupt(void)
{
    rtcc_flag=1; //set flag
    IFS3bits.RTCIF = 0; //clear flag    
}


int main(void) {

    TRISA = 0xFFFF;
    TRISB = 0x1340;
    AD1PCFG = 0xFDFF;
    lcdInit();
    i2c_init();
    keypadInit();
    rtccInit();
    alarmInit();
    disableAlarm();
    
    // Main Loop
#if 0
    getStartTime();
    setTime(parseStartTime());
    askRate();
    clearLine2();
    clearLine1();
    countData();
    findOldNew();
    lcdIntPrint(newest_addr); delay(2000);
    while(1){
        meteringMode();
        countData();
        findOldNew();
        dataDispMode();
    }
#endif    

#if 1
    countData();
    lcdIntPrint(data_count);
    delay(2000);
    findOldNew();
    lcdIntPrint(newest_addr);
    delay(2000);
    while(1){
        findOldNew();
        meteringMode();
        findOldNew();
        dataDispMode();
    }    
#endif    
    return 0;
} 

void meteringMode(void){
    lcdPrint("Ruler");
    
    // Get Rate from EEPROM
    long temp, iwashere;
    eeRead(&temp, 0x1FFA);
    float rate = intToFloat(temp);
    lcdFloatPrint(rate);
    delay(1000);

    int key_val, alisnaboi = 0;
    while(!alisnaboi){
        
        // Blocking for car presence
        getPulseWidth();

        if (getIsPressed()){
            key_val = getKeyValue();
            if (key_val == 11){
                alisnaboi = 1;
                setIsPressed(0);
                return;
            }   
        }        
#if 1   
        if (pulse_width > 2000)
            continue;
        else {

            if (newest_addr == 49)
                newest_addr = 0;
            else
                newest_addr++;            

            car_minute = 0;
            enableAlarm();
            clearLine1();
            lcdPrint("Fee: "); lcdFloatPrint(rate * (car_minute + 1));
            clearLine2(); lcdPrint("Car Parked");
            iwashere = convTime(getTime());
            
            while(1) {
                if (getIsPressed()){
                    key_val = getKeyValue();
                    if (key_val == 11){
                        alisnaboi = 1;
                        setIsPressed(0);
                        break;
                    }   
                }
                    
                if (rtcc_flag){
                    car_minute++;
                    rtcc_flag = 0;
                    clearLine1();
                    lcdPrint("Fee: "); lcdFloatPrint(rate * (car_minute));
                }
                getPulseWidth();
                if (pulse_width < 2000)
                    continue;
                else
                    break;
            }
        }
        clearLine2(); lcdPrint("No Park");
        disableAlarm();
        rtcc_flag = 0;
        clearLine1();
        lcdIntPrint(newest_addr);
        eeWrite(floatToInt(rate * (car_minute+1)) , newest_addr * 12 + 0);
        eeWrite(iwashere, newest_addr * 12 + 4);
        delay(1000);
        eeWrite(convTime(getTime()), newest_addr * 12 + 8);
        delay(1000);
#endif
    }
}

void dataDispMode(void){
    int cur_addr = 0;
    int key_val, half = 0;
    
    printDataPoint(0, 0);
    while(1){
        resetPullup();
        if (!getIsPressed()){
            continue;
        }
        else {
            key_val = getKeyValue();
            if (key_val == 2){
                if (cur_addr == 49){
                    cur_addr = 0;
                }
                else if ((cur_addr >= newest_addr)&&(data_count < 50))
                    cur_addr = 0;
                else
                    cur_addr++;
                half = 0;
            }
            else if (key_val == 8){
                if (cur_addr == 0){
                    if (data_count >= 50)
                        cur_addr = 49;
                    else
                        cur_addr = newest_addr;
                }
                else
                    cur_addr--;
                half = 0;
            }
            else if (key_val == 1){
                cur_addr = oldest_addr;
                half = 0;
            }
            else if (key_val == 9){
                cur_addr = newest_addr;
                half = 0;
            }
            else if (key_val == 4){
                if (!half)
                    continue;
                half = 0;
            }
            else if (key_val == 6){
                if (half)
                    continue;
                half = 1; 
            }
            else if (key_val == 10){
                setIsPressed(0);
                return;
            }
            printDataPoint(cur_addr, half);
            setIsPressed(0);
        }
    }
}

void printDataPoint(int address, int half){
    long l1, l2;
    eeRead(&l1, address * 12);
    if (half)
        eeRead(&l2, address * 12 + 8);
    else 
        eeRead(&l2, address * 12 + 4);
    
    clearLine1();
    lcdIntPrint(address); lcdPrint(" "); lcdFloatPrint(intToFloat(l1));
    setCursor(0xC0);
    displayEETime(l2);
}

void countData(void){
    /*
     * If the whole datapoint is 0 then noty
     */
    data_count = 0;
    long l1,l2,l3;
    while (data_count < 50){
        eeRead(&l1, data_count * 12);
        eeRead(&l2, data_count * 12 + 4);
        eeRead(&l3, data_count * 12 + 8);
        
        if ((l1 != 0) || (l2 != 0) || (l3 != 0))
            data_count++;
        else
            break;
    }
}

void findOldNew(void){
    /*
     *  Check only the start_time of data points
     */
    
    int count = 0;
    long current, previous = 0;
    while(count < data_count){
        eeRead(&current, count * 12 + 4);
        if (current >= previous){
            previous = current;
            count++;
        }
        else
            break;
    }
    
    if (count == 0) {
        newest_addr = 0;
        oldest_addr = 0;
    }
    else {
        newest_addr = count - 1;
        if (data_count >= 50){
            oldest_addr = newest_addr + 1;
            //eeClean(count * 3); // Because it's a fallacy
        }
        else 
            oldest_addr = 0;
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

void getStartTime(void){ // A blocking function
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

void askRate(void){ // A blocking function
    
    // Check if there's rate in 0x1FFA
    long i_price = 0;
    eeRead(&i_price, 0x1FFA);
    fi_t fi_price;
    fi_price.i = i_price;
    float f;
    int key;
    if ((i_price == 0)||(i_price == 0xFFFFFFFF)){
        scanFloat(&f);
        eeWrite(floatToInt(f), 0x1FFA);
    }
    else {
        clearLine1();
        lcdPrint("Rate: "); lcdFloatPrint(fi_price.f);
        clearLine2();
        lcdPrint("Change? *-N  #-Y");
        while(1){
            resetPullup();
            if (!getIsPressed()){
                continue;
            }
            else {
                key = getKeyValue();
                if (key == 11){
                    scanFloat(&f);
                    eeWrite(floatToInt(f), 0x1FFA);
                    break;
                }
                else if (key == 10){
                    break;
                }
            }
        }
    }
}

void scanFloat(float *addr){ // A blocking function
    clearLine1();
    lcdPrint("Enter Rate:");
    char f_string1[17] = "Integer:        ";
    char f_string2[17] = "Decimal:        ";
    short key, cpos, done = 0;
    while(!done){
        
        // Integer Part
        clearLine2();    
        lcdPrint(f_string1);
        setCursor(0xC9);
        while(1){
            resetPullup();
            if (!getIsPressed())
                continue;
            else {
                key = getKeyValue();
                cpos = getCursorPos();
                if (key == 10){
                    if (cpos != 0xC9){
                        setCursor(cpos - 1);
                    }
                }
                else if (key == 11){
                    if (cpos >= (0xC0 + 15)){
                        setIsPressed(0);
                        break;
                    }
                    else {
                        setCursor(cpos + 1);
                    }
                }
                else {
                    if (cpos == 0xC9){
                        Nop();
                    }
                    else if (f_string1[cpos - 0xC0 - 1] == ' '){
                        continue;
                    }
                    lcdIntPrint(key);
                    f_string1[cpos - 0xC0] = key + '0';   
                }
                setIsPressed(0);
            }
        }
        
        // Fractional part
        clearLine2();
        lcdPrint(f_string2);
        setCursor(0xC9);
        while(1){
            resetPullup();
            if (!getIsPressed())
                continue;
            else {
                key = getKeyValue();
                cpos = getCursorPos();
                if (key == 10){
                    if (cpos != 0xC9){
                        setCursor(cpos - 1);
                    }
                }
                else if (key == 11){
                    if (cpos >= (0xC0 + 15)){
                        setIsPressed(0);
                        done = 1;
                        break;
                    }
                    else {
                        setCursor(cpos + 1);
                    }
                }
                else {
                    if (cpos == 0xC9){
                        Nop();
                    }
                    else if (f_string2[cpos - 1 - 0xC0] == ' '){
                        continue;
                    }
                    lcdIntPrint(key);
                    f_string2[cpos - 0xC0] = key + '0';
                }
                setIsPressed(0);
            }
        }   
    }
    clearLine1();
    lcdPrint(f_string1);
    clearLine2();
    lcdPrint(f_string2);
    //while(1){}
    long integer, decimal;
    integer = parseNumber(f_string1, _INTEGER);
    decimal = parseNumber(f_string2, _DECIMAL);
    
    float temp = decimal;
    temp = temp / 100;
    *addr = integer;
    *addr = *addr + temp;
}

long parseNumber(char *string, int d_type){
    int count, count2, count3, start, end;
    setCursor(0xC0 + 13);
    count = 0;
    
    // Get Start locus
    while (count < 16){
        if ((*(string + count) < '0') || 
            (*(string + count) > '9'))
            count++;
        else
            break;
    }
    start = count;
    
    // Get End locus
    while (count < 16){
        if ((*(string + count) >= '0') &&
            (*(string + count) <= '9'))
            count++;
        else
            break;
    }
    end = count;
    
    // If DECIMAL, send only 2 of the MSB because centavos ???
    if (d_type == _DECIMAL){
        if (end - start == 1)
            return (*(string + 9) - '0') * 10;
        return (*(string + 9) - '0') * 10 + *(string + 10) - '0';
    }
    
    // String to Integer
    long temp, number = 0;
    count = end - start;
    count3 = 0;
    clearLine1();
    while (count > 0){
        count2 = 1; 
        temp = 1;
        while (count2 < count){
            temp = temp * 10;
            count2++;
        }
        number = number + (*(string + count3 + 9) - '0') * temp;
        count--;
        count3++;
    }
    return number;
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

void eeClean(int count){
    while(count < 153){
        eeWrite(0, count * 4);
        count++;
    }
}

long convTime(rtcc_t input_time){
    /* Convert rtcc_t time to a eeWrite-ready format
     * yr - 12 | mon - 4 | mday - 5 | hour - 5 | minute - 6
     */
    long data = 0;
    long temp;
    
    // Year
    data = input_time.rtcc_year;
    data = data << 20;
    
    // Month
    temp = input_time.rtcc_mon;
    data = data + (temp << 16);
    
    // Day
    temp = input_time.rtcc_mday;
    data = data + (temp << 11);

    // Hour
    temp = input_time.rtcc_hour;
    data = data + (temp << 6);

    // Minute
    data = data + input_time.rtcc_min;  
    
    return data;
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

void getPulseWidth(){
    getHigh();
    pulse_width = 0;
    getLow();
}

void getHigh(){
    while(PORTAbits.RA4 == 0){
        
    }
}

void getLow(){
    while(PORTAbits.RA4 == 1){
        pulse_width++;
    }
}