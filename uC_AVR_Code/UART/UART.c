/*
 * UART.c
 *
 * Created: 30/03/2024 09:15:52 p. m.
 *  Author: yiyoc
 */ 

#include "UART.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>




void initserial(){
	UBRR0H = (BRC>>8); //Configuración la velocidad de transmisión 
	UBRR0L = BRC;
	UCSR0B |= ((1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0)); //Habilita la transmisión y recepcción, asi como la interrupción para recepción
	UCSR0C |=  ((1<<UCSZ00)|(1<<UCSZ01)); // Tamaño de palabra 8 bits, comunicación asincrona, paridad desactivada, modo asyncrono
}

void anexaSerial(int dato){
	UDR0=dato;
	_delay_ms(30);
}

void saltoLinea(){
	anexaSerial(10);
	anexaSerial(13);
}

void escribeSerial(char *str){
	while(*str) anexaSerial(*str++);
}


void escribeNumAChar(int num){
	
	//Covierte un numero (postivo o negativo) a cadena de caracteres
	char buffer[20];
	itoa(num, buffer, 10); //10: sistema numerico Decimal, 2:Sistema Numerico Binario
	escribeSerial(buffer);
	//int longitud = strlen(buffer); // Detecta la cantidad de caracteres que tiene una cadena
	//char miCadena[] = "-Hola, mundo!"; //Ejemplo de cadena
	//sprintf(buffer, "%f", numero); //Para numeros flotantes
}

void escribeFlAChar(float num, char ndecimal){
	int In=num;
	int dec=((num-In)*(pow(10,ndecimal)));
	char buffer[20];
	char dbuffer[20];
	itoa(In, buffer, 10); //10: sistema numerico Decimal, 2:Sistema Numerico Binario
	escribeSerial(buffer);
	anexaSerial(46); //Punto decimal
	itoa(dec, dbuffer, 10); //10: sistema numerico Decimal, 2:Sistema Numerico Binario
	escribeSerial(dbuffer);
}

char ComandoStr[20]='\n';
volatile unsigned char RxContador=0;  
char rx='\0'; 

ISR(USART_RX_vect){
	rx= UDR0;
	
	if (rx==10)
	{
		ComandoStr[RxContador]='\0'; 
		RxContador=0;
		if (strstr(ComandoStr,"a on"))
		{
			
		}else if (strstr(ComandoStr,"b on"))
		{
		}
		
		
	}else{
		ComandoStr[RxContador]=rx;	
	}
	
	
}

char resibeSerial(){
	return rx;
}


