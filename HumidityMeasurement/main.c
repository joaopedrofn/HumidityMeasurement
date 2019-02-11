/*
 * HumidityMeasurement.c
 *
 * Created: 10/12/2018 08:27:23
 * Author : Joaopedrofn
 */ 

#ifndef F_CPU

#define  F_CPU 16000000UL

#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define PIN 7

void USART_init() {
  UBRR0H = 0;
  UBRR0L = 0b001100111;
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);
  UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void USART_send(unsigned int data){
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
}

unsigned char USART_receive() {
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}


void readDHT(){
	uint8_t data[5];
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	//INPUT PULL UP
	DDRD &= ~(1<<PIN);
	PORTD |= (1<<PIN);
	_delay_ms(1);

	//SET LINE AS LOW
	DDRD |= (1<<PIN);
	PORTD &= ~(1<<PIN);
	_delay_us(1100);

	uint32_t cycles[80];

	//CRITICAL AREA
	cli();
	DDRD &= ~(1<<PIN);
	PORTD |= (1<<PIN);
	_delay_us(40);

	//WAIT FOR 80us LOW SIGNAL AND THEN HIGH SIGNAL
	while((PIND & (1 << PIN)) == 0x00);
	while((PIND & (1 << PIN)) == (1 << PIN));

	for( int i = 0; i < 80; i+=2 ){
		uint32_t lowCycles  = cycles[2*i];
		uint32_t highCycles = cycles[2*i+1];
		uint32_t  count1, count2;
		count1 = count2 = 0;
		while((PIND & (1 << PIN)) == 0x00){
			count1++;
		}
		while((PIND & (1 << PIN)) != 0x00){
			count2++;
		}
		cycles[i] = count1;
		cycles[i+1] = count2;
	}

	//END OF CRITICAL AREA
	sei();

	//CONVERT PULSE LENGTH TO VALUE
	for (int i=0; i<40; ++i) {
		uint32_t lowCycles  = cycles[2*i];
		uint32_t highCycles = cycles[2*i+1];
		data[i/8] <<= 1;
		if (highCycles > lowCycles) {
			data[i/8] |= 1;
		}
	}
	
	char dataChar[10];
	itoa(((uint16_t)data[0] << 8|data[1]), dataChar, 10);
	USART_send(dataChar[0]);
	USART_send(dataChar[1]);
	USART_send(dataChar[2]);
	USART_send(dataChar[3]);
}

void readYL(){
	uint8_t data[2];
	
	//START CONVERTION
	ADCSRA |= (1<<ADSC);

	while ((ADCSRA & (1<<ADSC)) != 0 );

	data[1] = ADCL;
	data[0] = ADCH;
	char dataChar[10];
	itoa(((uint16_t)data[0] << 8|data[1]), dataChar, 10);
	USART_send(dataChar[0]);
	USART_send(dataChar[1]);
	USART_send(dataChar[2]);
	USART_send(dataChar[3]);
}


int main(void)
{
	USART_init();
	//DHT22 INITIAL SETTINGS
  DDRD &= ~(1<<PIN);
  PORTD |= (1<<PIN);
  //YL69 INITIAL SETTINGS
  PRR = (0<<PRADC);
  ADCSRA = (1<<ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) ;
  ADMUX = (1 << REFS0);
    while (1) 
    {
		unsigned char signal = USART_receive();
		switch(signal){
			case 'a': 
				readDHT();
				break;
			case 's':
				readYL();
				break;
		}
	}
}