#include "xc.h"
#include "delay.h"
#include "i2c.h"

// flags
static int idle = 1;
static int acked = 0;
static int rx_complete = 0;
static int tx_complete = 0;

void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void){
    if (!I2C1STATbits.ACKSTAT){     // ACK received  
        acked = 1;
    }
    
    if (I2C1STATbits.RBF){          // RX completed      
        rx_complete = 1;
    }
    
    if (!I2C1STATbits.TBF){         // TX completed   
        tx_complete = 1;
    }

    if (!I2C1STATbits.IWCOL || !I2C1STATbits.BCL){    
        idle = 1;                   // bus is idle or no write collision
    }
    IFS1bits.MI2C1IF=0;
}

void reset_flags(){
    idle = 1;
    tx_complete = 0;
    rx_complete = 0;
    acked = 0;
}

void wait_idle(){
    while (!idle){
        // do nothing, just wait
    }
    reset_flags();
}

void i2c_start(){
    wait_idle();
    I2C1CONbits.SEN = 1;
}

void i2c_stop(){
    wait_idle();
    I2C1CONbits.PEN = 1;
}

void i2c_init(void){
    IEC1bits.MI2C1IE = 1;
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.I2CSIDL = 1;        // Stop in Idle mode
    I2C1CONbits.ACKDT = 0;          // Sends ACK during Acknowledge
    I2C1BRG = 4;                    // FSCL = 400 kHz
    I2C1CONbits.I2CEN = 1;
}

void i2c_tx(int data){
    wait_idle();
    idle = 0;
    I2C1TRN = data;

    while(!tx_complete){
        // do nothing, just wait
    }

    while(!acked){
        // do thing, just wait
    }
    reset_flags();
}

int i2c_rx(){
    int temp = 0;
    I2C1CONbits.ACKEN = 0;      // bit 4
    I2C1CONbits.RCEN = 0;       // bit 3
    I2C1CONbits.PEN = 0;        // bit 2
    I2C1CONbits.RSEN = 0;       // bit 1
    I2C1CONbits.SEN = 0;        // bit 0

    I2C1CONbits.RCEN = 1;       
    wait_idle();
    idle=0;

    while(!rx_complete){
        // do nothing
    }

    temp=I2C1RCV;
    reset_flags();
    return temp;
}

void send_ack(){
    wait_idle();
    idle = 0;
    I2C1CONbits.ACKEN = 1;
    idle = 1;
}

void send_nack(){
    wait_idle();
    idle = 0;
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    I2C1CONbits.ACKDT = 0;
    idle = 1;
}