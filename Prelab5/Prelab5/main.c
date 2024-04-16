/******************************************************************************
; Universidad del Valle de Guatemala
; 1E2023: Programacion de Microcontroladores
; main.c
; Autor: Jacob Tabush
; Proyecto: Prelaboratorio 5
; Hardware: ATMEGA328P
; Creado: 15/04/2024
; Ultima modificacion: 15/04/2024
*******************************************************************************/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "PWM/PWM.h"

void initADC(void);
char changesomebits(char oldvalue, char bitstochange, char newvalue);

uint16_t ADCResult = 0;

void setup(void)
{
	cli();

	initPWM1FastTopA(reset, no_invertido, 64, 2499);
	
	UCSR0B = 0; // Deshablitamos serial en D0 y D1
	
	initADC();
	
	sei();
}

int main(void)
{
	setup();
	updateDutyCycle1A(150);
	while (1)
	{
		ADCSRA |= (1<<ADSC); //reiniciamos el ADC
		_delay_ms(100);
		
	}
}

void initADC(void) //Funcion para inicializar el ADC
{
	ADMUX = 0;
	
	ADMUX |= (1<<REFS0); //conectamos a AVcc
	ADMUX &= ~(1<<REFS1);
	
	ADMUX |= 0b00000110; //Seleccionamos A6
	
	ADMUX |= (1<<ADLAR); // Justificado a la izquierda
	
	ADCSRA |= (1<<ADEN); //Encendemos el ADC
	ADCSRA |= (1<<ADIE); // Encendemos el interrupt
	ADCSRA |= (0b00000111); //Prescaler de 128
}

ISR(ADC_vect){
	ADCResult = ADCH + 65;
	ADCSRA |= (1<<ADIF);
	
	updateDutyCycle1A(ADCResult);
	
	return;
	
}

char changesomebits(char oldvalue, char bitstochange, char newvalue) //Funcion para solo cambiar algunos bits en un registro
{	char result = (newvalue & bitstochange) | (oldvalue & ~bitstochange);
	return result;
}
