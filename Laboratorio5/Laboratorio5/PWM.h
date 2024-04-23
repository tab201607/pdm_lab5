/******************************************************************************
; Universidad del Valle de Guatemala
; 1E2023: Programacion de Microcontroladores
; pwm.h
; Autor: Jacob Tabush
; Proyecto: Libreria PWM
; Hardware: ATMEGA328P
; Creado: 15/04/2024
; Ultima modificacion: 15/04/2024
*******************************************************************************/


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <stdint.h>

#define invertido 1 
#define no_invertido 0

#define reset 1
#define no_reset 0

void initPWM0FastA(uint8_t resetorno, uint8_t inverted, uint16_t prescaler);
void initPWM0FastB(uint8_t resetorno, uint8_t inverted, uint16_t prescaler);

void initPWM1FastTopA(uint8_t resetorno, uint8_t inverted, uint16_t prescaler, uint16_t top);

void initPWM2FastA(uint8_t resetorno, uint8_t inverted, uint16_t prescaler);
void initPWM2FastB(uint8_t resetorno, uint8_t inverted, uint16_t prescaler);
void initPWM2FastTop(uint8_t inverted, uint16_t prescaler, uint8_t top);	

void updateDutyCycle0A(uint8_t duty);
void updateDutyCycle0B(uint8_t duty);

void updateDutyCycle1A(uint16_t duty);

void updateDutyCycle2A(uint8_t duty);
void updateDutyCycle2B(uint8_t duty);

#endif /* PWM_H_ */