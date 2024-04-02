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
#include <string.h> //Cadena de caracteres; 
#include <stdio.h>

char ComandoStr[20];
volatile unsigned char RxContador=0;
char rx='\0';
unsigned char numero=12; 

int main(void)
{
	cli();
	DDRB|= (1<<PINB3)|(1<<PINB4);
	DDRC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2); 
	PORTC&= ~(1<<PINC0)|(1<<PINC1)|(1<<PINC2);
	initserial(); 
	timer1_init(); 
	timer1_on();
	sei();
	
	
	
    while (1) 
    {
		
    }
}

ISR(TIMER1_COMPA_vect){
	PORTB ^= (1<<PORTB4)|(1<<PORTB3);
	//PORTB ^= (1<<PORTB3);
}

//ISR(TIMER1_COMPB_vect){
	//PORTB ^= (1<<PORTB3);
//}

ISR(USART_RX_vect){
	rx= UDR0;
	
	
	if (rx=='*'){
		ComandoStr[RxContador]='\0';
		RxContador=0;
		if (strstr(ComandoStr,"a on")){PORTC|=(1<<PINC0);PORTC&= ~(1<<PINC1)|(1<<PINC2);
		}else if (strstr(ComandoStr,"b on")){ PORTC|=(1<<PINC1); PORTC&= ~(1<<PINC0)|(1<<PINC2);}
		
	}else{ComandoStr[RxContador++]=rx;}
	
	
}
