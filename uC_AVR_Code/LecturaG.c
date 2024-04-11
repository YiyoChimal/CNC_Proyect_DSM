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


char RxBuffer[10][20],instruccion[20],corX[20],corY[20],corZ[20];
volatile unsigned char RxContador_G=0,j=0; 
int identi=0;



unsigned char LeerCodigo(char rx){
	
	
	if(rx == '*'){
		
		//Cerarra las cadenas de caracteres;
		RxBuffer[j][RxContador_G]= '\0';
	
		//Imprimir la cadena de cararcters leida.
		anexaSerial(10);
		for (int i=0;i<=j;i++){
			for (int p=0;p<20;p++){anexaSerial(RxBuffer[i][p]);}
			anexaSerial(32);
		}
		anexaSerial(10);
		
		//Detección de instrucción y eje de cordenadas 
		for(int Ins; Ins<=j; Ins++){
			if (RxBuffer[j][0]=='G' || RxBuffer[j][0]=='M' ){
				for (int d=0;d<20;d++){instruccion[d]=RxBuffer[j][d+1];}
			}else if (RxBuffer[j][0]=='X'){
				for (int d=0;d<20;d++){corX[d]=RxBuffer[j][d+1];}
			}else if (RxBuffer[j][0]=='Y'){
				for (int d=0;d<20;d++){corY[d]=RxBuffer[j][d+1];}
			}else if (RxBuffer[j][0]=='Z'){
				for (int d=0;d<20;d++){corZ[d]=RxBuffer[j][d+1];}
				}else if(strstr(RxBuffer,"Homming")){
					// PORTC^=(1<<PINC0); 
				}
			}
		
		//Reiniciar los contadores y vector 
		for (int i=0;i<=j;i++){
			for (int p=0;p<20;p++){RxBuffer[i][p]=0;}
		}
		RxContador_G = 0;
		j=0;
		

		}else{	//lectura de caracteres 
		
		if (rx==' '){
			RxBuffer[j][RxContador_G]= '\0';
			j++;//inicador de un espacio
			RxContador_G=0; 
			}else{
			RxBuffer[j][RxContador_G++]=rx; //Almacena la cadena de caracteres
			 
			}
		}
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