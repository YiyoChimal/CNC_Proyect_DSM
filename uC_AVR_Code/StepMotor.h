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
	float Vl; //Velosidad linea
	//Movimiento
	float DeltaT; 
	float TDT; 
	float SDT; 
	
	//void (*Init)(struct GeneralMotor *Datos); //Nombre de la función, datos a dar en la funcion en ester cadao la estructura
	
	
	}GeneralMotor;

	void CNC_Init(GeneralMotor *Datos,float RPM,float Sr,float Rs,float Tt); 


	typedef struct{
		
		//Iniciales
		float Vln; //Velocidad lineal motor del eje
		float Fmn; //Frecuencia del motor del eje n
		float Tmn; //Periodo del motor del eje n
		
		//Movimiento
		float ni; //Posición inicial
		float nf; //Posiciópn final
		float Deltan; //Delta de la distancia recorrida por el motor n;
		float TDn; //Timpo de la delta
		float SDn;  //Pasos de la delta
		float GDn; //Contante de delta 
		volatile unsigned int CountF; //Contador para la frecuencia 
		volatile unsigned int CountS; //Contador para los Pasos 
		
	}AxisMotor;

	

#endif /* GENERALMOTOR_H_ */

