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

#define PulseX PORTD^= (1<<PIND4); //Eje X
#define DirX_P PORTD|=(1<<PIND7);
#define DirX_N PORTD&=~(1<<PIND7);

#define PulseY PORTD^= (1<<PIND5);	//Eje Y
#define DirY_P PORTB|=(1<<PINB0);
#define DirY_N PORTB&=~(1<<PINB0);

#define PulseZ PORTD^= (1<<PIND6);	//Eje Z
#define DirZ_P PORTB|=(1<<PINB1);
#define DirZ_N PORTB&=~(1<<PINB1);



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

	//Pines control de motores a paso
	DDRD|= (1<<PIND4)|(1<<PIND5)|(1<<PIND6)|(1<<PIND7); //Pulse 1, Pulse 2, Pulse 3, Dir1
	DDRB|=(1<<PINB0)|(1<<PINB1); //Dir2,Dir3
	
	//Configuracion inicial de motores a paso
	CNC_Init(&Mg,RPM,Sr,Rs,Tt);
	Init_Counters(&Mx,&Mg);
	Init_Counters(&My,&Mg);
	Init_Counters(&Mz,&Mg);
	
	//Ejemplo 
	Mx.ni=1.0;
	Mx.nf=17.0; 
	My.ni=0.0;
	My.nf=0.0;
	Mz.ni=0.0;
	Mz.nf=0.0;
	Mx.Tmn=9;
	Mx.SDn=400;
	
	
	
	//LEDS prueba comunicación serial 
	DDRC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2); 
	PORTC&= ~(1<<PINC0)|(1<<PINC1)|(1<<PINC2);
	initserial(); 
	timer1_init(); 
	timer1_on();

	sei();
	
	
	
    while (1) 
    {
		//escribeFlAChar(Mg.Vl,3);
		//saltoLinea();
		//_delay_ms(100);
		
		//Two_Axis(&Mx,&My,&Mg);					
    }
}



