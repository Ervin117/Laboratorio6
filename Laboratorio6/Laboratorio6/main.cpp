/*
 * Laboratorio6.cpp
 *
 * Created: 4/19/2025 4:29:47 PM
 * Author : Ervin Gomez 231226
 */ 

/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************/
// Function prototypes
void setup();
void serialUART();
void serialLECT(char letra);
void mostrarLect(char dato);

/****************************************/
// Main Function
int main (void)
{
	setup();
	serialLECT('e');
	while (1)
	{
		
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	cli(); 
	serialUART();
	DDRB = 0xFF; 
	sei();
}

void serialUART()
{
	DDRD |= (1<< DDD1); 
	DDRD &= ~(1<<DDD0);
	
	//configurado para enviar y recibir. 
	//UCSR0A = 0; 
	UCSR0B |= (1<< RXCIE0) | (1<<RXEN0) | (1<<TXEN0); 
	UCSR0C |= (1<< UCSZ01) | (1<<UCSZ00);
	UBRR0 = 103; //valor calculado para los 9600
}
void serialLECT(char letra)
{
	while ((UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = letra; 
	
}
void mostrarLect(char dato)
{
	PORTB = dato; 
}

/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	mostrarLect(UDR0);
}