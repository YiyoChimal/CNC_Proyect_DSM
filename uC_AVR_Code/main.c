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

//Calculos
 #define RPM 120//Revoluciónes por minuto
 #define Sr 400//Pasos por vuelta
 #define Rs 8//Distancia por vuelta del tornillo ACME en mm
GeneralMotor Mg;
AxisMotor Mx;
AxisMotor My;
AxisMotor Mz; 

int main(void)
{
	cli();
	
	//Configuracion inicial de motores a paso
	CNC_Init(&Mg,RPM,Sr,Rs);  
	Axis_Set(&Mx,&Mg); 
	Axis_Set(&My,&Mg); 
	Axis_Set(&Mz,&Mg); 
	
	//Datos=&Mg; 
	DDRD|= (1<<PIND4)|(1<<PIND5)|(1<<PIND6)|(1<<PIND7); //Pulse 1, Pulse 2, Pulse 3, Dir1
	DDRB|=(1<<PINB0)|(1<<PINB1); //Dir2,Dir3
	
	DDRC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2); //LEDS prueba comunicación serial 
	PORTC&= ~(1<<PINC0)|(1<<PINC1)|(1<<PINC2);
	initserial(); 
	timer1_init(); 
	timer1_on();
	sei();
	
	
	
    while (1) 
    {
		escribeFlAChar(Mg.Vl,3);
		saltoLinea();
		_delay_ms(100);
    }
}



ISR(TIMER1_COMPA_vect){
	
	
	count0++; 	//Timer de 10us, el motor tarda 400 pasos en dar una vuelta 
				 //Ejemplo con un periodo de 1.25ms (0.625ms por cada pulso en alto y bajo) para tener una frecuancia de trabajo de 120rpm
				//2 vueltas por segundo 
		
	if (count1<=800) //Ejemplo de una vuelta 400 para el pulso en bajo y 400 para el pulso en alto
	{
		if (count0>125) //Frecuancia 400 pulsos por segundo 
		{	
			PORTD|=(1<<PIND7); 
			PORTD^= (1<<PIND4); //Pulse 1
			count0=0;
			count1++; 
		
	}}//}else{PORTD&=~(1<<PIND4);} //Mantener el pulso en bajo 
	
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


