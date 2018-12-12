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
	
    while (1) 
    {
		uint8_t tempL;
		uint8_t tempH;
		uint8_t humidityL;
		uint8_t humidityH;
		_delay_ms(2000);
		DDRD = (1 << PORTD7);
		PORTD = (1 << PORTD7);
		_delay_ms(1);
		PORTD = 0;
		_delay_us(20);
		cli();
		DDRD = 0;
		_delay_ms(10);
		_delay_us(160);
		for(int i = 0; i < 8; i++){
			tempL = tempL | (((PORTD >> PORTD7) & 0x80)<<i);
			_delay_us(50);
		}
		for(int i = 0; i < 8; i++){
			tempH = tempH | (((PORTD >> PORTD7) & 0x80)<<i);
			_delay_us(50);
		}
		for(int i = 0; i < 8; i++){
			humidityL = humidityL | (((PORTD >> PORTD7) & 0x80)<<i);
			_delay_us(50);
		}
		for(int i = 0; i < 8; i++){
			humidityH = humidityH | (((PORTD >> PORTD7) & 0x80)<<i);
			_delay_us(50);
		}
		sei();
		USART_send(tempH);
		USART_send(tempL);
				
    }
}

