/*
 * UART.h
 *
 * Created: 30/03/2024 09:16:10 p. m.
 *  Author: yiyoc
 */ 

#ifndef UART_H_
#define UART_H_


#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 16000000 //Frecuancia de reloj de una placa de dezarrollo arduino uno, nano
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#define BRC ((F_CPU/16/BAUD)-1) //Formula para modo normal asincrono 


void initserial(); //Inicialización
void anexaSerial(int dato);  //Anexa las cadenas de datos
void saltoLinea(); //Salto de linea 
void escribeSerial(char *str); //Envia una cadena de datos
void escribeNumAChar(int num); //Envia un dato de tipo entero
void escribeFlAChar(float num, char ndecimal); //Envia un dato de tipo float 

//char resibeSerial(void); 



#endif /* UART_H_ */