/*
    a - RB7 (output)
    b - RA0 (input)
    c - RB5 (output)
    d - RA3 (input)
    e - RB10 (output)
    f - RA2 (input)
    g - RA1 (input)
*/

// This will optimally work in 4MHz environment
#include "xc.h"
#include "keypad.h"
#include "interrupt_cfg.h"
#define DEB_MAX 10

static int key;
static int is_pressed = 0;

void keyOn(void){
    IEC1bits.CNIE = 1;  
    return;
}
void keyOff(void){
    IEC1bits.CNIE = 0;
    return;
}

void keypadInit(void){
    CNPU1 = CNPU1 | 0x000C; // Bit 2 and 3
    CNPU2 = CNPU2 | 0x6000; // Bit 13 and 14
    
    CNEN1 = CNEN1 | 0x000C; // Bit 2 and 3
    CNEN2 = CNEN2 | 0x6000; // Bit 13 and 14
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;    
}


void resetPullup(void){
    IEC1bits.CNIE = 0;    
    LATBbits.LATB5 = 0;
    LATBbits.LATB7 = 0;
    LATBbits.LATB10 = 0;
    IEC1bits.CNIE = 1; 
    return;
}

void setIsPressed(int value){
    is_pressed = value;
    return;
}

int getIsPressed(void){
    return is_pressed;
}

int getKeyValue(void){
    return key;
}

void setKey(int value){
    key = value;
}

#if __ENABLE_KEYPAD_INTERRUPT_INSIDE_LIBRARY__
void __attribute__((interrupt)) _CNInterrupt(void){
    int deb_ctr = 0; //debounce counter  
    
    if (!PORTAbits.RA0){
        LATBbits.LATB5 = 0;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 1;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }
            
        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 0;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 2;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 3;
            is_pressed = 1;
            IFS1bits.CNIF = 0;   
            return;
        }
    }
    else if (!PORTAbits.RA1){
        LATBbits.LATB5 = 0;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 4;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 0;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 5;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 6;
            is_pressed = 1;
            IFS1bits.CNIF = 0;    
            return;
        }
    }
    else if (!PORTAbits.RA2){
        LATBbits.LATB5 = 0;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 7;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 0;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 8;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 9;
            is_pressed = 1;
            IFS1bits.CNIF = 0;  
            return;
        }
    }
    else if (!PORTAbits.RA3){
        LATBbits.LATB5 = 0;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 10;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 0;
        LATBbits.LATB10 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 0;
            is_pressed = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB5 = 1;
        LATBbits.LATB7 = 1;
        LATBbits.LATB10 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            key = 11;
            is_pressed = 1;
            IFS1bits.CNIF = 0; 
            return;
        }
    }       
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}
#endif