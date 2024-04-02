/*
 * PruebasCNCC.c
 *
 * Created: 30/03/2024 07:35:40 p. m.
 * Author : yiyoc
 */ 
#define F_CPU 16000000
#define BAUD 9600

#include "UART/UART.h"
#include "Timer/Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h> //Cadena de caracteres; 
#include <stdio.h>

char ComandoStr[20];
volatile unsigned char RxContador=0;
char rx='\0';
unsigned char numero=12; 
volatile unsigned int count0=0,count1=0; 

int main(void)
{
	cli();
	DDRD|= (1<<PIND4)|(1<<PIND5)|(1<<PIND6)|(1<<PIND7); //Pulse 1, Pulse 2, Pulse 3, Dir1
	DDRB|=(1<<PINB0)|(1<<PINB1); //Dir2,Dir3
	
	DDRC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2); //LEDS prueba comunicación serial 
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
	
	
	//Timer de 10us, el motor tarda 200 pasos en dar una vuelta 
	count0++;  //Ejemplo con un periodo de 2.5ms (1.25ms por cada pulso en alto y bajo) para tener una frecuancia de trabajo de 120rpm
				//2 vueltas por segundo 
		
	
	if (count1<=400) //Ejemplo de una vuelta 200 para el pulso en bajo y 200 para el pulso en alto
	{
		if (count0>125) //Frecuancia 400 pulsos por segundo 
		{	
			PORTD|=(1<<PIND7); 
			PORTD^= (1<<PIND4); //Pulse 1
			
			count0=0;
			count1++; 
		}
	
	}else if(count1>400 & count1<=800){
		
		if (count0>125) //Frecuancia 400 pulsos por segundo
		{
			PORTD^= (1<<PIND5); //Pulse 2
			PORTB|=(1<<PINB0);
			count0=0;
			count1++;
		}
	}
	
	
	
	
	
}

//ISR(TIMER1_COMPB_vect){
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
