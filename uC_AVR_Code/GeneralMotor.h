/*
 * GeneralMotor.h
 *
 * Created: 02/04/2024 06:19:02 p. m.
 *  Author: yiyoc
 */ 


#ifndef GENERALMOTOR_H_
#define GENERALMOTOR_H_


typedef struct{
	//Caracteristicas para todos los motores a pasos
	float Sd;//pasos para 1mm
	float Dp;//Distancia por paso
	
	//Caracteristicas para Cada motor Mx,My,Mz
	float Rps; //Rps=Revolución por segundo
	float Fm; //Frecuancia de motor o St
	float Tm; //Periodo motores
	float Vl; //Velosidad lineal
	
	//void (*Init)(struct GeneralMotor *Datos); //Nombre de la función, datos a dar en la funcion en ester cadao la estructura
	
	
	}GeneralMotor;

	void CNC_Init(GeneralMotor *Datos,float RPM,float Sr,float Rs); 



#endif /* GENERALMOTOR_H_ */