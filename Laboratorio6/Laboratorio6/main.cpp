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
void stringTermi(char* oracion); 
void ADC_1(); 
uint8_t lecADC(); 
void menu(); 
void num(uint8_t numero);

/****************************************/
// Main Function
int main (void)
{
	setup();
	serialLECT('E');
	serialLECT('G');
	serialLECT('3');
	stringTermi("\n El delay es del diablo");
	menu();
	while (1)
	{
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	cli(); 
	DDRB = 0xFF; 
	serialUART();
	ADC_1(); 
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

void stringTermi(char* oracion)
{
	for (uint8_t i = 0; *(oracion+i) != '\0'; i++)
	{
		serialLECT(*(oracion+i));
	}
}

void ADC_1()
{
	//ADMUX = 0;
	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1 << MUX0) |  (1 << MUX1) | (1<<MUX2); //
	
	//ADCSRA = 0;
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN);// |  (1<<ADIE) ;
	ADCSRA |= (1<<ADSC);
}

uint8_t lecADC()
{
	ADCSRA |= (1<<ADSC); //Empezar conversión
	while (ADCSRA & (1<<ADSC)); 
	return ADCH; //Leer los valores altos
}

void menu()
{
	stringTermi("\n\nMenu:\n");
	stringTermi("1. Leer Potenciometro\n");
	stringTermi("2. Enviar ASCII\n");
	stringTermi("Seleccione una opcion:\n");
}

void num(uint8_t numero)
{
	//se separa el numero sus cifras correspondientes
	uint8_t centenas = numero / 100; //saber centenas
	uint8_t decenas = (numero % 100) / 10; //saber decenas
	uint8_t unidades = numero % 10;	//saber unidades
	
	//Es el punto de origen del conteo
	serialLECT(centenas + '0');
	serialLECT(decenas + '0');
	serialLECT(unidades + '0');
}
/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	//mostrarLect(UDR0);
	uint8_t opcion = UDR0; // Capturar dato recibido

	if (opcion == '1')
	{
		uint8_t lectura = lecADC(); // Leer ADC7
		num(lectura); // Enviar como carácter ASCII
		stringTermi("\n"); // Nueva línea para orden
	}
	else if (opcion == '2')
	{
		stringTermi("\nEnviando ASCII...\n");
	}
	else
	{
		stringTermi("\nOpcion invalida.\n");
	}
	
	menu(); // Volver a mostrar el menu
} 