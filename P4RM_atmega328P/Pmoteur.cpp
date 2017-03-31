/*
 * Pmoteur.cpp
 *
 * Created: 12/11/2016 11:43:41
 *  Author: Aurelien
 */ 
#include "Pmoteur.hpp"

PMoteur::PMoteur() :  mCommande(0), mConsigne(0), mPwmConfigurer(false),mDirection(false)
{
	GenererPwm(); //intialisation tu timer 1 pour la PWM
}

PMoteur::~PMoteur()
{
	
}

void PMoteur::ChangerCommande(uint16_t nouvCommande) //applique le nouveau rapport cyclique de la pwm
{	
	OCR1A = nouvCommande;
}

void PMoteur::GenererPwm() //initialisation du timer 1
{
		if (!mPwmConfigurer)
		{
			TCCR1A = (1 << WGM11);
			TCCR1A &= ~(1 << WGM10);
			TCCR1B = (1 << WGM13)|(1<<WGM12);//PWM en mode 14, fast pwm, TOP=ICR1, (voir doc page 133)
			
			TCCR1A |= (1 << COM1A1);
			TCCR1A &= ~(1 << COM1A0);//Clear OC1A on compare match, set OC1A at bottom (non inverting mode)
			
			TCCR1B |= (1<<CS10);//pas de division de fréquence
			TCCR1B &= ~(1<<CS11 | 1<<CS12);
			
			mNombrePeriode=(uint16_t)((double)(F_CPU)/FREQUENCE_PWM);
			ICR1 = (uint16_t)mNombrePeriode; 
			mPwmConfigurer=true;
		}
	
}

void PMoteur::DemarrerMoteur() //enable la PWM
{
	if (mPwmConfigurer && (PINC&1)==0)
	{
		PORTC |= (1<< PORTC1);
	}
}

void PMoteur::EteindreMoteur() //disable la PWM
{
	mCommande=0;
	PORTC&= ~(1 << PORTC1);
	
}

void PMoteur::ChangerDirection(bool nouvDirection) //change l'etat de la broche reliè a l'entrée qui définit le sens de rotation des moteurs
{
	mDirection=nouvDirection;
	if (nouvDirection)
	{
		PORTB |=(1<<PORTB2);
	} 
	else
	{
		PORTB &= ~(1<<PORTB2);
	}
}

void PMoteur::ChangerConsigne(uint8_t nouvConsigne) //enregistre une nouvelle consigne de vitesse reçu de la RPI
{
	mConsigne=(double)(nouvConsigne*3.1372549);
}

void PMoteur::CalculCommande() //calcul la nouvelle commande en fonction de la commande actuelle, de la consigne et de l'option vitesse progressive
{
	if (vitesseProgressive)
	{
		
		if (mCommande>=mConsigne+10)
		{
				mCommande -= 10;
		}
		else if (mCommande<= mConsigne-10)
		{
			mCommande +=10;
		}
		else 
		{
			mCommande=(int32_t)mConsigne;
		}
	}
	else 
		mCommande=mConsigne;
		
	if (mCommande>800)
	{
		mCommande=800;
	} 
	else if(mCommande<0)
	{
		mCommande=0;
	}
	
	ChangerCommande((uint16_t)(mCommande));
}