/*
 * usart_test.c
 *
 * Created: 30/01/2019 08:43:26
 * Author : mssso
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

void USART_send(unsigned int data){
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
}

int main(void)
{
  USART_init();
  uint8_t dataL, dataH;
    PRR = (0<<PRADC);
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  while (1)
  {
    _delay_ms(2000);
    
    //inicia conversão
    ADCSRA = (1 << ADSC);
    uint32_t count;
    while ((ADCSRA & (1<<ADSC)) == (1<<ADSC) );
        
    dataL = ADCL;
    dataH = ADCH;
    
    ADCSRA |= (1<<ADIF);
    
    float f =((uint16_t)dataH) << 8 | dataL;
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


