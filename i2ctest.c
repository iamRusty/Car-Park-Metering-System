/*
 * File:   i2ctest.c
 * Author: Rusty
 *
 * Created on May 6, 2017, 9:13 AM
 */

#include "xc.h"
#include "include/delay.h"
#include "include/lcd_4bit.h"
#include "include/i2c.h"

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=IO
#pragma config I2C1SEL=PRI 

// data vars
char indata[] = "CoE_115";

int main(void) {
    AD1PCFG = 0xFFFF;
    TRISB = 0x9FE0;

    
    lcdInit();
    lcdPrint("lcd test");
    
    i2c_init();
    
    /* Write to EEPROM
     * Store "CoE_115" in address 0x1000 of the EEPROM 
     */
    // send I2C start sequence
    i2c_start();

    // send EEPROM CTRL byte with Write enabled
    i2c_tx(0b10100000);
    // send EEPROM high address
    i2c_tx(0x10);
    // send EEPROM low address
    i2c_tx(0x00); 

    // send actual data;
    int _count_char_ = 0;
    while (_count_char_ < 7){
        i2c_tx(indata[_count_char_]); 
        _count_char_++;
    }
    i2c_tx(0);

    // send I2C stop sequence
    i2c_stop();

    delay(5); //5ms delay

    /* Read from EEPROM
     * Get data written previously in the EEPROM
     */

    // send I2C start sequence
    i2c_start();
    // send EEPROM CTRL byte with Write enabled
    i2c_tx(0b10100000);
    // send EEPROM high address
    i2c_tx(0x10);
    // send EEPROM low address
    i2c_tx(0x00);
    // send I2C start sequence
    i2c_start();
    // send EEPROM CTRL byte with Read enabled
    i2c_tx(0b10100001);

    // receive data
    char lcdString[8];
    _count_char_ = 0;
    while (_count_char_ < 7){
        lcdString[_count_char_] = i2c_rx();

        // send ACK, but not on last byte
        if (_count_char_ < 6)
        send_ack();
        _count_char_++;
    }
    lcdString[_count_char_] = 0;
    
    // send I2C stop sequence
    i2c_stop();

    // print on next line   
    setCursor(0xC0);
    lcdPrint(lcdString);

    while(1){   
        // Display data
    }
    
    return 0;
}