ISR(TIMER1_COMPA_vect){	
	
	
	unsigned int tiempo=9, pasos=100; 
	
	Mx.CountT++; //Contador del Periodo
	
	if (Mx.CountS<=pasos){ //Conteo de pasos
		if (Mx.CountT>tiempo){ //Conteo para Periodo
			//if (DatosG->OneShotDir==0){DatosG->OneShotDir=1;if (Datos->Deltan>0){DirX_P}else{DirX_N}}//Dir
			PulseX //Pulse
			Mx.CountT=0;
			Mx.CountS++;
		}
	}
	
		//count0++; 	//Timer de 10us, el motor tarda 400 pasos en dar una vuelta
		////Ejemplo con un periodo de 1.25ms (0.625ms por cada pulso en alto y bajo) para tener una frecuancia de trabajo de 120rpm
		////2 vueltas por segundo
		//if (count1<=800) //Ejemplo de una vuelta 400 para el pulso en bajo y 400 para el pulso en alto
		//{
			//if (count0>9) //Frecuancia 400 pulsos por segundo
			//{
				////PORTD|=(1<<PIND7);
				//PORTD^= (1<<PIND4); //Pulse 1
				//count0=0;
				//count1++;
				//
				//}}//}else{PORTD&=~(1<<PIND4);} //Mantener el pulso en bajo
		//
	
	//Move_X_Axis(/*&Mx,&Mg*/);
	//if ((Mx.ni != Mx.nf)&(My.ni != My.nf)){
		//if (Mg.OneShot==0){Mg.OneShot=1;Two_Axis(&Mx,&My,&Mg);}
		//Move_XY_Axis(&Mx,&My,&Mg);		
	//}else if((Mx.ni != Mx.nf)&(My.ni == My.nf)){
		//if(Mg.OneShot==0){Mg.OneShot=1;One_Axis(&Mx,Mg);}
		//Move_X_Axis(&Mx,Mg);	
	//}else if((Mx.ni == Mx.nf)&(My.ni != My.nf)){
		//if(Mg.OneShot==0){Mg.OneShot=1;One_Axis(&My,Mg);}
		//Move_Y_Axis(&My,Mg);
	//}else if(Mz.ni != Mz.nf){
		//if(Mg.OneShot==0){Mg.OneShot=1;One_Axis(&Mz,Mg);}
		//Move_Z_Axis(&Mz,Mg);
	//}
	//PulseX
	
	
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


void One_Axis(AxisMotor *Datos, GeneralMotor *DatosG);

void One_Axis(AxisMotor *Datos, GeneralMotor *DatosG){
	//Datos de congifuración; 
	Datos->Vln=DatosG->Vl; //Velolcidad linean
	Datos->Fmn=DatosG->Fm; //Frecuencia
	Datos->Tmn=DatosG->Tm; //Periodo 
	
	//Datos de movimiento 
	Datos->Deltan=Datos->nf-Datos->ni; //Delta
	//Datos->TDn=Datos->Deltan/Datos->Vln; //Tiempo en recorrer la distancia 
	Datos->SDn=Datos->Deltan/DatosG->Dp; //Pasos para recorer la distancia 
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
	DatosX->Fmn=DatosG->Fm/DatosX->GDn;
	DatosX->Tmn=(1/DatosX->Fmn)/DatosG->MT;	//Periodo modificado 
	DatosX->SDn=DatosX->Deltan/DatosG->Dp; 
	
	//Para Eje Y 
	DatosY->GDn=DatosG->TDT/DatosY->TDn;
	//DatosY->Vln=DatosG->Vl/DatosY->GDn;
	DatosY->Fmn=DatosG->Fm/DatosY->GDn;
	DatosY->Tmn=(1/DatosY->Fmn)/DatosG->MT;	//Periodo modificado 
	DatosY->SDn=DatosY->Deltan/DatosG->Dp;
}

void  Move_X_Axis(/*AxisMotor *Datos,GeneralMotor *DatosG*/); 

void  Move_X_Axis(/*AxisMotor *Datos,GeneralMotor *DatosG*/){
	Mx.CountT++; //Contador del Periodo
	
	if (Mx.CountS<=400){ //Conteo de pasos
		if (Mx.CountT>10){ //Conteo para Periodo
			//if (DatosG->OneShotDir==0){DatosG->OneShotDir=1;if (Datos->Deltan>0){DirX_P}else{DirX_N}}//Dir
			PulseX //Pulse
			Mx.CountT=0;
			Mx.CountS++;
		}
	}

	//Datos->CountT++; //Contador del Periodo 
	//
	//if (Datos->CountS<=Datos->SDn){ //Conteo de pasos 
		//if (Datos->CountT>Datos->TDn){ //Conteo para Periodo 
			////if (DatosG->OneShotDir==0){DatosG->OneShotDir=1;if (Datos->Deltan>0){DirX_P}else{DirX_N}}//Dir
			//PulseX //Pulse
			//Datos->CountT=0; 
			//Datos->CountS++; 
		//}
	//}//else{
		//Datos->ni=Datos->nf; 
		////Prueba 
		//Datos->CountT=0; 
		//Datos->CountS=0; 
		//DatosG->OneShot=0; 
		//DatosG->OneShotDir=0; 
	//}
}

void Move_Y_Axis(AxisMotor *Datos,GeneralMotor *DatosG);

void Move_Y_Axis(AxisMotor *Datos,GeneralMotor *DatosG){
	Datos->CountT++; //Contador del Periodo
	
	if (Datos->CountS<=Datos->SDn){ //Conteo de pasos
		if (Datos->CountT){ //Conteo para Periodo
			if (DatosG->OneShotDir==0){DatosG->OneShotDir=1;if (Datos->Deltan>0){DirY_P}else{DirY_N}}//Dir
			PulseY //Pulse
			Datos->CountT=0;
			Datos->CountS++;
		}
		}else{
		Datos->ni=Datos->nf;
		//Prueba
		Datos->CountT=0;
		Datos->CountS=0;
		DatosG->OneShot=0;
		DatosG->OneShotDir=0;
	}
}
void Move_Z_Axis(AxisMotor *Datos,GeneralMotor *DatosG);

void Move_Z_Axis(AxisMotor *Datos,GeneralMotor *DatosG){
	Datos->CountT++; //Contador del Periodo
	
	if (Datos->CountS<=Datos->SDn){ //Conteo de pasos
		if (Datos->CountT){ //Conteo para Periodo
			if (DatosG->OneShotDir==0){DatosG->OneShotDir=1;if (Datos->Deltan>0){DirZ_P}else{DirZ_N}}//Dir
			PulseZ //Pulse
			Datos->CountT=0;
			Datos->CountS++;
		}
		}else{
		Datos->ni=Datos->nf;
		//Prueba
		Datos->CountT=0;
		Datos->CountS=0;
		DatosG->OneShot=0;
		DatosG->OneShotDir=0;
	}
}

void Move_XY_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG); 

void Move_XY_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG){
	DatosX->CountT++; 
	DatosY->CountT++; 
	if (DatosG->OneShotDir==0){DatosG->OneShotDir=1; if (DatosX->Deltan>0){DirX_P}else{DirX_N} if (DatosY->Deltan>0){DirY_P}else{DirY_N}}//Dir
	
	if (DatosX->CountS<=DatosX->SDn){
		if (DatosX->CountT){ //Conteo para Periodo
			PulseX //Pulse
			DatosX->CountT=0;
			DatosX->CountS++;
		}
	}
	
	if (DatosY->CountS<=DatosY->SDn){
		if (DatosY->CountT){ //Conteo para Periodo
			PulseY //Pulse
			DatosY->CountT=0;
			DatosY->CountS++;
		}
	}else{
		//Posición de inicio y final 
		DatosX->ni=DatosX->nf; 
		DatosY->ni=DatosY->nf;
		//Contadores 
		DatosX->CountT=0;
		DatosX->CountS=0;
		DatosY->CountT=0;
		DatosY->CountS=0;
		DatosG->OneShot=0;
		DatosG->OneShotDir=0;
	}
	
	
}

void Init_Counters(AxisMotor *Datos,GeneralMotor *DatosG){
	Datos->CountS=0;
	Datos->CountT=0;
	DatosG->OneShot=0;
	DatosG->OneShotDir=0;
}
