/********************
Mega 328PB 
********************/

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>

#include "n5110.h"

#define F_CPU 8000000

volatile char lazer_stat = 0;

ISR(INT0_vect) //interrupt 0 handler
{
	(lazer_stat == 0)?(lazer_stat = 1):(lazer_stat = 0);
}

void preset(void)
{
	// Port setup
	DDRD = 0x78;	//PWM pin setup
	PORTD = 0x87;

	// ADC setup
	ADCSRA |= (1<<ADEN) 		  	//ADC on
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	//Division factor 128 (64 kHz)
	ADMUX |= (0<<REFS1)|(1<<REFS0) 		//AVcc with external capacitor at AREF pin
	|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0); //Default ADC channel 0


	sei();		//enable global interrupts

	Lcd_init();	//enable lcd screen
	Lcd_clear();
}

int readADC ()
{
	ADCSRA |= (1<<ADSC); //Start conversion
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

int main(void)
{
	preset();

	char laz_onoff[4];

	while(1){
		Lcd_clear();
		if(lazer_stat == 1){
			PORTD |= (1<<PD4);
			laz_onoff = "ON";
		}else{
			PORTD |= (0<<PD4);
			laz_onoff = "OFF";
		}
		Lcd_prints(1, 1, FONT_1X, (unsigned char*)"Lazer:");
		Lcd_print(8, 1, FONT_1X, (unsigned char*) laz_onoff);

	}
}
