/* 
* PRouesCodeuses.cpp
*
* Created: 30/12/2016 11:21:58
* Author: Aurelien
*/


#include "PRouesCodeuses.h"

// default constructor
PRouesCodeuses::PRouesCodeuses()
{
	ConfigCompteur();
} //PRouesCodeuses

// default destructor
PRouesCodeuses::~PRouesCodeuses()
{
	
} //~PRouesCodeuses

void PRouesCodeuses::ConfigCompteur()
{
	
	TCCR0A &= (~(1<<COM0A1))& (~(1<<COM0A0))& (~(1<<COM0B1))& (~(1<<COM0B0)); //OC0A et OC0B non connecté au timer0
	//mode normal TOP=255
	TCCR0A &= ~(1<<WGM00) & (~(1<<WGM01));
	TCCR0B = ~(1<<WGM02); 
	
	//clk -> front montant sur T0
	TCCR0B |= (1<<CS02) | (1<<CS01) | (1<<CS00);
	
	TIMSK0 = (1<<TOIE0); // interruption quand la valeur du compteur atteint la valeur du registre de comparaison OCR0A	
	TIMSK0 &= ~(1<<OCIE0B)& ~(1<<OCIE0A);
}