/*
 *    Note: The Nop()s are important for timing
 */

/*
    D4 - RB1
    D5 - RB2
    D6 - RB3
    D7 - RB4
*/

#define DECIMAL_POINT_PRECISION 2       // Default 3 decimal points to print
#include "xc.h"
#include "lcd_4bit.h"


static int blink_value;
static int cursor_value;
static int cursor_pos;

// Only supports alpha-numeric
void lcdPrint(char* string){
    int __string_length__;
    __string_length__ = strlen(string);
    int count;
    count = 0;
    
    // The first 128 characters in supported LCD characters
    // are based on ASCII Table
    while(count < __string_length__){
        lcdWrite(string[count]);
        count++;
    }
    
    return;
}

void lcdIntPrint(long value){
    char __string_buffer__[16];
    sprintf(__string_buffer__, "%li", value);
    lcdPrint(__string_buffer__);
    return;
}

void lcdFloatPrint(float float_number){
    int abscissa = (int) float_number;
    float decimal_part;
    decimal_part = float_number - abscissa;
    int count = 0;
    while (count < 1 * DECIMAL_POINT_PRECISION){
        decimal_part = decimal_part * 10;
        count++;
    }
    lcdIntPrint(abscissa);
    lcdPrint(".");
    lcdIntPrint((int) decimal_part); 
    return;
}

void lcdTimePrint(long time){
    if (time < 10)
        lcdPrint("0");
    lcdIntPrint(time);
    return;
}

void lcdInit(void){
    // No data should be displayed to or from the display for 15ms
    delay(15);
    
    // Function Set command: (8-bit Interface)
    send4ToLCD(0x0003);
    delay(5);
    
    send4ToLCD(0x0003);
    //__delay_us(100);
    delay(15);
    
    // After this command is written, BF can be checked
    send4ToLCD(0x0003);
    delay(15);
    
    // Function Set: Sets interface to 4-bit
    send8ToLCD(0x0002);
    delay(15);
    
    // Function Set (Interface = 4-bit, 
    //    Set N for number of lines	(1)
    //    Set F for character font    (0)
    send8ToLCD(0x0028);
    delay(15);
    
    // Display Off
    send8ToLCD(0x0008);
    delay(15);
    
    // Clear Display 
    send8ToLCD(0x0001);
    delay(15);
    
    // Entry Mode Set
    send8ToLCD(0x0006);
    delay(15);
    
    // Display ON
    //  Set C for Cursor	(1)
    //  Set B for Blink     (1)
    send8ToLCD(0x000F);
    delay(15);
    
    // Initialize default values of blink and cursor and cursor_pos
    blink_value = 1;
    cursor_value = 1;
    cursor_pos = 0x80;
}

void send4ToLCD(int instruction_4){
    instruction_4 = instruction_4 << 1;
    LATB = instruction_4;
    Nop();
    LATB = LATB | 0x4000;
    Nop();
    LATB = LATB & 0x000F;
    Nop();
    return;
}

void send8ToLCD(int instruction_8){
    int ls_data = instruction_8 & 0x000F;
    ls_data = ls_data << 1;
    int ms_data = instruction_8 & 0x00F0;
    ms_data = ms_data >> 4;
    ms_data = ms_data << 1;
    
    LATB = ms_data;
    Nop();
    LATB = LATB | 0x4000;
    Nop();
    LATB = LATB & 0x000F;
    Nop();

    LATB = ls_data;
    Nop();
    LATB = LATB | 0x4000;
    Nop();
    LATB = LATB & 0x000F;
    Nop();
    //delay(1)
    return;
}

void lcdWrite(int instruction_8){
    int ls_data = instruction_8 & 0x000F;
    ls_data = ls_data << 1;
    ls_data = ls_data | 0x2000;
    int ms_data = instruction_8 & 0x00F0;
    ms_data = ms_data >> 4;
    ms_data = ms_data << 1;
    ms_data = ms_data | 0x2000;
    
    LATB = ms_data;
    Nop();
    LATB = LATB | 0x4000;
    Nop();
    LATB = LATB & 0x000F;
    Nop();

    LATB = ls_data;
    Nop();
    LATB = LATB | 0x4000;
    Nop();
    LATB = LATB & 0x000F;
    Nop();
    cursor_pos++;
    delay_us(100);
    return;
}

void setCursor(int DDRAM_address){
    send8ToLCD(DDRAM_address);
    cursor_pos = DDRAM_address;
    delay(1);
}

void clearDisplay(){
    clearLine1();
    clearLine2();
    setCursor(0x80);
}
void clearLine1(void){
    setCursor(0x80);
    lcdPrint("                ");
    setCursor(0x80);
}

void clearLine2(void){
    setCursor(0xC0);
    lcdPrint("                ");
    setCursor(0xC0);
}

void noBlink(void){
    if (cursor_value)
        send8ToLCD(0x0E);
    else
        send8ToLCD(0x0C);
}

void noCursor(void){
    if (cursor_value)
        send8ToLCD(0x0D);
    else
        send8ToLCD(0x0C);
}

int getCursorPos(void){
    return cursor_pos;
}