/******************************************************************************
; Universidad del Valle de Guatemala
; 1E2023: Programacion de Microcontroladores
; main.c
; Autor: Jacob Tabush
; Proyecto: Poslaboratorio 5
; Hardware: ATMEGA328P
; Creado: 22/04/2024
; Ultima modificacion: 23/04/2024
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

uint16_t ADCResultServo1 = 0;
uint8_t ADCResultServo2 = 0;
uint8_t ADCResultLED = 0;
uint8_t ADCChannel = 6;

void setup(void)
{
	cli();
	
	CLKPR = (0b10000000); // habilitamos cambios del prescale
	CLKPR = (0x03); // Colocamos prescaler de 8


	initPWM0Fake(0, 255, 255, 1); // activamos el timer 0 en pwm falso
	initPWM1FastTopA(reset, no_invertido, 8, 2499); //activamos el timer 1 en modo pwm
	initPWM2FastB(reset, no_invertido, 128); // Activamos el timer 2 en modo pwm

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
	
	
	while (1) //Ejecutamos los ADCs de los 3 channels
	{
		ADCChannel = 5; //iniciamos el ADC convirtiendo el canal 5
		convertADC(ADCChannel);
		_delay_ms(100);
		
		ADCChannel = 6; //iniciamos el ADC convirtiendo el canal 6
		convertADC(ADCChannel);
		_delay_ms(100);
		
		ADCChannel = 7; //iniciamos el ADC convirtiendo el canal 6
		convertADC(ADCChannel);
		_delay_ms(100);
	}
}

//////////////////////////////////////////////////////
// Funciones ADC
//////////////////////////////////////////////////////

void initADC(void) //Funcion para inicializar el ADC
{
	ADMUX = 0;
	
	ADMUX |= (1<<REFS0); //conectamos a AVcc
	ADMUX &= ~(1<<REFS1);
	
	ADMUX |= (1<<ADLAR); // Justificado a la izquierda
	
	ADCSRA |= (1<<ADEN); //Encendemos el ADC
	ADCSRA |= (1<<ADIE); // Encendemos el interrupt
	ADCSRA |= (0b00000011); //Prescaler de 128
}

void convertADC(char channel) //Funcion para leer info ADC
{	ADMUX &= ~(0x0F); // Borramos los ultimos 4 bits de ADMUX
	
	if (channel > 8) {channel = 8;} //valor maximo es 8
	
	if (channel < 6) {DIDR0 = channel;}
	
	ADMUX |= channel; // seleccionamos el canal correcto
	ADCSRA |= (1<<ADSC); // iniciamos el ADC
}

//////////////////////////////////////////////////////
// Funciones ISR
//////////////////////////////////////////////////////

ISR(ADC_vect){
	if (ADCChannel == 5) {ADCResultServo1 = ADCH + 65;}
	else if (ADCChannel == 6) {ADCResultServo2 = ADCH/8 + 7;}
	else if (ADCChannel == 7) {ADCResultLED = ADCH;}
	ADCSRA |= (1<<ADIF);
	
	updateDutyCycle1A(ADCResultServo1);
	updateDutyCycle2B(ADCResultServo2);
	updateDutyCycleFake(ADCResultLED);
	
	return;
}

ISR(TIMER0_OVF_vect) {
	fakePWMcounter++;
	TCNT0 = 255 - fakePWMtimertop; // reiniciamos el timer
	
	if (fakePWMcounter == fakePWMtop) {
		fakePWMcounter = 0; // reiniciamos el counter externo
		PORTB |= (1<<PORTB5); // encendemos el pin
	}
	
	if (fakePWMcounter == fakePWMduty) {
		PORTB &= ~(1<<PORTB5); //Apagamos el pin
	}
	
	TIFR0 |= TOV0;
}