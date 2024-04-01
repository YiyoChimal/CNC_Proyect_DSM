/*
 * PruebasCNCC.c
 *
 * Created: 30/03/2024 07:35:40 p. m.
 * Author : yiyoc
 */ 
#define F_CPU 16000000
#define BAUD 9600

#include "UART/UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
char string[20]; 


int main(void)
{
	cli();
	DDRC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2); 
	PORTC&= ~(1<<PINC0)|(1<<PINC1)|(1<<PINC2);
    initserial(); 
	sei();
	
    while (1) 
    {
	

	
	
		if (strstr(ComandoStr,"a on"))
		{
			PORTC|=(1<<PINC0);
			PORTC&= ~(1<<PINC1)|(1<<PINC2);
		}else if (strstr(ComandoStr,"b on")){
			PORTC|=(1<<PINC1);
			PORTC&= ~(1<<PINC0)|(1<<PINC2);
		}
		
		
    }
}



