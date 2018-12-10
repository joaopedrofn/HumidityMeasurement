/*
 * HumidityMeasurement.c
 *
 * Created: 10/12/2018 08:27:23
 * Author : jzrhard06
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef F_CPU

#define  F_CPU 16000000UL

#endif

int main(void)
{
	
    /* Replace with your application code */
    while (1) 
    {
		uint16_t temp;
		uint16_t humidity;
		_delay_ms(20000);
		DDRD = (1 << PORTD7);
		PORTD = (1 << PORTD7);
		_delay_ms(25);
		PORTD = 0;
		_delay_ms(2);
		cli();
		PORTD = (1 << PORTD7);
		_delay_ms(4);
		DDRD = 0;
		_delay_ms(1);
		_delay_us(16);
		for(int i = 0; i < 16; i++){
			temp[i] = PORTD[PORTD7];
			_delay_us(5);
		}
		for(int i = 0; i < 16; i++){
			humidity[i] = PORTD[PORTD7];
			_delay_us(5);
		}
		
				
    }
}

