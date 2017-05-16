/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: alarm_rtcc.h  
 * Author: Rusty Mina (rusty.mina@eee.upd.edu.ph)
           Owen Cabuyadao (jbcabuyadao@up.edu.ph)
 * Comments:
 * Revision history: 1
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __RTCC_CCP_COM__PLEX_ALARM__
#define	__RTCC_CCP_COM__PLEX_ALARM__
#endif

#include <xc.h> // include processor files - each processor file is guarded.
#include "delay.h"
#include <stdio.h>
#include "stdlib.h"
#include "libpic30.h"

//int rtcc_flag=0;

typedef  struct {
    unsigned short alrm_rtcc_sec;
    unsigned short alrm_rtcc_min;
    unsigned short alrm_rtcc_hour;
    unsigned short alrm_rtcc_wday;
    unsigned short alrm_rtcc_mday;
    unsigned short alrm_rtcc_mon;
    unsigned short alrm_rtcc_year;
} alrm_rtcc_t;

void alarmInit();
void disableAlarm();
void enableAlarm();    

//void __attribute__((interrupt,auto_psv)) _RTCCInterrupt(void);
