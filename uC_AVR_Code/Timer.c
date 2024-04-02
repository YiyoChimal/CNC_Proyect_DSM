/*
 * Timer.c
 *
 * Created: 17/10/2023 01:32:19 p. m.
 *  Author: yiyoc Author: marlon
 */ 

/*Time table 

1s=1
1ms=1000
10us=100000

The maxumun time is 3 us 

8 bits= 255
16 bits= 65535


*/


 #include "TIMER.h"
 #define Fclk1a 200000
 #define div1a 8
 
 #define Fclk1b 200000
 #define div1b 8
 
 //Timer 1a
 void timer1_init(){
	 
	 //Operation Mode = CTC
	 TCCR1A &=~ (1<<WGM10);
	 TCCR1A &=~ (1<<WGM11);
	 TCCR1B |=  (1<<WGM12);
	 TCCR1B &=~ (1<<WGM13);
	 
	 //Compare value Timer 1a
	 OCR1A = (16000000 /div1a/Fclk1a) - 1;
	
	//Compare value Timer 1b
	 OCR1A =(16000000 /div1b/Fclk1b) - 1; 
	
	 //enable interrupt 1a
	 TIMSK1 |= (1<<OCIE1A);
	 
	 //enable interrupt 1b
	 TIMSK1 |= (1<<OCIE1B);
 }

 void timer1_on(){

TCNT1 = 0x0000;


switch (div1a) {
	case 1:   TCCR1B |= (1<<CS10); TCCR1B &=~ (1<<CS11); TCCR1B &=~ (1<<CS12); break;
	case 8:	  TCCR1B &=~(1<<CS10); TCCR1B |=  (1<<CS11); TCCR1B &=~ (1<<CS12); break;
	case 64:  TCCR1B |=  (1<<CS11); TCCR1B |=  (1<<CS11); TCCR1B &=~ (1<<CS12); break;
	case 256: TCCR1B &=~ (1<<CS12); TCCR1B |=  (1<<CS11); TCCR1B &=~ (1<<CS12); break;
	case 1024: 	 TCCR1B |=  (1<<CS10); TCCR1B &=~ (1<<CS11); TCCR1B |=  (1<<CS12); break;
}
	
 }

 void timer1_off(){
	 
	 //Clock setting T1clock = 0 Hz
	 TCCR1B &=~ (1<<CS10);
	 TCCR1B &=~ (1<<CS11);
	 TCCR1B &=~ (1<<CS12);

 }


