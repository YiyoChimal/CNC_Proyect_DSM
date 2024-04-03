/*
 * GeneralMotor.c
 *
 * Created: 02/04/2024 06:41:14 p. m.
 *  Author: yiyoc
 */ 
#include <avr/io.h>
#include "StepMotor.h"


void CNC_Init(GeneralMotor *Datos,float RPM, float Sr, float Rs){

	//Caracteristicas para todos los motores a pasos
	Datos->Sd=Sr/Rs; //pasos para 1mm
	Datos->Dp=1/Datos->Sd; //Distancia por paso
	
	//Caracteristicas para Cada motor Mx,My,Mz
	Datos->Rps=RPM/60; //Rps=Revolución por segundo
	Datos->Fm=Datos->Rps*Sr; //Frecuancia de motor o St
	Datos->Tm=1/Datos->Fm; //Periodo motores
	Datos->Vl=Datos->Fm*Datos->Dp; //Velosidad lineal
	
}

void Axis_Set(AxisMotor *Datos1, GeneralMotor *Datos2){
	
}


//float Lineal_One_Axi(float ni, float nf, float Vln,float Dpn );
//
//float Lineal_Multiple_Axis(float Delta1,float Delta2,float Vl, float TDn, float Fm);
//
//float Lineal_One_Axi(float ni, float nf, float Vln,float Dpn ){
//float Deltan=0, TDn=0, SDn=0, Dirn=0; //Deltan=Distancia, TDn=Tiempo, SDn=Pasos, Dn=Dirección; ni=posición inicial, nf=posición final;
//Deltan=nf-ni;
//TDn=Deltan/Vln;
//SDn=Deltan/Dpn;
//
//}
//
//float Lineal_Multiple_Axis(float Delta1,float Delta2,float Vl, float TDn, float Fm){
//float DeltaT=0,TDT=0, GDn=0, Vln=0, Fmn=0, Tmn=0;
//DeltaT=sqrt(pow(Delta1,2)+pow(Delta2,2));
//TDT=DeltaT/Vln;
//GDn=TDT/TDn;
//Vln=Vl/GDn;
//Fmn=Fm/GDn;
//Tmn=1/Fmn;
//}
//
//
//
//
//typedef struct{
//float Rpsn;
//float Fmn;
//float Tmn;
//float Vln;
//}MotorMove;
