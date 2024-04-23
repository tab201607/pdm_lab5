/******************************************************************************
; Universidad del Valle de Guatemala
; 1E2023: Programacion de Microcontroladores
; main.c
; Autor: Jacob Tabush
; Proyecto: Laboratorio 5
; Hardware: ATMEGA328P
; Creado: 17/04/2024
; Ultima modificacion: 17/04/2024
*******************************************************************************/

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "PWM.h"

void initADC(void);
void convertADC(char channel);
char changesomebits(char oldvalue, char bitstochange, char newvalue);



uint16_t ADCResultServo1 = 0;
uint8_t ADCResultServo2 = 0;
uint8_t ADCChannel = 6;


void setup(void)
{
	cli();
	
	CLKPR = (0b10000000); // habilitamos cambios del prescale	
	CLKPR = (0x03); // Colocamos prescaler de 8

	initPWM1FastTopA(reset, no_invertido, 8, 2499); //activamos el timer 0 con tiemo
	//initPWM2FastTop(no_invertido, 128, 156);
	initPWM2FastB(reset, no_invertido, 128);
	DDRD = 0xFF;
	
	//UCSR0B = 0; // Deshablitamos serial en D0 y D1
	
	initADC();
	
	sei();
}

int main(void)
{
	setup();
	updateDutyCycle1A(150);
	updateDutyCycle2B(10);
	while (1)
	{
		ADCChannel = 6; //iniciamos el ADC convirtiendo el canal 6
		convertADC(ADCChannel);
		_delay_ms(100);
		
		ADCChannel = 7; //iniciamos el ADC convirtiendo el canal 6
		convertADC(ADCChannel);
		_delay_ms(100);
	}
}

void initADC(void) //Funcion para inicializar el ADC
{
	ADMUX = 0;
	
	ADMUX |= (1<<REFS0); //conectamos a AVcc
	ADMUX &= ~(1<<REFS1);
	
	ADMUX |= (1<<ADLAR); // Justificado a la izquierda
	
	ADCSRA |= (1<<ADEN); //Encendemos el ADC
	ADCSRA |= (1<<ADIE); // Encendemos el interrupt
	ADCSRA |= (0b00000111); //Prescaler de 128
}

void convertADC(char channel) //Funcion para leer info ADC
{	ADMUX &= ~(0x0F); // Borramos los ultimos 4 bits de ADMUX
	
	if (channel > 8) {channel = 8;} //valor maximo es 8
	
	ADMUX |= channel; // seleccionamos el canal correcto
	ADCSRA |= (1<<ADSC); // iniciamos el ADC 
	}

ISR(ADC_vect){
	if (ADCChannel == 6) {ADCResultServo1 = ADCH + 65;}
	else if (ADCChannel == 7) {ADCResultServo2 = ADCH/8 + 7;}
	ADCSRA |= (1<<ADIF);
	
	updateDutyCycle1A(ADCResultServo1);
	updateDutyCycle2B(ADCResultServo2);
	
	return;
	
}

char changesomebits(char oldvalue, char bitstochange, char newvalue) //Funcion para solo cambiar algunos bits en un registro
{	char result = (newvalue & bitstochange) | (oldvalue & ~bitstochange);
	return result;
}
