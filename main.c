
/*
* C program designed to turn on off 2 connected lamps simultaneously every 2 seconds
*/
#include "tm4c123gh6pm.h"
#include <stdio.h>

void timer0A_delayMs(int ttime);
void RelayedLight(void);

int main(void){
RelayedLight(void);
}


////////////////////////////////////////////////////////////////////////////////
///////////////millisecond delay timer////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void timer0A_delayMs(int ttime)
{
    int i;
    SYSCTL_RCGCTIMER_R |= 1;     /* enable clock to Timer Block 0 */

    TIMER0_CTL_R = 0;            /* disable Timer before initialization */
    TIMER0_CFG_R = 0x04;         /* 16-bit option */
    TIMER0_TAMR_R = 0x02;        /* periodic mode and down-counter */
    TIMER0_TAILR_R = 16000 - 1;  /* Timer A interval load value register */
    TIMER0_ICR_R = 0x1;          /* clear the TimerA timeout flag*/
    TIMER0_CTL_R |= 0x01;        /* enable Timer A after initialization */

    for(i = 0; i < ttime; i++) { while ((TIMER0_RIS_R & 0x1) == 0) ;      /* wait for TimerA timeout flag */
        TIMER0_ICR_R = 0x1;      /* clear the TimerA timeout flag */
    }
}

void RelayedLight(void){

volatile unsigned long delay;
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // activate port D
delay = SYSCTL_RCGC2_R;      // allow time for clock to stabilize
GPIO_PORTD_DIR_R |= 0x0F;    // make PD3-0 out
GPIO_PORTD_AFSEL_R &= 0xF0; // regular port function
GPIO_PORTD_DEN_R |= 0x0F;    // enable digital I/O on PD3-0
GPIO_PORTD_DATA_R = 0x00;

while(1){

    timer0A_delayMs(2000);
    GPIO_PORTD_DATA_R = 0x08;    // enable digital I/O on PD3-0
    timer0A_delayMs(2000);		// 2 second delay
    GPIO_PORTD_DATA_R = ~0x08;	       // disable digital I/O on PD3-0
    timer0A_delayMs(2000);
    GPIO_PORTD_DATA_R = 0x04;
    timer0A_delayMs(2000);
    GPIO_PORTD_DATA_R = ~0x04;
    }
}
