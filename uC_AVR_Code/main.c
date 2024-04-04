/*
 * PruebasCNCC.c
 *
 * Created: 30/03/2024 07:35:40 p. m.
 * Author : yiyoc
 */ 
#define F_CPU 16000000
#define BAUD 9600

#include "UART/UART.h" //Comunicación serial
#include "Timer/Timer.h" //Timer para PWM (Control de motores)
#include "StepMotor/StepMotor.h" //Definir las caracteristicas de los motores a paso

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h> //Cadena de caracteres; 
#include <stdio.h>
#include <math.h> // Para Calculos 
//RX
char ComandoStr[20]; //Almacenamiento de strings
volatile unsigned char RxContador=0; //Contador para la resepción de datos
char rx='\0'; //Almacenador de datos tipo Char
unsigned char numero=12; 

//StepMotors
volatile unsigned int count0=0,count1=0;

//Calculos
 #define RPM 120//Revoluciónes por minuto
 #define Sr 400//Pasos por vuelta
 #define Rs 8//Distancia por vuelta del tornillo ACME en mm
 #define Tt 10 //Periodo del timer en Us
GeneralMotor Mg;
AxisMotor Mx;
AxisMotor My;
AxisMotor Mz; 

int main(void)
{
	cli();
	
	//Configuracion inicial de motores a paso
	CNC_Init(&Mg,RPM,Sr,Rs,Tt);  
	
	
	//Datos=&Mg; 
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
		escribeFlAChar(Mg.Vl,3);
		saltoLinea();
		_delay_ms(100);
    }
}



ISR(TIMER1_COMPA_vect){
//Timer de 10us, el motor tarda 400 pasos en dar una vuelta
//Ejemplo con un periodo de 1.25ms (0.625ms por cada pulso en alto y bajo) para tener una frecuancia de trabajo de 120rpm
//2 vueltas por segundo
		
	count0++; 	
	if (count1<=800) //Ejemplo de una vuelta 400 para el pulso en bajo y 400 para el pulso en alto
	{
		if (count0>125){ //Frecuancia 400 pulsos por segundo	
			PORTD|=(1<<PIND7); 
			PORTD^= (1<<PIND4); //Pulse 1
			count0=0;
			count1++; 	
	}}//}else{PORTD&=~(1<<PIND4);} //Mantener el pulso en bajo 
	
}



ISR(USART_RX_vect){
	rx= UDR0;
	
	if (rx=='*'){
		ComandoStr[RxContador]='\0';
		RxContador=0;
		if (strstr(ComandoStr,"a on")){PORTC|=(1<<PINC0);PORTC&= ~(1<<PINC1)|(1<<PINC2);
			}else if (strstr(ComandoStr,"b on")){ PORTC|=(1<<PINC1); PORTC&= ~(1<<PINC0)|(1<<PINC2);}	
	}else{ComandoStr[RxContador++]=rx;}		
}


void One_Axis(AxisMotor *Datos1, GeneralMotor *Datos2);

void One_Axis(AxisMotor *Datos1, GeneralMotor *Datos2){
	//Datos de congifuración; 
	Datos1->Vln=Datos2->Vl; //Velolcidad linean
	Datos1->Fmn=Datos2->Fm; //Frecuencia
	Datos1->Tmn=Datos2->Tm; 
	
	//Datos de movimiento 
	Datos1->Deltan=Datos1->nf-Datos1->ni; //Delta
	//Datos1->TDn=Datos1->Deltan/Datos1->Vln; //Tiempo en recorrer la distancia 
	Datos1->SDn=Datos1->Deltan/Datos2->Dp; //Pasos para recorer la distancia 
	//Dir 
}

void Two_Axis(AxisMotor *DatosX,AxisMotor *DatosY, GeneralMotor *DatosG);

void Two_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG){
	
	//Calculo de la Delta
	DatosX->Deltan=DatosY->nf-DatosX->ni; //Delta X
	DatosY->Deltan=DatosY->nf-DatosY->ni; //Delta Y
	//Dir x
	//Dir y
	DatosG->DeltaT=sqrt(pow(DatosX->Deltan,2)+pow(DatosY->Deltan,2));
	DatosG->TDT=DatosG->DeltaT/DatosG->Vl;
	
	//Para Eje X 
	DatosX->GDn=DatosG->TDT/DatosX->TDn;
	//DatosX->Vln=DatosG->Vl/DatosX->GDn;
	//DatosX->Fmn=DatosG->Fm/DatosX->GDn;
	DatosX->Tmn=DatosG->Tm/DatosX->GDn;
	DatosX->SDn=DatosX->Deltan/DatosG->Dp; 
	
	//Para Eje Y 
	DatosY->GDn=DatosG->TDT/DatosY->TDn;
	//DatosY->Vln=DatosG->Vl/DatosY->GDn;
	//DatosY->Fmn=DatosG->Fm/DatosY->GDn;
	DatosY->Tmn=DatosG->Tm/DatosY->GDn;
	DatosY->SDn=DatosY->Deltan/DatosG->Dp;
}

void Move_One_Axis(AxisMotor *Datos); 

void Move_One_Axis(AxisMotor *Datos){
	
	
		count0++;
		if (count1<=800) //Ejemplo de una vuelta 400 para el pulso en bajo y 400 para el pulso en alto
		{
			if (count0>125){ //Frecuancia 400 pulsos por segundo
				PORTD|=(1<<PIND7);
				PORTD^= (1<<PIND4); //Pulse 1
				count0=0;
				count1++;
				}}//}else{PORTD&=~(1<<PIND4);} //Mantener el pulso en bajo
	
}