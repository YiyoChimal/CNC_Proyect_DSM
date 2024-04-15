/*
 * GeneralMotor.c
 *
 * Created: 02/04/2024 06:41:14 p. m.
 *  Author: yiyoc
 */ 

 // RPM 120//Revoluciónes por minuto
 // Sr 400//Pasos por vuelta
  // Rs 8//Distancia por vuelta del tornillo ACME en mm
 // Tt 10 //Periodo del timer en Us
 
#include <avr/io.h>
#include <math.h> // Para Calculos
#include <util/delay.h>
#include "StepMotor.h"


void CNC_Init(GeneralMotor *Datos,float RPM, float Sr, float Rs,float Tt){
//Unidades en mm, seg 

	//Caracteristicas para todos los motores a pasos
	Datos->Sd=Sr/Rs; //pasos para 1mm
	Datos->Dp=(1.00/(Datos->Sd)); //Distancia por paso
	
	//Caracteristicas para Cada motor Mx,My,Mz
	Datos->Rps=RPM/60; //Rps=Revolución por segundo
	Datos->Fm=Datos->Rps*Sr; //Frecuancia de motor o St
	Datos->Tm=(1/(Datos->Fm))*1000; //Periodo motores *1000
	Datos->MT=(Tt/1000); //Modificador de periodo
	Datos->Tm=((Datos->Tm)/Datos->MT)-3; 	//Periodo modificado 
	Datos->Vl=Datos->Fm*Datos->Dp; //Velosidad lineal
	
}


void One_Axis(AxisMotor *Datos, GeneralMotor *DatosG){
	//Datos de congifuración;
	Datos->Vln=DatosG->Vl; //Velolcidad linean
	Datos->Fmn=DatosG->Fm; //Frecuencia
	Datos->Tmn=DatosG->Tm; //Periodo
	
	//Datos de movimiento
	Datos->Deltan=fabs(Datos->nf-Datos->ni); //Delta
	//Datos->TDn=Datos->Deltan/Datos->Vln; //Tiempo en recorrer la distancia
	Datos->SDn=(Datos->Deltan/DatosG->Dp)*2; //Pasos para recorer la distancia
	//Dir
}

void Two_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG){
	
	//Calculo de la Delta
	DatosX->Deltan=fabs(DatosX->nf-DatosX->ni); //Delta X
	DatosY->Deltan=fabs(DatosY->nf-DatosY->ni); //Delta Y
	
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


void  Move_X_Axis(AxisMotor *Datos,GeneralMotor *DatosG){
	
	if((Datos->nf-Datos->ni)>0){DirX_P}else{DirX_N} //Dir
	DatosG->CountT1=0; //Reiniciar Contador
	
	while (Datos->CountS<=Datos->SDn){ //Conteo de pasos
		if(DatosG->CountT1>Datos->Tmn){ //Conteo para Periodo
			PulseX//Pulse
			DatosG->CountT1=0;
			Datos->CountS++;
		}
	}
	
	//Igualar Coordenadas
	Datos->ni=Datos->nf;
	//Prueba
	Datos->CountS=0;
	DatosG->CountT1=0;
	DatosG->CountT2=0;
	DatosG->OneShot=0;
	//DatosG->OneShotDir=0;
}


void Move_Y_Axis(AxisMotor *Datos,GeneralMotor *DatosG){
	if((Datos->nf-Datos->ni)>0){DirY_P}else{DirY_N} //Dir
	DatosG->CountT1=0; //Reiniciar Contador
	
	while (Datos->CountS<=Datos->SDn){ //Conteo de pasos
		if(DatosG->CountT1>Datos->Tmn){ //Conteo para Periodo
			PulseY//Pulse
			DatosG->CountT1=0;
			Datos->CountS++;
		}
	}
	
	//Igualar Coordenadas
	Datos->ni=Datos->nf;
	//Prueba
	Datos->CountS=0;
	DatosG->CountT1=0;
	DatosG->CountT2=0;
	DatosG->OneShot=0;
	//DatosG->OneShotDir=0;
}

void Move_Z_Axis(AxisMotor *Datos,GeneralMotor *DatosG){
	if((Datos->nf-Datos->ni)>0){DirZ_P}else{DirZ_N} //Dir
	DatosG->CountT1=0; //Reiniciar Contador
	
	while (Datos->CountS<=Datos->SDn){ //Conteo de pasos
		if(DatosG->CountT1>Datos->Tmn){ //Conteo para Periodo
			PulseZ//Pulse
			DatosG->CountT1=0;
			Datos->CountS++;
		}
	}
	
	//Igualar Coordenadas
	Datos->ni=Datos->nf;
	//Prueba
	Datos->CountS=0;
	DatosG->CountT1=0;
	DatosG->CountT2=0;
	DatosG->OneShot=0;
	//DatosG->OneShotDir=0;
}

void Move_XY_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG){

	
	if ((DatosX->nf-DatosX->ni)>0){DirX_P}else{DirX_N}//Dir
	if ((DatosY->nf-DatosY->ni)>0){DirY_P}else{DirY_N}//Dir
	DatosG->CountT1=0; DatosG->CountT2=0;//Reiniciar los contadores
	
	while(DatosG->OneShot==0){
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
			//Activar One Shot
			DatosG->OneShot=1;
		}
	}
	
	
	//Posición de inicio y final
	DatosX->ni=DatosX->nf;
	DatosY->ni=DatosY->nf;
	//Contadores
	DatosX->CountS=0;
	DatosY->CountS=0;
	DatosG->CountT1=0;
	DatosG->CountT2=0;
	DatosG->OneShot=0;
	//DatosG->OneShotDir=0;
	
	
}


void Home(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG,float Hm_X,float Hm_Y,float Hm_Z ){
	//Distancia a recorrer
	float Dc_X=15.0,Dc_Y=15.0,Dc_Z=15.0; //Distancia a la cama;
	
	DatosX->nf=Hm_X+Dc_X;
	DatosY->nf=Hm_Y+Dc_Y;
	DatosZ->nf=Hm_Z+Dc_Z;
	
	DatosX->Deltan=fabs(DatosX->nf-DatosX->ni); //Delta X
	DatosY->Deltan=fabs(DatosY->nf-DatosY->ni); //Delta Y
	DatosZ->Deltan=fabs(DatosZ->nf-DatosZ->ni); //Delta Z
	
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
	DatosG->OneShotHome=0; 
	DatosX->CountS=0;
	DatosY->CountS=0;
	DatosZ->CountS=0;
	
	//Establecer punto cero
	DatosX->ni=DatosX->nf=0;
	DatosY->ni=DatosY->nf=0;
	DatosZ->ni=DatosZ->nf=0;
	
}

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
	//DatosG->OneShotDir=0;
}


