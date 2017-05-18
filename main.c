#include <msp430.h> 
#include "UART.h"

/*
 * main.c
 */

unsigned char var_test = 0;
void _config_IO()
{
    P1DIR = 0x01;
    P1REN |= 0x08;
    P1OUT = 0x08;
}
void _config_InterruptIO()
{
    P1IE |= 0x08;
    P1IES |= 0x08;
    P1IFG &= ~(0x08);
    _BIS_SR(GIE);
}
void _config_Timmer0_A0()
{
    BCSCTL3 |= LFXT1S_2;
    TA0CCR0 = 3000;
    TA0CTL = MC_0;
    TA0CCTL0 = CCIE;
    _BIS_SR(GIE);
}
void _config_Timmer1_A0()
{
    BCSCTL3 |= LFXT1S_2;
    TA1CCR0 = 750;
    TA1CTL = MC_0;
    TA1CCTL0 = CCIE;
    _BIS_SR(GIE);
}
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    UART_init();
    _config_IO();
    _config_InterruptIO();
    _config_Timmer0_A0();
    _config_Timmer1_A0();
	return 0;
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void myTimer1ISR(void)
{
    P1OUT ^= 0x01;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void myTimer0ISR(void)
{
    UART_printf_string("ACTIVE");
}
#pragma vector=PORT1_VECTOR
__interrupt void Port1(void)
{
    if(var_test == 0)
    {
        UART_printf_string("ACTIVE");
        TA0CTL = TASSEL_1 + ID_3 + MC_1;
        var_test ++;
    }
    else
    {
        UART_printf_string("STOP");
        TA0CTL = MC_0;
        var_test = 0;
    }
    P1IFG &= ~(0x08);
}
