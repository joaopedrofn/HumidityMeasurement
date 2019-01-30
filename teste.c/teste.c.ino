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

#define PIN 6

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

int main(void)
{
  uint8_t data[5];
  USART_init();
  DDRD &= ~(1<<PIN);
  PORTD |= (1<<PIN);
    while (1) 
    {
    _delay_ms(2000);
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
    
    float f =((uint16_t)data[0]) << 8 | data[1];
    f *= 0.1;
    char h[5];
    String str;
    str = String(f, 6);
    str.toCharArray(h, 7);
    for(int i = 0; i < strlen(h); i++)
      USART_send(h[i]);
    USART_send('\n');
    }
}


