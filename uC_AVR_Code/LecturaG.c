/*
* LecturaG.c
*
* Created: 07/04/2024 09:32:01 p. m.
*  Author: Axel&yiyoc
*/

//#include <util/setbaud.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "UART/UART.h"
#include "LecturaG.h"


char RxBuffer[20][30],instruccion[30],corX[30],corY[30],corZ[30];
volatile unsigned char RxContador_G=0,RxContador2=0;

int j=0,identi=0;



unsigned char LeerCodigo(char rx){
	
	
	if(rx == '*'){
		
		//Cerarra las cadenas de caracteres;
		RxBuffer[j][RxContador_G]= '\0';
		instruccion[RxContador2++]='\0';
		corX[RxContador2++]='\0';
		corY[RxContador2++]='\0';
		corZ[RxContador2++]='\0';
		//Reiniciar los contadores;
		RxContador_G = 0;
		RxContador2 = 0;
		j=0;
		//Imprimir la cadena de cararcters leida.
		anexaSerial(10);
		for (int i=0;i<20;i++)
		{
			for (int p=0;p<30;p++)
			{
				
			anexaSerial(RxBuffer[i][p]);
				
				
			}
			anexaSerial(32);
			
		}
		anexaSerial(10);
		
		
		for (int d=0;d<30;d++)
		{
			instruccion[d]=RxBuffer[0][d+1];
		}
		
		for (int d=0;d<30;d++)
		{
			corX[d]=RxBuffer[1][d+4];
			
		}
		
		for (int d=0;d<30;d++)
		{
			corY[d]=RxBuffer[2][d+14];
		}
		for (int d=0;d<30;d++)
		{
			corZ[d]=RxBuffer[3][d+25];
		}
		
		
		//printf("%f",2.3568);
		//
		//escribeFlAChar(83.112543,9);

		}else{
		//RxBuffer[RxContador++] = dato;
		if (rx==' '){
			j++;//inicador de un espacio
			}else{
			RxBuffer[j][RxContador_G++]=rx; //Almacena la cadena de caracteres
		}
	}
	
	return 0;
}

float GetConfi(){
	float tipo;
	char *ptr;
	tipo = strtod(instruccion, &ptr);
	
	return tipo;
}

float GetCordenadaX(){
	char *ptr;
	float cordenadaX;
	cordenadaX = strtod(corX, &ptr);
	return cordenadaX;
}

float GetCordenadaY(){
	char *ptr;
	float cordenadaY;
	cordenadaY = strtod(corY, &ptr);
	return cordenadaY;
}

float GetCordenadaZ(){
	char *ptr;
	float cordenadaZ;
	cordenadaZ = strtod(corZ, &ptr);
	return cordenadaZ;
}