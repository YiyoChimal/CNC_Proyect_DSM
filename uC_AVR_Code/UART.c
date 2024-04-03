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
	UBRR0H = (BRC>>8); //Configuraci�n la velocidad de transmisi�n 
	UBRR0L = BRC;
	UCSR0B |= ((1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0)); //Habilita la transmisi�n y recepcci�n, asi como la interrupci�n para recepci�n
	UCSR0C |=  ((1<<UCSZ00)|(1<<UCSZ01)); // Tama�o de palabra 8 bits, comunicaci�n asincrona, paridad desactivada, modo asyncrono
}

void anexaSerial(int dato){
	while(!(UCSR0A&(1<<5)));    // mientras el registro UDR0 est� lleno espera
	UDR0 = dato;            //cuando el el registro UDR0 est� vacio se envia el caracter
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





//char resibeSerial(){	
	//char *Cadena= ComandoStr; 
	//while(*Cadena) return *Cadena++; 
//}



