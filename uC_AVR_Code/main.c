/*
 * PruebasCNCC.c
 *
 * Created: 30/03/2024 07:35:40 p. m.
 * Author : axelf&yiyoc
 */ 
#define F_CPU 16000000
#define BAUD 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h> //Cadena de caracteres; 
#include <stdio.h>
#include <math.h> // Para Calculos 
#include "LecturaG.h"
#include "Timer/Timer.h" //Timer para PWM (Control de motores)
#include "StepMotor/StepMotor.h" //Definir las caracteristicas de los motores a paso
#include "UART/UART.h" //Comunicación serial

//Calculos
#define RPM 120.0//Revoluciónes por minuto
#define Sr 400.0//Pasos por vuelta
#define Rs 8.0//Distancia por vuelta del tornillo ACME en mm
#define Tt 10.0 //Periodo del timer en Us
//Punto cero_Cordenadas de la cama
#define H_X 1.0
#define H_Y 1.0
#define H_Z 1.0
//Resolución de curvas 
#define DivAngle 2.0 //Angulo a dividir

//StepMotors
volatile unsigned int count0=0,count1=0;
GeneralMotor Mg;
AxisMotor Mx;
AxisMotor My;
AxisMotor Mz;
Curve C; 
//Comunicación Serial 
char rx='\0';

int main(void){
	cli();

	//Pines control de motores a paso
	DDRD|= (1<<PIND4)|(1<<PIND5)|(1<<PIND6)|(1<<PIND7); //Pulse 1, Pulse 2, Pulse 3, Dir1
	DDRB|=(1<<PINB0)|(1<<PINB1); //Dir2,Dir3
	//Interrupciones globales 
	DDRD&=~(1<<PIND2)|(1<<PIND3); 
	DDRB&=~(1<<PINB4); 
	//Configuracion inicial de motores a paso
	
	CNC_Init(&Mg,RPM,Sr,Rs,Tt);
	Init_Counters_Data(&Mx,&My,&Mz,&Mg);
	
	//LEDS prueba comunicación serial
	DDRC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2); 
	PORTC&= ~(1<<PINC0)|(1<<PINC1)|(1<<PINC2);
	initserial(); 
	timer1_init(); 
	timer1_on();

	//Interruptions to detect the bits changing
	EICRA|=(1<<ISC11); //Rising edge on INT1
	EICRA|=(1<<ISC10);
	EIMSK|=(1<<INT1); //Enable INT1

	EICRA|=(1<<ISC01); //Rising edge on INT0
	EICRA|=(1<<ISC00);
	EIMSK|=(1<<INT0); //Enable INT0
	
	//Interrupciones por estado logico
	PCICR|=(1<<PCIE0); //Abilita las interrupciones PCINT de los puertos B
	PCMSK0|=(1<<PCINT4); //Para los puertos PB4

	sei();
	

    while (1) 
    {
		if(Mg.ComanG==0 || Mg.ComanG==1){
			if ((Mx.ni != Mx.nf)&(My.ni != My.nf)){
				Two_Axis(&Mx,&My,&Mg);
				Move_XY_Axis(&Mx,&My,&Mg);
				escribeSerial("FinisH");
				}else if((Mx.ni != Mx.nf)&(My.ni == My.nf)){
				One_Axis(&Mx,&Mg);
				Move_X_Axis(&Mx,&Mg);
				escribeSerial("FinisH");
				}else if((Mx.ni == Mx.nf)&(My.ni != My.nf)){
				One_Axis(&My,&Mg);
				Move_Y_Axis(&My,&Mg);
				escribeSerial("FinisH");
				}else if(Mz.ni != Mz.nf){
				One_Axis(&Mz,&Mg);
				Move_Z_Axis(&Mz,&Mg);
				escribeSerial("FinisH");
				}else if(Mg.OneShotHome==1){
				Home(&Mx,&My,&Mz,&Mg,H_X,H_Y,H_Z);
				escribeSerial("FinisH");
			}
		}else if(Mg.ComanG==2 || Mg.ComanG==3){
			if ((Mx.ni != Mx.nf)&(My.ni != My.nf)){
			CurveSet(&Mx,&My,&Mg,&C,DivAngle); 
			escribeSerial("FinisH");
			}
		}
		
    }
}

ISR(INT0_vect){
	Mg.GoHome=1; 
}
ISR(INT1_vect){
	Mg.GoHome=1; 
}

ISR(PCINT0_vect){
	if (PINB & (1<<PINB4)>0){ //Solo cuando el pulso sea de subida
		Mg.GoHome=1; 
	}	
}

ISR(TIMER1_COMPA_vect){	
	Mg.CountT1++; //Contador 1
	Mg.CountT2++; //Contador 2
}



ISR(USART_RX_vect){
	rx= UDR0;
	LeerCodigo(rx);
	Mg.ComanG=GetConfi(); 
	Mg.I=GetCordenadai();
	Mg.J=GetCordenadaj(); 
	Mx.nf=GetCordenadaX();
	My.nf=GetCordenadaY();
	Mz.nf=GetCordenadaZ();
	Mg.OneShotHome=GetGoHome(); 
}


