/*
 * PruebasCNCC.c
 *
 * Created: 30/03/2024 07:35:40 p. m.
 * Author : axelf&yiyoc
 */ 
#define F_CPU 16000000
#define BAUD 9600

#include "LecturaG.h"
#include "Timer/Timer.h" //Timer para PWM (Control de motores)
#include "StepMotor/StepMotor.h" //Definir las caracteristicas de los motores a paso
#include "UART/UART.h" //Comunicación serial


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
	GeneralMotor Mg;
	AxisMotor Mx;
	AxisMotor My;
	AxisMotor Mz;
	
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
//Punto cero_Cordenadas de la cama
#define H_X 1.0
#define H_Y 1.0
#define H_Z 1.0


float Ejem=-255.1265666; 

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
	
	//Ejemplo 
	Mx.ni=8.0;
	Mx.nf=0.0; 
	My.ni=0.0;
	My.nf=0.0;
	Mz.ni=0.0;
	Mz.nf=0.0;
	
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

//if (Mg.OneShot==0){Home(&Mx,&My,&Mz,&Mg,H_X,H_Y,H_Z);Mg.OneShot=1;}

	if ((Mx.ni != Mx.nf)&(My.ni != My.nf)){
		if (Mg.OneShot==0){Mg.OneShot=1;Two_Axis(&Mx,&My,&Mg);}
		Move_XY_Axis(&Mx,&My,&Mg);
		}else if((Mx.ni != Mx.nf)&(My.ni == My.nf)){
		if(Mg.OneShot==0){Mg.OneShot=1;One_Axis(&Mx,&Mg);}
		Move_X_Axis(&Mx,&Mg);
		}else if((Mx.ni == Mx.nf)&(My.ni != My.nf)){
		if(Mg.OneShot==0){Mg.OneShot=1;One_Axis(&My,&Mg);}
		Move_Y_Axis(&My,&Mg);
		}else if(Mz.ni != Mz.nf){
		if(Mg.OneShot==0){Mg.OneShot=1;One_Axis(&Mz,&Mg);}
		Move_Z_Axis(&Mz,&Mg);
	}
		
	//escribeFlAChar(Mz.SDn,5);
	//saltoLinea();
	//_delay_ms(100);	
			
    }
}

ISR(INT0_vect){
	Mg.GoHome=1; 
	//PORTC|=(1<<PINB0); 
}
ISR(INT1_vect){
	Mg.GoHome=1; 
	//PORTC&=~(1<<PINB0); 
}

ISR(PCINT0_vect){
	if (PINB & (1<<PINB4)>0){ //Solo cuando el pulso sea de subida
		Mg.GoHome=1; 
		//PORTC|=(1<<PINB0);
		////PORTC&=~(1<<PINB0); 
	}	
	//PORTC^= (1<<PINC0);
	
}

ISR(TIMER1_COMPA_vect){	
	Mg.CountT1++;
	Mg.CountT2++;
}



ISR(USART_RX_vect){
	rx= UDR0;
	LeerCodigo(rx);
	Mx.nf=GetCordenadaX();
	My.nf=GetCordenadaY();
	Mz.nf=GetCordenadaZ();
}

void One_Axis(AxisMotor *Datos, GeneralMotor *DatosG);

void One_Axis(AxisMotor *Datos, GeneralMotor *DatosG){
	//Datos de congifuración; 
	Datos->Vln=DatosG->Vl; //Velolcidad linean
	Datos->Fmn=DatosG->Fm; //Frecuencia
	Datos->Tmn=DatosG->Tm; //Periodo 
	
	//Datos de movimiento 
	Datos->Deltan=abs(Datos->nf-Datos->ni); //Delta
	//Datos->TDn=Datos->Deltan/Datos->Vln; //Tiempo en recorrer la distancia 
	Datos->SDn=(Datos->Deltan/DatosG->Dp)*2; //Pasos para recorer la distancia 
	//Dir 
}

void Two_Axis(AxisMotor *DatosX,AxisMotor *DatosY, GeneralMotor *DatosG);

