/*
 * HumidityMeasurement.c
 *
 * Created: 10/12/2018 08:27:23
 * Author : jzrhard06
 */ 

#ifndef F_CPU

#define  F_CPU 16000000UL

#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>



void USART_init() {
	UBRR0H = 0;
	UBRR0L = 0b001100111;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void USART_send(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

int main(void)
{
	USART_init();
	DDRD |= (1<<PORTD7);
	PORTD |= (1<<PORTD7);
    while (1) 
    {
		int8_t tempL;
		int8_t tempH;
		uint8_t humidityL;
		uint8_t humidityH;
		uint8_t retries, i;
		int8_t j;
		retries = i =j =0;
		uint8_t timeout = 0;
		_delay_ms(2000);
		
		//START READING	
		PORTD |= (1<<PORTD7);
		_delay_ms(250);
		
		//SET LOW FOR 20 ms
		DDRD &= ~(1<<PORTD7);
		PORTD &= ~(1<<PORTD7);
		_delay_ms(20);
		
		//TURN OFF INTERRUPTIONS (CRITICAL PHASE)
		cli();
		
		//END START SIGNAL
		PORTD |= (1<<PORTD7);
		_delay_us(40);
		
		//START TO READ
		DDRD |= (1<<PORTD7);
		PORTD |= (1<<PORTD7);
		_delay_us(10);
		
		//WAIT 80 us OF LOW LEVEL
		//while(!(PORTD & (1<<PORTD7)));
		_delay_us(80);
		
		//WAIT 80 us OF HIGH LEVEL
		//while((PORTD & (1<<PORTD7)));
		_delay_us(80);
		
		//READ BYTES
		for(int i = 0; i < 8; i++){
			tempL = tempL | (PORTD & (1<<PORTD7));
			_delay_us(50);
		}
		for(int i = 0; i < 8; i++){
			tempH = tempH | (PORTD & (1<<PORTD7));
			_delay_us(50);
		}
		for(int i = 0; i < 8; i++){
			humidityL = humidityL | (PORTD & (1<<PORTD7));
			_delay_us(50);
		}
		for(int i = 0; i < 8; i++){
			humidityH = humidityH | (PORTD & (1<<PORTD7));
			_delay_us(50);
		}
		
		//ENABLE INTERRUPTIONS (END OF CRITICAL PHASE)
		sei();
		
		USART_send(tempH+48);
		USART_send(tempL+48);
				
    }
}

