/******************************************************************************
; Universidad del Valle de Guatemala
; 1E2023: Programacion de Microcontroladores
; PWM.c
; Autor: Jacob Tabush
; Proyecto: Libreria PWM
; Hardware: ATMEGA328P
; Creado: 15/04/2024
; Ultima modificacion: 15/04/2024
*******************************************************************************/

#include "PWM.h"

void initPWM0FastA(uint8_t resetorno, uint8_t inverted, uint16_t prescaler){
	//Poner a PD6 como salida
	DDRD |= (1<<DDD6);
	
	if (resetorno) { //Para evitar eliminar la configuracion de una al modificar la otra
	TCCR0A = 0;
	TCCR0B = 0;}
	
	TCCR0A |= (1<<COM0A1); //Encendemos OC0A
	// Configurando OC0A como invertido o no invertido
	if (inverted) {TCCR0A |= (1<<COM0A0);}

	// Modo PWM Fast TOP = 0xFF
	TCCR0A |= (1<<WGM01)|(1<<WGM00);
	
	//Prescaler
	
	switch(prescaler) {
		case 1: TCCR0B |= (1<<CS00);
		break;
		
		case 8: TCCR0B |= (1<<CS01);
		break;
		
		case 64: TCCR0B |= (1<<CS01)|(1<<CS00);
		break;
		
		case 256: TCCR0B |= (1<<CS02);
		break;
		
		case 1024: TCCR0B |= (1<<CS02)|(1<<CS00);
		break;
		
		default: TCCR0B |= (1<<CS02)|(1<<CS00); //1024 de default
	}
	
	
	};

void initPWM0FastB(uint8_t resetorno, uint8_t inverted, uint16_t prescaler){
	//Poner a PD6 como salida
	DDRD |= (1<<DDD6);
	
	if (resetorno) {
	TCCR0A = 0;
	TCCR0B = 0;}
	
	TCCR0A |= (1<<COM0B1); //Encendemos OC0A
	// Configurando OC0A como invertido o no invertido
	if (inverted) {TCCR0A |= (1<<COM0B0);}

	// Modo PWM Fast TOP = 0xFF
	TCCR0A |= (1<<WGM01)|(1<<WGM00);
	
	//Prescaler
	
	switch(prescaler) {
		case 1: TCCR0B |= (1<<CS00);
		break;
		
		case 8: TCCR0B |= (1<<CS01);
		break;
		
		case 64: TCCR0B |= (1<<CS01)|(1<<CS00);
		break;
		
		case 256: TCCR0B |= (1<<CS02);
		break;
		
		case 1024: TCCR0B |= (1<<CS02)|(1<<CS00);
		break;
		
		default: TCCR0B |= (1<<CS02)|(1<<CS00); //1024 de default
	}
	
	
};

void initPWM1FastTopA(uint8_t resetorno, uint8_t inverted, uint16_t prescaler, uint16_t top){
	//Poner a PB1 como salida
	DDRB |= (1<<DDB1);
	
	if (resetorno) { //Para evitar eliminar la configuracion de una al modificar la otra
		TCCR1A = 0;
	TCCR1B = 0;}
	
	TCCR1C = 0;
	
	TCCR1A |= (1<<COM1A1); //Encendemos OC0A
	// Configurando OC0A como invertido o no invertido
	if (inverted) {TCCR1A |= (1<<COM1A0);}

	// Modo PWM Fast 10 bit TOP = ICR1
	TCCR1A |= (1<<WGM13)|(1<<WGM12)|(1<<WGM11);
	
	//Prescaler
	
	switch(prescaler) {
		case 1: TCCR1B |= (1<<CS10);
		break;
		
		case 8: TCCR1B |= (1<<CS11);
		break;
		
		case 64: TCCR1B |= (1<<CS11)|(1<<CS10);
		break;
		
		case 256: TCCR1B |= (1<<CS12);
		break;
		
		case 1024: TCCR1B |= (1<<CS12)|(1<<CS10);
		break;
		
		default: TCCR1B |= (1<<CS12)|(1<<CS10); //1024 de default
	}
	
	ICR1 = top;
};

void updateDutyCycle0A(uint8_t duty) {
	OCR0A = duty;}

void updateDutyCycle0B(uint8_t duty) {
	OCR0B = duty;}
	
void updateDutyCycle1A(uint16_t duty) {
	OCR1A = duty;
}