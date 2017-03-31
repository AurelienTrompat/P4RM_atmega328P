/*
 * P4RM_atmega328P.h
 *
 * Created: 29/12/2016 19:31:40
 *  Author: Aurelien
 */ 


#ifndef P4RM_ATMEGA328P_H_
#define P4RM_ATMEGA328P_H_

 #define F_CPU 16000000 // fréquence de l'horloge

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "PRobot.h"
 
PRobot Robot;
 
//Compteur
uint8_t compteur1s = 0;
uint8_t compteurTimeOut = 0;

//I2C
uint8_t vitesseMoteur = 0;
bool direction = false;
bool enableMoteur = false;
bool vitesseProgressive = false;
bool renvoieDistance = false;
bool razDefault = false;
uint8_t tempDistance= 0;

//flag
bool flagInitVitesseTransmission = false;
bool flagAsservisement = false;
bool flagBatterie = false;
bool actualiserVitesse = false;
bool premieroctet = false;
bool actualiserOrdre = false;
bool razI2C = false;
//fonction d'interruption
 ISR(TIMER2_COMPA_vect); //Timer 0 intérruptions toutes les 2 ms
 ISR(PCINT1_vect); //interruption externe broche default du driver moteur
 ISR(TWI_vect); // interruption généré par le bus I2C
 ISR(TIMER0_COMPA_vect);

#endif /* P4RM_ATMEGA328P_H_ */