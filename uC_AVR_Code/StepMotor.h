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
	float Rps; //Rps=Revoluci�n por segundo
	float Fm; //Frecuancia de motor o St
	float Tm; //Periodo motores
	float Vl; //Velosidad lineal
	
	//void (*Init)(struct GeneralMotor *Datos); //Nombre de la funci�n, datos a dar en la funcion en ester cadao la estructura
	
	
	}GeneralMotor;

	void CNC_Init(GeneralMotor *Datos,float RPM,float Sr,float Rs); 


	typedef struct{
		
		//Iniciales
		float Vln; //Velocidad lineal motor del eje
		float Fmn; //Frecuencia del motor del eje n
		float Tmn; //Periodo del motor del eje n
		
		//Movimiento
		float ni; //Posici�n inicial
		float nf; //Posici�pn final
		float Deltan; //Delta de la distancia recorrida por el motor n;
		float TDn; //Timpo de la delta
		float SDn;  //Pasos de la delta
		
		
	}AxisMotor;

	void Axis_Set(AxisMotor *Datos1, GeneralMotor *Datos2);

#endif /* GENERALMOTOR_H_ */