void Two_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG){
	
	//Calculo de la Delta
	DatosX->Deltan=abs(DatosX->nf-DatosX->ni); //Delta X
	DatosY->Deltan=abs(DatosY->nf-DatosY->ni); //Delta Y
	//Dir x
	//Dir y
	DatosG->DeltaT=sqrt(pow(DatosX->Deltan,2)+pow(DatosY->Deltan,2));
	DatosG->TDT=DatosG->DeltaT/DatosG->Vl;
	
	//Para Eje X 
	DatosX->TDn=DatosX->Deltan/DatosG->Vl; 
	DatosX->GDn=DatosG->TDT/DatosX->TDn;
	//DatosX->Vln=DatosG->Vl/DatosX->GDn;
	DatosX->Fmn=DatosG->Fm/DatosX->GDn;
	DatosX->Tmn=((1/DatosX->Fmn)*1000/DatosG->MT)-3;	//Periodo modificado 
	DatosX->SDn=(DatosX->Deltan/DatosG->Dp)*2; 
	
	//Para Eje Y 
	DatosY->TDn=DatosY->Deltan/DatosG->Vl; 
	DatosY->GDn=DatosG->TDT/DatosY->TDn;
	//DatosY->Vln=DatosG->Vl/DatosY->GDn;
	DatosY->Fmn=DatosG->Fm/DatosY->GDn;
	DatosY->Tmn=((1/DatosY->Fmn)*1000/DatosG->MT)-3;	//Periodo modificado 
	DatosY->SDn=(DatosY->Deltan/DatosG->Dp)*2;
}



void  Move_X_Axis(AxisMotor *Datos,GeneralMotor *DatosG); 

void  Move_X_Axis(AxisMotor *Datos,GeneralMotor *DatosG){
	
	if (DatosG->OneShotDir==0){DatosG->OneShotDir=1; DatosG->CountT1=0; if((Datos->nf-Datos->ni)>0){DirX_P}else{DirX_N}}//Dir
	
	if (Datos->CountS<=Datos->SDn){ //Conteo de pasos 
		if(DatosG->CountT1>Datos->Tmn){ //Conteo para Periodo 
			PulseX//Pulse
			DatosG->CountT1=0; 
			Datos->CountS++; 
		}
	}else{
		Datos->ni=Datos->nf; 
		//Prueba 
		Datos->CountS=0; 
		DatosG->CountT1=0;
		DatosG->CountT2=0;  
		DatosG->OneShot=0; 
		DatosG->OneShotDir=0; 
		
	}
}

void Move_Y_Axis(AxisMotor *Datos,GeneralMotor *DatosG);

void Move_Y_Axis(AxisMotor *Datos,GeneralMotor *DatosG){

}
void Move_Z_Axis(AxisMotor *Datos,GeneralMotor *DatosG);

void Move_Z_Axis(AxisMotor *Datos,GeneralMotor *DatosG){

}

void Move_XY_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG); 

void Move_XY_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG){

	 
	if (DatosG->OneShotDir==0){DatosG->OneShotDir=1; DatosG->CountT1=0; DatosG->CountT2=0;if ((DatosX->nf-DatosX->ni)>0){DirX_P}else{DirX_N} if ((DatosY->nf-DatosY->ni)>0){DirY_P}else{DirY_N}}//Dir
	
	if (DatosX->CountS<=DatosX->SDn){
		if (DatosG->CountT1>DatosX->Tmn){ //Conteo para Periodo
			PulseX //Pulse
			DatosG->CountT1=0; 
			DatosX->CountS++;
		}
	}
	
	if (DatosY->CountS<=DatosY->SDn){
		if (DatosG->CountT2>DatosY->Tmn){ //Conteo para Periodo
			PulseY //Pulse
			DatosG->CountT2=0;
			DatosY->CountS++;
		}
	}else{
		//Posición de inicio y final 
		DatosX->ni=DatosX->nf; 
		DatosY->ni=DatosY->nf;
		//Contadores 
		DatosX->CountS=0;
		DatosY->CountS=0;
		DatosG->CountT1=0; 
		DatosG->CountT2=0; 
		DatosG->OneShot=0;
		DatosG->OneShotDir=0;
	}
	
	
}

