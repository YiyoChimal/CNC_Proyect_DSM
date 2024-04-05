/*
 * GeneralMotor.c
 *
 * Created: 02/04/2024 06:41:14 p. m.
 *  Author: yiyoc
 */ 
#include <avr/io.h>
#include "StepMotor.h"


void CNC_Init(GeneralMotor *Datos,int RPM, int Sr, int Rs,unsigned int Tt){

	//Caracteristicas para todos los motores a pasos
	Datos->Sd=Sr/Rs; //pasos para 1mm
	Datos->Dp=1/Datos->Sd; //Distancia por paso
	
	//Caracteristicas para Cada motor Mx,My,Mz
	Datos->Rps=RPM/60; //Rps=Revolución por segundo
	Datos->Fm=Datos->Rps*Sr; //Frecuancia de motor o St
	Datos->Tm=1/Datos->Fm; //Periodo motores
	Datos->MT=(Tt/2000); //Modificador de periodo
	Datos->Tm=(Datos->Tm)/Datos->MT; 	//Periodo modificado 
	Datos->Vl=Datos->Fm*Datos->Dp; //Velosidad lineal
	
}




