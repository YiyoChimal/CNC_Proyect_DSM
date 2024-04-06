/*
 * GeneralMotor.c
 *
 * Created: 02/04/2024 06:41:14 p. m.
 *  Author: yiyoc
 */ 
#include <avr/io.h>
#include "StepMotor.h"

 // RPM 120//Revoluciónes por minuto
 // Sr 400//Pasos por vuelta
  // Rs 8//Distancia por vuelta del tornillo ACME en mm
 // Tt 10 //Periodo del timer en Us


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