void Home(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG,float Hm_X,float Hm_Y,float Hm_Z ); 
void Home(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG,float Hm_X,float Hm_Y,float Hm_Z ){
	//Distancia a recorrer 
	float Dc_X=15.0,Dc_Y=15.0,Dc_Z=15.0; //Distancia a la cama; 
	
	DatosX->nf=Hm_X+Dc_X; 
	DatosY->nf=Hm_Y+Dc_Y; 	
	DatosZ->nf=Hm_Z+Dc_Z; 	
	
	DatosX->Deltan=abs(DatosX->nf-DatosX->ni); //Delta X
	DatosY->Deltan=abs(DatosY->nf-DatosY->ni); //Delta Y
	DatosZ->Deltan=abs(DatosZ->nf-DatosZ->ni); //Delta Z
	
	//Pasos para el origen 
	DatosX->SDn=(DatosX->Deltan/DatosG->Dp)*2;//Pasos X
	DatosY->SDn=(DatosY->Deltan/DatosG->Dp)*2;//Pasos Y
	DatosZ->SDn=(DatosZ->Deltan/DatosG->Dp)*2;//Pasos Z
	
	//Movimiento 
	DirX_P DirY_P DirZ_P //Direcciones hacio los switch
	
	//Eje Z
	DatosG->GoHome=0; //Activar recorrido hacia eje Z
	DatosG->CountT1=0; //Reinicio de timer 
	
	while(DatosG->GoHome==0){
	if (DatosG->CountT1>DatosG->Tm){ //Conteo para Periodo
		PulseZ
		DatosG->CountT1=0;
	}
	}
	
	_delay_ms(100);
	DirZ_N
	DatosG->CountT1=0; //Reinicio de timer 
	while(DatosZ->CountS<=(DatosZ->SDn)){	//Conteo de pasos 
			if(DatosG->CountT1>DatosG->Tm){ //Conteo para Periodo
				PulseZ 
				DatosZ->CountS++;
				DatosG->CountT1=0;
			}
	}
	
	
	//Eje X
	DatosG->GoHome=0;//Activar recorrido Hacia eje X
	DatosG->CountT1=0;//Reincio de timer 
	
	while(DatosG->GoHome==0){
		if (DatosG->CountT1>DatosG->Tm){ //Conteo para Periodo
			PulseX
			DatosG->CountT1=0;
		}
	}
	
	_delay_ms(100);
	DirX_N
	DatosG->CountT1=0; //Reinicio de timer
	
	while(DatosX->CountS<=(DatosX->SDn)){	//Conteo de pasos
		if(DatosG->CountT1>DatosG->Tm){ //Conteo para Periodo
			PulseX
			DatosX->CountS++;
			DatosG->CountT1=0;
		}
	}
	
	
	//Eje Y
	DatosG->GoHome=0;//Activar recorrido Hacia eje X
	DatosG->CountT1=0;//Reincio de timer
	
	while(DatosG->GoHome==0){
		if (DatosG->CountT1>DatosG->Tm){ //Conteo para Periodo
			PulseY
			DatosG->CountT1=0;
		}
	}
	
	_delay_ms(100);
	DirY_N
	DatosG->CountT1=0; //Reinicio de timer
	
	while(DatosY->CountS<=(DatosY->SDn)){	//Conteo de pasos
		if(DatosG->CountT1>DatosG->Tm){ //Conteo para Periodo
			PulseY
			DatosY->CountS++;
			DatosG->CountT1=0;
		}
	}
	
	//Reinico de contadores  
	DatosG->GoHome=0;//Reiniciar
	DatosG->CountT1=0;//Reincio de timer
	DatosG->CountT2=0;
	DatosX->CountS=0;
	DatosY->CountS=0;
	DatosZ->CountS=0;
	//Establecer punto cero 
	DatosX->ni=DatosX->nf=0; 
	DatosY->ni=DatosY->nf=0; 
	DatosZ->ni=DatosZ->nf=0; 
}


void Init_Counters_Data(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG); 
void Init_Counters_Data(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG){
	//Cordenadas
	DatosG->GoHome=0; 
	DatosX->ni=DatosX->nf=0;
	DatosY->ni=DatosY->nf=0;
	DatosZ->ni=DatosZ->nf=0;
	//Contadores
	DatosX->CountS=0;
	DatosY->CountS=0;
	DatosZ->CountS=0;
	DatosG->CountT1=0; 
	DatosG->CountT2=0; 
	DatosG->OneShot=0;
	DatosG->OneShotDir=0;
}
