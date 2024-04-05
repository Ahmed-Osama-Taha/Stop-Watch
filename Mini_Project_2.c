/*
 * Mini_Project_2.c
 *
 *  Created on: Sep 12, 2023
 *      Author: Ahmed Osama
 *      Certificate name: Ahmed Osama Taha
 *      Email: ahmed.osama23@msa.edu.eg
 */

# include <avr/io.h>
# include <avr/interrupt.h>
# define F_CPU 1000000UL
# include <util/delay.h>

unsigned char seconds=0;
unsigned mins =0;
unsigned hours =0;

unsigned char tick=0;
unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hour1=0;
unsigned char hour2=0;

void seconds1_display(void);
void seconds2_display(void);
void mins1_display(void);
void mins2_display(void);
void hours1_display(void);
void hours2_display(void);



void draw_seven_seg(unsigned char a);

int main(void){
	TCNT1 = 0;		    /* Set timer1 initial count to zero */
	OCR1A = 1000 ;       /* Set the compare value */
	TCCR1A=0b00000000;
	TCCR1B=0b00001101;  // timer 1 compare mode with 1024 prescaler
	TIMSK |=(1<<OCIE1A);
	SREG |= (1<<7);

	MCUCR |=(1<<ISC01); /* Interrupt zero confg.*/
	MCUCR &=~(1<<ISC00);
	GICR |=(1<<INT0);

	DDRD &=~(1<<2);  // button INT0
	DDRD &=~(1<<3);  // button INT1
	DDRB &=~(1<<2);  // button INT2

	MCUCR |=(1<<ISC11);  /* Interrupt one confg.*/
	MCUCR |=(1<<ISC10);
	GICR |=(1<<INT1);

	MCUCSR &=~(1<<ISC2); /* Interrupt two confg.*/
	 GICR |=(1<<INT2);


	DDRC =0x0F; // setting the first four bits at port c as output to the 7447 decoder

	while(1){
		 seconds1_display();
		 seconds2_display();
		 mins1_display();
		 mins2_display();
		 hours1_display();
		 hours2_display();
	}
}

ISR(INT0_vect){
    sec1=0;
	sec2=0;
	min1=0;
	min2=0;
    hour1=0;

	 while(!(PIND&(1<<2)));

}
ISR(INT1_vect){
	TCCR1B=0b00001000;
	while(PIND&(1<<3));
}
ISR(INT2_vect){
	TCCR1B=0b00001101;
	while(!(PINB&(1<<2)));
}

ISR(TIMER1_COMPA_vect){
	tick++;
	if(tick==1){
		sec1++;
		if (sec1>9){
			sec1=0;
			sec2++;
		}
		if(sec2>5){
			sec2=0;
			min1++;
		}
		if(min1>9){
			min1=0;
			min2++;
		}
		if(min2>5){
			min2=0;
			hour1++;
		}
		if(hour1>9){
			hour1=0;
			hour2++;
		}
		if(hour2>2){
			sec1=0;
			sec2=0;
			min1=0;
			min2=0;
			hour1=0;
			hour2=0;
		}

		tick=0;
	}
}
void seconds1_display(void){
						PORTA |= (1<<0); // Enable the first 7-segment
						PORTA &=~(1<<1); // disable the second 7-segment
						PORTA &=~(1<<2); // disable the third 7-segment
						PORTA &=~(1<<3); // disable the fourth 7-segment
						PORTA &=~(1<<4); // disable the fifth 7-segment
						PORTA &=~(1<<5); // disable the sixth 7-segment
						draw_seven_seg(sec1);
						_delay_ms(2);
}
void seconds2_display(void){
						PORTA &=~(1<<0); // disable the first 7-segment
						PORTA |= (1<<1);  // Enable the second 7-segment
						PORTA &=~(1<<2); // disable the third 7-segment
						PORTA &=~(1<<3); // disable the fourth 7-segment
						PORTA &=~(1<<4); // disable the fifth 7-segment
						PORTA &=~(1<<5); // disable the sixth 7-segment
						draw_seven_seg(sec2);
						_delay_ms(2);
}
void mins1_display(void){

						PORTA &=~(1<<0); // disable the first 7-segment
						PORTA &=~(1<<1);  // disable the second 7-segment
						PORTA |=(1<<2);  // enable the third 7-segment
						PORTA &=~(1<<3); // disable the fourth 7-segment
						PORTA &=~(1<<4); // disable the fifth 7-segment
						PORTA &=~(1<<5); // disable the sixth 7-segment
						draw_seven_seg(min1);
						_delay_ms(2);
}
void mins2_display(void){

						PORTA &=~(1<<0); // disable the first 7-segment
						PORTA &=~(1<<1);  // disable the second 7-segment
						PORTA &=~(1<<2);  // disable the third 7-segment
						PORTA |=(1<<3); // Enable the fourth 7-segment
						PORTA &=~(1<<4); // disable the fifth 7-segment
						PORTA &=~(1<<5); // disable the sixth 7-segment
						draw_seven_seg(min2);
						_delay_ms(2);

}
void hours1_display(void){

						PORTA &=~(1<<0); // disable the first 7-segment
						PORTA &=~(1<<1);  // disable the second 7-segment
						PORTA &=~(1<<2);  // disable the third 7-segment
						PORTA &=~(1<<3); // disable the fourth 7-segment
						PORTA |=(1<<4); // Enable the fifth 7-segment
						PORTA &=~(1<<5); // disable the sixth 7-segment
						draw_seven_seg(hour1);
						_delay_ms(2);
}
void hours2_display(void){
						PORTA &=~(1<<0);
						PORTA &=~(1<<1);
						PORTA &=~(1<<2);
						PORTA &=~(1<<3);
						PORTA &=~(1<<4);
						PORTA |= (1<<5);
						draw_seven_seg(hour2);
						_delay_ms(2);
}

void draw_seven_seg(unsigned char a){

	if (a==0){
	PORTC=0x00;
	}
	if (a==1){
	PORTC=0x01;
	}
	if (a==2){
	PORTC=0x02;
	}
	if (a==3){
	PORTC=0x03;
	}
	if (a==4){
	PORTC=0x04;
	}
	if (a==5){
	PORTC=0x05;
	}
	if (a==6){
	PORTC=0x06;
	}
	if (a==7){
	PORTC = 0x07;
	}
	if (a==8){
	PORTC =0x08;
	}
	if(a==9){
	PORTC = 0x09;
	}

}
