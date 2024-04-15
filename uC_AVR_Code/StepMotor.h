/*
 * GeneralMotor.h
 *
 * Created: 02/04/2024 06:19:02 p. m.
 *  Author: yiyoc
 */ 


#ifndef GENERALMOTOR_H_
#define GENERALMOTOR_H_

#define PulseX PORTD^= (1<<PIND4); //Eje X
#define DirX_P PORTD|=(1<<PIND7);
#define DirX_N PORTD&=~(1<<PIND7);

#define PulseY PORTD^= (1<<PIND5);	//Eje Y
#define DirY_P PORTB|=(1<<PINB0);
#define DirY_N PORTB&=~(1<<PINB0);

#define PulseZ PORTD^= (1<<PIND6);	//Eje Z
#define DirZ_P PORTB|=(1<<PINB1);
#define DirZ_N PORTB&=~(1<<PINB1);


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
	float DeltaT; //Delta Total
	float TDT;  //Tiempo en recorrer la delta 
	float SDT; //Pasos para recorrer la delta 
	float MT; //Modificador del periodo
	volatile unsigned int CountT1; //Contador para el periodo uno
	volatile unsigned int CountT2; //Contador para el periodo uno
	volatile unsigned char OneShot; //Contador de un disparo
	//volatile unsigned char OneShotDir; //Contador para dirección  
	unsigned char GoHome; 
	volatile unsigned char OneShotHome; 
	//void (*Init)(struct GeneralMotor *Datos); //Nombre de la función, datos a dar en la funcion en ester cadao la estructura
	
	
	}GeneralMotor;

	typedef struct{
		
		//Iniciales
		float Vln; //Velocidad lineal motor del eje
		float Fmn; //Frecuencia del motor del eje n
		unsigned int Tmn; //Periodo del motor del eje n
		
		//Movimiento
		float ni; //Posición inicial
		float nf; //Posiciópn final
		float Deltan; //Delta de la distancia recorrida por el motor n;
		float TDn; //Timpo de la delta
		unsigned int SDn;  //Pasos de la delta
		float GDn; //Contante de delta 
		//volatile unsigned int CountT; //Contador para el periodo
		volatile unsigned int CountS; //Contador para los Pasos 
		
	}AxisMotor;
	
	void CNC_Init(GeneralMotor *Datos,float RPM,float Sr,float Rs,float Tt); 
	void One_Axis(AxisMotor *Datos, GeneralMotor *DatosG);
	void Two_Axis(AxisMotor *DatosX,AxisMotor *DatosY, GeneralMotor *DatosG);
	void  Move_X_Axis(AxisMotor *Datos,GeneralMotor *DatosG);
	void  Move_Y_Axis(AxisMotor *Datos,GeneralMotor *DatosG);
	void  Move_Z_Axis(AxisMotor *Datos,GeneralMotor *DatosG);
	void Move_XY_Axis(AxisMotor *DatosX,AxisMotor *DatosY,GeneralMotor *DatosG);
	void Home(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG,float Hm_X,float Hm_Y,float Hm_Z );
	void Init_Counters_Data(AxisMotor *DatosX,AxisMotor *DatosY,AxisMotor *DatosZ,GeneralMotor *DatosG);


#endif /* GENERALMOTOR_H_ */

